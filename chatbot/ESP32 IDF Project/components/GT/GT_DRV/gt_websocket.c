/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* ESP HTTP Client Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
// #include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_websocket_client.h"
#include "esp_event.h"
#include <cJSON.h>

#include "gt_websocket.h"

#define NO_DATA_TIMEOUT_SEC 60

#define MC_API_KEY

static const char *TAG = "websocket";

static TimerHandle_t shutdown_signal_timer;
static SemaphoreHandle_t shutdown_sema;


static esp_websocket_client_handle_t client = NULL;

bool key_is_press = false; 
static bool isFirstAudiouri = false;
ReceivedAnswerData* chat_bot_receive = NULL;
static GT_PROTOCOL* gt_pro = NULL;

static WEB_HISTORY_DATA* web_history_data[WEB_HISTORY_ARRY*2] = {NULL};
extern ChatbotData cb_data;
extern QueueHandle_t gui_task_queue;
extern SemaphoreHandle_t key_mutex;
BaseType_t xTaskWoken_mutex = pdFALSE;
BaseType_t xTaskWoken_queue = pdFALSE;
extern SemaphoreHandle_t audio_event_mutex;
SemaphoreHandle_t web_history_sem = NULL; //wait update
static WEBSOCKET_STATUS websocket_status_flag = 0;



static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static void shutdown_signaler(TimerHandle_t xTimer)
{
    ESP_LOGI(TAG, "No data received for %d seconds, signaling shutdown", NO_DATA_TIMEOUT_SEC);
    // xSemaphoreGive(shutdown_sema);
}

#if CONFIG_WEBSOCKET_URI_FROM_STDIN
static void get_string(char *line, size_t size)
{
    int count = 0;
    while (count < size) {
        int c = fgetc(stdin);
        if (c == '\n') {
            line[count] = '\0';
            break;
        } else if (c > 0 && c < 127) {
            line[count] = c;
            ++count;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

#endif /* CONFIG_WEBSOCKET_URI_FROM_STDIN */

static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    static uint8_t event_type = 0;
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_CONNECTED");
        websocket_status_flag = WEBSOCKET_CONNECTED;
        gt_websocket_client_request();
        
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DISCONNECTED");
        websocket_status_flag = WEBSOCKET_DISCONNECTED;
        log_error_if_nonzero("HTTP status code",  data->error_handle.esp_ws_handshake_status_code);
        if (data->error_handle.error_type == WEBSOCKET_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", data->error_handle.esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", data->error_handle.esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  data->error_handle.esp_transport_sock_errno);
        }
        break;
    case WEBSOCKET_EVENT_DATA:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DATA");
        ESP_LOGI(TAG, "Received opcode=%d", data->op_code);
        if (data->op_code == 0x08 && data->data_len == 2) {
            ESP_LOGW(TAG, "Received closed message with code=%d", 256 * data->data_ptr[0] + data->data_ptr[1]);
        }

        key_is_press = false;
        char* jbuf = (char*)audio_malloc(data->data_len+1);
        memset(jbuf, 0, data->data_len+1);
        memcpy(jbuf, data->data_ptr, data->data_len);
        int res = websocket_chatbot_json_parse(jbuf, chat_bot_receive);        
        printf("res = %d\r\n", res);
        switch (res)
        {
        case WEB_PARSE_SUCCESSED:
            ESP_LOGI(TAG, "WEB_PARSE_SUCCESSED");
            xSemaphoreTake(audio_event_mutex, portMAX_DELAY);
            if(key_is_press == false)
            {
                gt_pro->head_type = REC_WEB_DATA;
                gt_pro->data = chat_bot_receive;
                xQueueSend(gui_task_queue, &gt_pro, portMAX_DELAY);
            
                if(isFirstAudiouri == true)
                {     
                    if(chat_bot_receive->tts_audio != NULL && strcmp(chat_bot_receive->tts_audio, "") != 0)
                    {
                        event_type  = AUDIO_RUNNING;
                        xQueueSendFromISR(audio_event_queue, &event_type, &xTaskWoken_queue);
                        isFirstAudiouri = false;
                    }  
                }
            }
            xSemaphoreGive(audio_event_mutex);
            break;

        case WEB_DATA_ERROR:
            ESP_LOGW(TAG, "WEB_DATA_ERROR");
            xSemaphoreTake(audio_event_mutex, portMAX_DELAY);
            event_type  = AUDIO_GENERATE_ERROR;
            xQueueSendFromISR(audio_event_queue, &event_type, &xTaskWoken_queue);
            
            gt_pro->head_type = REC_WEB_DATA;
            gt_pro->data = chat_bot_receive;
            xQueueSend(gui_task_queue, &gt_pro, portMAX_DELAY);
            xSemaphoreGive(audio_event_mutex);
            break;
        
        case WEB_AGENT_STOP: 
            ESP_LOGW(TAG, "WEB_AGENT_STOP");
            break;
        case WEB_AGENT_STOP_FORCE:
            ESP_LOGI(TAG, "WEB_AGENT_STOP_FORCE");
            break;
        default:
            ESP_LOGW(TAG, "chat_bot_receive = NULL");
            break;
        }
        audio_free(jbuf);
        jbuf = NULL;
        ESP_LOGW(TAG, "Total payload length=%d, data_len=%d, current payload offset=%d\r\n", data->payload_len, data->data_len, data->payload_offset);
        break;

    case WEBSOCKET_EVENT_ERROR:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_ERROR");
        log_error_if_nonzero("HTTP status code",  data->error_handle.esp_ws_handshake_status_code);
        if (data->error_handle.error_type == WEBSOCKET_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", data->error_handle.esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", data->error_handle.esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  data->error_handle.esp_transport_sock_errno);
        }
        break;
    }
}

void gt_websocket_client_request()
{
    char data[150] = {0};
    snprintf(data, sizeof(data), "{\"socket_type\":\"agent_event\",\"event_name\":\"auth_token\",\"event_params\": \
                    {\"token\":\"%s\"}}", API_KEY);
    esp_websocket_client_send_text(client, data, strlen(data), portMAX_DELAY);
}

int check_opcode(unsigned char opcode)
{
    if(opcode == 1002 || opcode == 1007)
    {
        xTimerDelete(shutdown_signal_timer, portMAX_DELAY);
        vSemaphoreDelete(shutdown_sema);
        esp_websocket_client_start(client);
        gt_websocket_client_request();
        return ESP_FAIL;
    }

    return ESP_OK;
}

void gt_websocket_client_checkroom_isexist()
{
    char data_create[] = {"{\"socket_type\":\"interface_call\",\"event_name\":\"session_check\",\"event_params\": \
                            {\"session_token\":\"room_id\"}}"};
    esp_websocket_client_send_text(client, data_create, strlen(data_create), portMAX_DELAY);
}

void gt_websocket_client_create_room()
{
    char data_create[] = {"{\"socket_type\":\"interface_call\",\"event_name\":\"create_session\",\"event_params\":\
                            {\"session_name\":\"test\",\"session_remark\":\"{}\",\"config_data\":{},\"model\": \
                            \"deepseek-chat\",\"prompt\":\"#\",\"temperature\":0.2,\"max_tokens\":4000,\"history_length\":8}}"};
    esp_websocket_client_send_text(client, data_create, strlen(data_create), portMAX_DELAY);
}

void gt_websocket_client_get_history_message()
{
    char data_buf[256] = {0};
    snprintf(data_buf, 256, "{\"socket_type\":\"interface_call\",\"event_name\":\"message_list\",\"event_params\": \
                             {\"session_token\":\"%s\",\"size\":6}}", chat_bot_receive->session_token);
    esp_websocket_client_send_text(client, data_buf, strlen(data_buf), portMAX_DELAY);
}

void gt_websocket_client_clear_history_message()
{
    char data_buf[256] = {0};
    snprintf(data_buf, 256, "{\"socket_type\":\"interface_call\",\"event_name\":\"message_clear\",\"event_params\": \
                            {\"session_token\":\"%s\"}}", chat_bot_receive->session_token);
    esp_websocket_client_send_text(client, data_buf, strlen(data_buf), portMAX_DELAY);
}

void gt_websocket_client_create_task()
{
    ESP_LOGE(TAG, "gt_websocket_client_create_task");
    int i = 0;
    char buf[30] = {0};
    char personality[20] = {0};
    bool emotion_output = true;
    char data_create_2[2500] = {0};
    emotion_output = strcmp(cb_data.settings->emotion_value, "打开") == 0 ? true : false;
    emotion_output = true;
    sprintf(buf, "%s,%s,%s,%s", cb_data.settings->bot_personality[0], cb_data.settings->bot_personality[1], cb_data.settings->bot_personality[2], cb_data.settings->bot_personality[3]);
#if 0
    snprintf(data_create_2, sizeof(data_create_2), "{\"socket_type\": \"agent_event\",\"event_name\": \"agent_action\",\
    \"event_params\": { \"agent_name\": \"chat_bot_bak\",\"upload_format\": \"pcm\",\
    \"upload_sample_rate\": 16000,\"mode\": \"customize\",\
    \"emotion_output\": %d,\"voice_id\": \"%s\",\
    \"speed\": 1, \"user_name\": \"%s\",\"user_age\": %d,\"bot_name\": \"%s\",\
    \"bot_character_description\": \"%s\",\
    \"bot_character\": \"朋友\",\"bot_personality\": \"%s\",\
    \"bot_tone\": \"%s\",\"llm_model\": \"abab6.5s-chat\",\
    \"max_tokens\": 100,\"tts_model\": \"MM_TTSL_realtime_speech-01-turbo\",\
    \"session_token\":\"%s\",\
    \"language\": \"中文\"}}",\
    emotion_output, cb_data.settings->voice_id,cb_data.settings->user_name, cb_data.settings->user_age, cb_data.settings->bot_name,\
    cb_data.settings->bot_description, buf, cb_data.settings->bot_tone, "");


    char data_create[] = {"{\"socket_type\": \"agent_event\",\"event_name\": \"agent_action\", \
                            \"event_params\": { \"agent_name\": \"chat_bot_v2\",\"upload_format\": \"pcm\", \
                            \"upload_sample_rate\": 16000,\"mode\": \"customize\", \
                            \"emotion_output\": true,\"voice_id\": \"cute_boy\", \
                            \"speed\": 1,\"user_age\": 5,\"bot_name\": \"智酱\", \
                            \"bot_description\": \"你是住在芯片王国里的AI机器人，可以帮助用户解决各式各样的问题\", \
                            \"bot_character\": \"朋友\",\"bot_personality\": \"有趣\", \
                            \"bot_response_style\": \"normal\",\"llm_model\": \"abab6.5s-chat\", \
                            \"max_tokens\": 100,\"tts_model\": \"MM_TTSL_realtime_speech-01-turbo\",\"asr_model\": \"BD_ASR_realtime\"}}"};
    esp_websocket_client_send_text(client, data_create_2, strlen(data_create_2), portMAX_DELAY);

#elif 0
    sprintf(buf, "%s,%s,%s,%s", "活泼", "好奇", "乐观", "善良");
    snprintf(data_create_2, sizeof(data_create_2), "{\"socket_type\":\"agent_event\",\"event_name\":\"agent_action\",\
    \"event_params\":{\"agent_name\":\"chat_bot_v2\",\
    \"mode\":\"customize\",\"emotion_output\":%d,\"voice_id\":\"%s\",\
    \"tts_model\":\"MM_TTSL_realtime_speech-01-turbo\",\"asr_model\":\"ALI_ASR_realtime_paraformer-realtime-v2\",\"upload_format\":\"pcm\",\
    \"upload_language\":\"zh\",\"upload_sample_rate\":\"16000\",\"tts_language\":\"Chinese\",\"language\":\"中文(zh)\",\"user_name\":\"%s\",\
    \"user_age\":%d,\"bot_name\":\"%s\",\"bot_character_description\":\"%s\",\
    \"bot_personality\":\"%s\",\"bot_tone\":\"%s\",\
    \"max_tokens\":100,\"session_token\":\"%s\"}}",\
    false, "MCV-34-b051f129c8e94235826803f61601b53e", "海扁王", 10, "智酱", "智酱是一个彩色果酱生物，它是一个网红AI，拥有着丰富的知识和技能。它不仅能够帮助用户解决问题，还能提供有趣的信息和建议。", buf,\ 
    "使用充满活力和幽默的口语化方式与用户互动，喜欢使用网络流行语和表情包，营造轻松愉快的氛围。习惯使用emoji表情。", "");

#else
    sprintf(buf, "%s,%s,%s,%s", "活泼", "好奇", "乐观", "善良");
    snprintf(data_create_2, sizeof(data_create_2), "{\"socket_type\":\"agent_event\",\"event_name\":\"agent_action\",\
    \"event_params\":{\"agent_name\":\"chat_bot_v2\",\
    \"mode\":\"customize\",\"emotion_output\":%d,\"voice_id\":\"%s\",\
    \"tts_model\":\"MM_TTSL_realtime_speech-01-turbo\",\"asr_model\":\"ALI_ASR_realtime_paraformer-realtime-v2\",\"upload_format\":\"pcm\",\
    \"upload_language\":\"zh\",\"upload_sample_rate\":\"16000\",\"tts_language\":\"Chinese\",\"language\":\"中文(zh)\",\"user_name\":\"%s\",\
    \"user_age\":%d,\"bot_name\":\"%s\",\"bot_character_description\":\"%s\",\
    \"bot_personality\":\"%s\",\"bot_tone\":\"%s\",\
    \"max_tokens\":100,\"session_token\":\"%s\"}}",\
    false, "male-qn-jingying-jingpin", "海扁王", 10, "智酱", "智酱是一个精通语文和历史的语文老师，拥有丰富的知识，能解决用户提出的与历史和文学相关的问题，还能提供有趣的信息和建议。", buf,\ 
    "使用充满活力和幽默的口语化方式与用户互动，喜欢使用网络流行语和表情包，营造轻松愉快的氛围。习惯使用emoji表情。", "");

#endif
    esp_websocket_client_send_text(client, data_create_2, strlen(data_create_2), portMAX_DELAY);
    
}

void gt_websocket_client_stop_send_audio_data()
{
    ESP_LOGE(TAG, "gt_websocket_client_stop_send_audio_data");
    char data_create[] = {"{\"socket_type\": \"intervent_event\", \"event_name\":\"agent_action\"}"};
    esp_websocket_client_send_text(client, data_create, strlen(data_create), portMAX_DELAY);
}

void gt_websocket_client_stop_receive_data()
{
    ESP_LOGE(TAG, "gt_websocket_client_stop_receive_data");
    char data_create[] = {"{\"socket_type\": \"intervent_event\",\"event_name\": \"stream_output\"}"};
    esp_websocket_client_send_text(client, data_create, strlen(data_create), portMAX_DELAY);
}

static void malloc_web_history_data()
{
    int i = 0;
    for(; i < WEB_HISTORY_ARRY*2; i++)
    {
        web_history_data[i] = (WEB_HISTORY_DATA*)audio_malloc(sizeof(WEB_HISTORY_DATA));
    }
}

void gt_websocket_client_init()
{
    esp_websocket_client_config_t websocket_cfg = {};

    // shutdown_signal_timer = xTimerCreate("Websocket shutdown timer", NO_DATA_TIMEOUT_SEC * 1000 / portTICK_PERIOD_MS,
    //                                      pdFALSE, NULL, shutdown_signaler);
    // shutdown_sema = xSemaphoreCreateBinary();

    websocket_cfg.uri = WEB_LINK;
    websocket_cfg.reconnect_timeout_ms = 10000;
    websocket_cfg.network_timeout_ms = 6000;
    websocket_cfg.buffer_size = 4096;
    websocket_cfg.pingpong_timeout_sec = 15;
    websocket_cfg.task_prio = 3;
    ESP_LOGI(TAG, "Connecting to %s...", websocket_cfg.uri);

    client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    // esp_websocket_client_start(client);
    // xTimerStart(shutdown_signal_timer, portMAX_DELAY);
    // gt_websocket_client_request();
    // xSemaphoreTake(shutdown_sema, portMAX_DELAY);
    
    malloc_web_history_data();
    
    chat_bot_receive = (ReceivedAnswerData*)audio_malloc(sizeof(ReceivedAnswerData));
    memset(chat_bot_receive, 0, sizeof(ReceivedAnswerData));
    chat_bot_receive->is_web_first_response = true;

    gt_pro = (GT_PROTOCOL*)audio_malloc(sizeof(GT_PROTOCOL));
    memset(gt_pro, 0, sizeof(GT_PROTOCOL));

    web_history_sem = xSemaphoreCreateBinary();
}

int gt_web_status()
{
    return websocket_status_flag;
}

void gt_websocket_send_data(char* data)
{
    
    ESP_LOGI(TAG, "gt_websocket_send_data =====================");
    if (false == esp_websocket_client_is_connected(client)) 
    {
        ESP_LOGE(TAG, "esp_websocket_client is not connected");
        return;
    }
    // esp_websocket_client_send_bin_partial(client, data, 1024, portMAX_DELAY);
    // esp_websocket_client_send_cont_msg(client, data+1024, 1024, portMAX_DELAY);
    // esp_websocket_client_send_fin(client, portMAX_DELAY);
    
    esp_websocket_client_send_bin(client, data, 1024, portMAX_DELAY);
    esp_websocket_client_send_bin(client, data + 1024, 1024, portMAX_DELAY);

    // esp_websocket_client_send_bin_partial(client, data, 2048, portMAX_DELAY);
    // esp_websocket_client_send_bin_partial(client, data + 2048, 2048, portMAX_DELAY);
    ESP_LOGI(TAG, "esp_websocket_client send successful !!!!!!!");
}

int gt_websocket_send_data_partial_fin()
{
    return esp_websocket_client_send_fin(client, portMAX_DELAY);
}

bool gt_websocket_client_state()
{
    ESP_LOGI(TAG, "gt_websocket_client_state");
    return esp_websocket_client_is_connected(client);
}

void gt_websocket_client_start()
{
    ESP_LOGI(TAG, "gt_websocket_client_start");
    int gt_res = 0;
    gt_res = esp_websocket_client_start(client);
 
    if(gt_res == ESP_FAIL)
    {
        ESP_LOGE(TAG, "esp_websocket_client_start is FAIL");
    }
    else
    {
        ESP_LOGI(TAG, "esp_websocket_client_start is OK");
    }
}

void gt_websocket_client_close()
{
    ESP_LOGI(TAG, "gt_websocket_client_close");
    int gt_res = 0;
    gt_res = esp_websocket_client_close(client, portMAX_DELAY);
    websocket_status_flag = WEBSOCKET_DISCONNECTED;
    if(gt_res == ESP_FAIL)
    {
        ESP_LOGE(TAG, "gt_websocket_client_close is FAIL");
    }
}

void gt_websocket_client_destroy()
{
    int gt_res = 0;
    gt_res = esp_websocket_client_destroy(client);
    if(gt_res == ESP_FAIL)
    {
        ESP_LOGE(TAG, "gt_websocket_client_destroy is FAIL");
    }
}

   
void set_isFirstAudiouri(bool value)
{
    chat_bot_receive->is_web_first_response = value;
    isFirstAudiouri = value;
}

bool get_isFirstAudiouri()
{
    return isFirstAudiouri;
}

char* get_chatbot_audio_uri()
{
    if(chat_bot_receive->tts_audio != NULL)
    {
        ESP_LOGE(TAG, "tts_audio is not NULL");
        return chat_bot_receive->tts_audio;
    }
    else
    {
        ESP_LOGE(TAG, "chat_bot_receive->tts_audio is NULL");
        return NULL;
    }
}

void free_chatbot_audio_uri()
{
    if(chat_bot_receive->tts_audio != NULL)
    {
        audio_free(chat_bot_receive->tts_audio);
        chat_bot_receive->tts_audio = NULL;
    }
    else
    {
        ESP_LOGE(TAG, "tts_audio is NULL");
    }

}

static void free_data(void* ptr)
{
    ESP_LOGI(TAG, "free_data");
    if(ptr != NULL)
    {
        audio_free(ptr);
        ptr = NULL;
    }
}

void web_receive_data_free()
{
    if(chat_bot_receive->llm_response != NULL)
    {
        audio_free(chat_bot_receive->llm_response);
        chat_bot_receive->llm_response = NULL;
    }
    if(chat_bot_receive->emotion_value != NULL)
    {
        audio_free(chat_bot_receive->emotion_value);
        chat_bot_receive->emotion_value = NULL;
    }
    if(chat_bot_receive->tts_audio != NULL)
    {
        audio_free(chat_bot_receive->tts_audio);
        chat_bot_receive->tts_audio = NULL;
    }
}

void set_key_is_press(bool value)
{
    key_is_press = value;
}

void set_web_session_token(char* web_room_id, int len)
{
    chat_bot_receive->session_token = (char*)audio_malloc(len+1);
    memset(chat_bot_receive->session_token, 0, len+1);
    strcpy(chat_bot_receive->session_token, web_room_id);
}

WEB_HISTORY_DATA** get_web_history_array()
{
    return web_history_data;
}

int websocket_parse_socket_message(char *jbuf)
{
    cJSON *json = cJSON_Parse(jbuf);
    if (json == NULL) {
        ESP_LOGE(TAG, "Failed to parse JSON");
        return -1;  // 返回失败
    }
    else
    {
        ESP_LOGI(TAG, "Successfull to parse JSON");
    }
    cJSON *socket_message = NULL;
    socket_message = cJSON_GetObjectItem(json, "socket_message");
    if(socket_message != NULL)
    {
        if(strcmp(socket_message->valuestring, "connection_established") == 0)
        {
            return ESP_OK;
        }
    }
    return ESP_FAIL;
}

int websocket_chatbot_json_parse(char *jbuf, ReceivedAnswerData* receive_buf)
{
    // 解析 JSON 响应
    int res = WEB_OTHER;
    ESP_LOGI(TAG, "jbuf = %s", jbuf);
    cJSON *json = cJSON_Parse(jbuf);
    if (json == NULL) {
        ESP_LOGE(TAG, "Failed to parse JSON");
        return WEB_PARSE_FAIL;  // 返回失败
    }
    else
    {
        ESP_LOGI(TAG, "Successfull to parse JSON");
    }
    char* socket_message_string = NULL;
    cJSON *socket_type = NULL, *interface_name = NULL, *socket_message = NULL;
    cJSON *agent_output = NULL, *socket_data = NULL, *event_name = NULL, *socket_status = NULL;

    do{
        socket_type = cJSON_GetObjectItem(json, "socket_type");
        GET_VALUEINT(socket_status, json, "socket_status", receive_buf->socket_status);
        if(receive_buf->socket_status == 3001 || receive_buf->socket_status == 4006)
        {
            cJSON_Delete(json);
            return WEB_DATA_ERROR;
        }
        GET_VALUESTRING(socket_message, json, "socket_message", socket_message_string);
        if(strcmp(socket_message_string, "agent_stop") == 0)
        {
            audio_free(socket_message_string);
            cJSON_Delete(json);
            return WEB_AGENT_STOP;
        }
        else if(strcmp(socket_message_string, "agent_stop_force") == 0)
        {
            audio_free(socket_message_string);
            cJSON_Delete(json);
            return WEB_AGENT_STOP_FORCE;
        }
    }while(0);

    agent_output = cJSON_GetObjectItem(json, "agent_output");
    socket_data = cJSON_GetObjectItem(json, "socket_data");
    if(agent_output == NULL && socket_data == NULL)
    {
        ESP_LOGE(TAG, "Failed to parse socket_data and agent_output");
        cJSON_Delete(json);
        return WEB_PARSE_FAIL;
    }

    ESP_LOGI(TAG, "Successfull to parse socket_data and agent_output");
    if(strcmp(socket_type->valuestring, "agent_event") == 0) //机器人回复
    {
        cJSON* recover_json = NULL;
        do{
            GET_VALUESTRING(recover_json, agent_output, "llm_response", receive_buf->llm_response);
            GET_VALUESTRING(recover_json, agent_output, "tts_audio", receive_buf->tts_audio);
            GET_VALUEDOUBLE(recover_json, agent_output, "to_sleep", receive_buf->audio_seconds);
            GET_VALUESTRING(recover_json, agent_output, "emotion_value", receive_buf->emotion_value);
            res = WEB_PARSE_SUCCESSED;
        }while(0);
    }
    else if(strcmp(socket_type->valuestring, "interface_call") == 0)
    {
        cJSON *recover_json = NULL, *data_array = NULL, *result = NULL, *result_size = NULL;
        int array_index = 0, history_count = 0;
        interface_name = cJSON_GetObjectItem(socket_data, "interface_name");
        if(strcmp(interface_name->valuestring, "message_list") == 0)
        {
            result = cJSON_GetObjectItem(socket_data, "result");
            result_size = cJSON_GetObjectItem(socket_data, "result_size");
            web_history_data[0]->result_size = result_size->valueint;
            do{
                data_array = cJSON_GetArrayItem(result, array_index);
                GET_VALUESTRING(recover_json, data_array, "message_content", web_history_data[array_index]->message_content);
                GET_VALUESTRING(recover_json, data_array, "message_category", web_history_data[array_index]->message_category);
                array_index++;
            }while(array_index < web_history_data[0]->result_size);
            xSemaphoreGive(web_history_sem);
        }
        else if(strcmp(interface_name->valuestring, "create_session") == 0)
        {
            do{
                GET_VALUESTRING(recover_json, socket_data, "session_token", receive_buf->session_token);
                ESP_LOGI(TAG, "receive_buf->session_token = %s", receive_buf->session_token);
                xQueueSend(web_room_id_queue, &receive_buf->session_token, portMAX_DELAY);
            }while(0);
        }
        res = WEB_OTHER;
    }

    cJSON_Delete(json);
    return res;  // -1:解析失败 0:解析成功

}
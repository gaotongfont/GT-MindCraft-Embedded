#ifndef __GT_UI_H__
#define __GT_UI_H__


#include "gt.h"
#include "gt_recording.h"
#include "gt_audio_play.h"
#include "http_send.h"
#include "gt_pipe_send.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "gt_font_config.h"
#include "audio_mem.h"
#include "wifi.h"
#include "gt_task_protocol.h"

#include "gt_record_http.h"
#include "gt_nvs_store.h"
// #include "gt_i2s.h"
#include "gt_pipeline_play.h"
#include "gt_audio_event.h"
#include "gt_audio_storage.h"
#include "gt_role_emote.h"
#include "gt_websocket.h"

extern SemaphoreHandle_t key_mutex;
extern SemaphoreHandle_t scr_id_mutex;
extern SemaphoreHandle_t audio_event_mutex;

extern QueueHandle_t audio_event_queue;
extern QueueHandle_t wifi_task_queue;

extern gt_obj_st * screen_home;
extern gt_obj_st * screen_setup;
extern gt_obj_st * screen_subtitle;
extern gt_obj_st * main_interface;
extern gt_obj_st * setting_list;
extern gt_obj_st * volume_and_brightness;
extern gt_obj_st * screen_keyboard;
extern gt_obj_st * forget_password;
extern gt_obj_st * connection_failed;
extern gt_obj_st * wifi_list;
extern gt_obj_st * History_page;
extern gt_obj_st * Function_settings;

extern ChatbotData cb_data;

extern void print_memory_info(void);

extern uint8_t selected_idx;

extern bool allow_reconnect;

void gt_ui_init(void);


gt_obj_st * gt_init_screen_home(void);
gt_obj_st * gt_init_screen_setup(void);
gt_obj_st * gt_init_screen_subtitle(void);
gt_obj_st * gt_init_main_interface(void);
gt_obj_st * gt_init_setting_list(void);
gt_obj_st * gt_init_volume_and_brightness(void);
gt_obj_st * gt_init_keyboard(void);
gt_obj_st * gt_init_forget_password(void);
gt_obj_st * gt_init_connection_failed(void);
gt_obj_st * gt_init_wifi_list(void);
gt_obj_st * gt_init_History_page(void);
gt_obj_st * gt_init_Function_settings(void);
typedef enum {
        GT_ID_SCREEN_HOME = 0,
        GT_ID_SCREEN_SETUP,
        GT_ID_SCREEN_SUBTITLE,
        GT_ID_MAIN_INTERFACE,
        GT_ID_SETTING_LIST,
        GT_ID_VOLUME_AND_BRIGHTNESS,
        GT_ID_KEYBOARD,
        GT_ID_FORGET_PASSWORD,
        GT_ID_CONNECTION_FAILED,
        GT_ID_WIFI_LIST,
        GT_ID_HISTORY_PAGE,
        GT_ID_FUNCTION_SETTINGS
} gt_scr_id_et;

typedef enum {
    AI_EMOJIS_WAITING = 0,
	AI_EMOJIS_RECORDING,
	AI_EMOJIS_SPEAKING,
    AI_EMOJIS_SYMPATHY,
    AI_EMOJIS_ENCOURAGE,
    AI_EMOJIS_HAPPY,
    AI_EMOJIS_CJ,
    AI_ANIM_AUDIO,
    AI_ANIM_SMILE,
    AI_EMOTE_XIAOZHI_NEUTRAL = 0,
    AI_EMOTE_XIAOZHI_DISGUST,
    AI_EMOTE_XIAOZHI_FEAR,
    AI_EMOTE_XIAOZHI_SADNESS,
    AI_EMOTE_XIAOZHI_SURPRISE,
    AI_EMOTE_XIAOZHI_ANGER,
    AI_EMOTE_XIAOZHI_HAPPY,
    AI_EMOTE_CAIJI_NEUTRAL,
    AI_EMOTE_CAIJI_DISGUST,
    AI_EMOTE_CAIJI_FEAR,
    AI_EMOTE_CAIJI_SADNESS,
    AI_EMOTE_CAIJI_SURPRISE,
    AI_EMOTE_CAIJI_ANGER,
    AI_EMOTE_CAIJI_HAPPY,
    AI_EMOTE_XIAOJINGYU_NEUTRAL,
    AI_EMOTE_XIAOJINGYU_DISGUST,
    AI_EMOTE_XIAOJINGYU_FEAR,
    AI_EMOTE_XIAOJINGYU_SADNESS,
    AI_EMOTE_XIAOJINGYU_SURPRISE,
    AI_EMOTE_XIAOJINGYU_ANGER,
    AI_EMOTE_XIAOJINGYU_HAPPY,
    AI_EMOTE_NEZHA_NEUTRAL,
    AI_EMOTE_NEZHA_DISGUST,
    AI_EMOTE_NEZHA_FEAR,
    AI_EMOTE_NEZHA_SADNESS,
    AI_EMOTE_NEZHA_SURPRISE,
    AI_EMOTE_NEZHA_ANGER,
    AI_EMOTE_NEZHA_HAPPY,
    AI_EMOJIS_TOTAL,
}gt_ai_emojis_et;

void set_emojis_in_player(gt_obj_st * obj, gt_ai_emojis_et emojis);

typedef enum {
    AI_SETTING_NONE = 0,
    AI_SETTING_USER_NAME,
    AI_SETTING_AGE,
	AI_SETTING_ROLE,
	AI_SETTING_CHAR,
	AI_SETTING_TIMBRE,
	AI_SETTING_AI_NAME,
    AI_SETTING_MODEL,
    AI_SETTING_REPLY_STYLE,
    AI_SETTING_EMOTION_VALUE,
}gt_ai_setting_et;

typedef struct {
    char name[10];
    char character_desc[350];
    char voice_id[60];
    char *personality[4];
    char tone[250];
} gt_ai_bot_role_st;

typedef enum {
    GT_AI_BOT_ZHI_JIANG = 0,
    GT_AI_BOT_XIAO_ZHI,
    GT_AI_BOT_CAI_JI,
    GT_AI_BOT_XIAO_JING_YU,
    GT_AI_BOT_NE_ZHA,
    GT_AI_BOT_TOTAL,
} gt_ai_bot_role_st_type;

typedef struct {
    char role[20];
    char value[20];
    char url[100];
} gt_ai_bot_emote_st;

void set_emote_data_to_ram(gt_ai_emojis_et emojis);
void set_emote_in_img(gt_obj_st * obj, gt_ai_emojis_et emojis);
void set_role_emote(gt_obj_st * player, gt_obj_st * img, gt_ai_emojis_et zhijiang_emojis, gt_ai_emojis_et xiaozhi_emojis, gt_ai_emojis_et caiji_emojis, gt_ai_emojis_et xiaojingyu_emojis, gt_ai_emojis_et nezha_emojis);
void set_items_in_listview(gt_obj_st * listview, gt_ai_setting_et option);
char* gt_vocie_id_string_get(char *timbre);
char* gt_timber_string_get(char *voice_id);
char* gt_bot_description_string_get(char *ai_name);
gt_ai_bot_role_st gt_get_ai_bot_infos(char *ai_name);
char* gt_mode_en_string_get(char *mode);
char* gt_mode_cn_string_get(char *mode);
char* gt_reply_style_en_string_get(char *reply_style);
char* gt_reply_style_cn_string_get(char *reply_style);

void identification_failed_ui();
void wifi_connecting_ui();
void waiting_rec_ui();
void wifi_connected_fail_ui();

void recording_exe_func(void);
void send_information_exe_func(void);
void identifying_failed_ui_in_subtitle(void);

void redraw_wifi_list(WIFI_ITEM_INFO* scan_wifi_list, int ap_count);
void change_wifi_connect_tip(uint8_t flag);

#if USE_HTTP_STREAM
void update_subtitles(ReceivedAnswerData* receive_data);
#endif //!USE_HTTP_STREAM



void update_wifi_icon();

gt_obj_st * _Clear_page_dialog1_init();
void set_history_in_chat();
void clear_chat_history();
gt_obj_st * _Unstable_network_dialog1_init();
gt_obj_st* serve_disconnect_dialog();
gt_obj_st* wifi_scanning_dialog();
void close_wifi_scan_dialog();
void set_wifi_status_icon(gt_wifi_icon_status_et status);
#endif


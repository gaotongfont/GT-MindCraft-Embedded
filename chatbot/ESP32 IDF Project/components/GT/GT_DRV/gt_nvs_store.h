#ifndef GT_NVS_STORE_H
#define GT_NVS_STORE_H

#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

//general api
uint8_t store_data_in_nvs(const char *key, const char *value);
uint8_t read_data_from_nvs(const char *key, const char *out_value, size_t max_len);
uint8_t erase_data_from_nvs(const char *key);

#endif
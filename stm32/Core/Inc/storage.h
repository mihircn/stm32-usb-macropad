#ifndef INC_STORAGE_H_
#define INC_STORAGE_H_

#include <stdint.h>

#define MAX_BUTTONS 4

#define CONFIG_MAGIC   0xBEEFCAFE
#define CONFIG_VERSION 1

typedef enum {
	BIND_TYPE_KEYBOARD = 0,
	BIND_TYPE_CONSUMER
}BindType_t;

typedef struct
{
	BindType_t type;

    uint8_t modifier;

    uint8_t keys[6];

    uint16_t consumer_key;

} Keybind_t;

typedef struct
{
    uint32_t magic;

    uint16_t version;

    Keybind_t binds[MAX_BUTTONS];

} KeyboardConfig_t;

extern KeyboardConfig_t g_config;

void Storage_SaveConfig(void);

void Storage_LoadConfig(void);

void Storage_SetDefaults(void);

#endif

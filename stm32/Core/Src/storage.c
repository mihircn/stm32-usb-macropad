/*
 * storage.c
 *
 *  Created on: 22-May-2026
 *      Author: cnmih
 */

#include "storage.h"
#include "stm32f4xx_hal.h"
#include "class/hid/hid.h"
#include <string.h>

#define STORAGE_ADDRESS 0x08060000

KeyboardConfig_t g_config;


void Storage_SaveConfig(void)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase;

    uint32_t sector_error;

    erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    erase.Sector       = FLASH_SECTOR_7;
    erase.NbSectors    = 1;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if(HAL_FLASHEx_Erase(&erase, &sector_error) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return;
    }

    uint8_t* data = (uint8_t*)&g_config;

    for(uint32_t i = 0; i < sizeof(KeyboardConfig_t); i++)
    {
        if(HAL_FLASH_Program(
                FLASH_TYPEPROGRAM_BYTE,
                STORAGE_ADDRESS + i,
                data[i]
           ) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return;
        }
    }

    HAL_FLASH_Lock();
}

void Storage_LoadConfig(void)
{
    memcpy(
        &g_config,
        (void*)STORAGE_ADDRESS,
        sizeof(KeyboardConfig_t)
    );

    if(g_config.magic != CONFIG_MAGIC ||
       g_config.version != CONFIG_VERSION)
    {
        Storage_SetDefaults();

        Storage_SaveConfig();
    }
}

void Storage_SetDefaults(void)
{
    memset(&g_config, 0, sizeof(g_config));

    g_config.magic = CONFIG_MAGIC;

    g_config.version = CONFIG_VERSION;

     //Example default binds

    // Button 0 -> CTRL + C
    g_config.binds[0].modifier = KEYBOARD_MODIFIER_LEFTCTRL;
    g_config.binds[0].keys[0]  = HID_KEY_A;

//    // Button 1 -> CTRL + V
//    g_config.binds[1].modifier = KEYBOARD_MODIFIER_LEFTCTRL;
//    g_config.binds[1].keys[0]  = HID_KEY_V;
}

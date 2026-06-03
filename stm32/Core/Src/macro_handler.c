/*
 * macro_handler.c
 *
 *  Created on: 22-May-2026
 *      Author: cnmih
 */


#include "macro_handler.h"
#include "hid_handler.h"
#include "tusb.h"
#include "main.h"

void Macro_Execute(Macrostep_t* macro,
                   uint16_t step_count)
{
    for(uint16_t i = 0; i < step_count; i++)
    {
        HID_SendBind(&macro[i].bind);

        USB_Delay(macro[i].hold_ms);

        HID_Release(&macro[i].bind);

        USB_Delay(20);
    }
}
void USB_Delay(uint32_t ms)
{
	uint32_t start = HAL_GetTick();

	while((HAL_GetTick() - start) < ms)
	{
		tud_task();
	}
}

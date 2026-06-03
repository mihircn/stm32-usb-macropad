/*
 * cdc_handler.c
 *
 *  Created on: 22-May-2026
 *      Author: cnmih
 */


#include "cdc_handler.h"
#include "tusb.h"
#include "main.h"
#include <string.h>
#include <stdint.h>

static char cdc_buf[512];

static uint8_t cdc_available = 0;

void CDC_Update(void){

	if(cdc_available)return;

	if(!tud_cdc_available()){
		return;
	}

	uint32_t count = tud_cdc_read(cdc_buf, sizeof(cdc_buf) - 1);

	cdc_buf[count] = '\0';

	if(count > 0){
		cdc_available = 1;
	}
}

char* CDC_Read() {
	cdc_available=0;
	return cdc_buf;
}

void CDC_Write(char* str)
{
    uint32_t len = strlen(str);
    uint32_t sent = 0;

    while (sent < len)
    {
        uint32_t avail = tud_cdc_write_available();

        if (avail == 0)
        {
            tud_task();
            tud_cdc_write_flush();
            continue;
        }

        uint32_t chunk = len - sent;
        if (chunk > avail)
        {
            chunk = avail;
        }

        uint32_t written = tud_cdc_write(str + sent, chunk);
        sent += written;

        tud_cdc_write_flush();
        tud_task();
    }
}

uint8_t CDC_Available(){
	return cdc_available;
}

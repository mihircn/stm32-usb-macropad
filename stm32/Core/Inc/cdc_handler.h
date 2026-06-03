/*
 * cdc_handler.h
 *
 *  Created on: 22-May-2026
 *      Author: cnmih
 */

#ifndef INC_CDC_HANDLER_H_
#define INC_CDC_HANDLER_H_
#include <stdint.h>

void CDC_Update(void);

void CDC_Write(char* str);

uint8_t CDC_Available(void);

char* CDC_Read(void);

#endif /* INC_CDC_HANDLER_H_ */

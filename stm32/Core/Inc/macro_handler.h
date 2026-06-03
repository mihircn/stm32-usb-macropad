/*
 * macro_handler.h
 *
 *  Created on: 22-May-2026
 *      Author: cnmih
 */

#ifndef INC_MACRO_HANDLER_H_
#define INC_MACRO_HANDLER_H_


#include <stdint.h>
#include "storage.h"

typedef struct {

	Keybind_t bind;
	uint16_t hold_ms;
}Macrostep_t;

void Macro_Execute(Macrostep_t* macro, uint16_t step_count);
void USB_Delay(uint32_t ms);


#endif /* INC_MACRO_HANDLER_H_ */

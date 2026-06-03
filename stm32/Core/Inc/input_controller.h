/*
 * InputController.h
 *
 *  Created on: 21-May-2026
 *      Author: cnmih
 */

#ifndef INC_INPUT_CONTROLLER_H_
#define INC_INPUT_CONTROLLER_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define MAX_BUTTONS 4
#define DEBOUNCE_MS 50

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;

	uint8_t current_state;
	uint8_t previous_state;

	uint8_t pressed_event;
	uint8_t released_event;

	uint32_t last_change_time;
}Button_t;


void Button_Register(GPIO_TypeDef* port, uint16_t pin);
void Buttons_Init(void);
void Buttons_Update(void);

uint8_t Button_Pressed(uint8_t index);
uint8_t Button_Released(uint8_t index);

#endif /* INC_INPUT_CONTROLLER_H_ */

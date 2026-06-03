/*
 * input_controller.c
 *
 *  Created on: 21-May-2026
 *      Author: cnmih
 */

#include "input_controller.h"

static Button_t buttons[MAX_BUTTONS];

static uint8_t button_count = 0;

void Button_Register(GPIO_TypeDef* port, uint16_t pin){
	if(button_count >= MAX_BUTTONS)
		return;

	buttons[button_count].port = port;
	buttons[button_count].pin = pin;

	buttons[button_count].current_state = 1;
	buttons[button_count].previous_state = 1;

	buttons[button_count].pressed_event =0;
	buttons[button_count].released_event = 0;

	buttons[button_count].last_change_time = 0;

	button_count++;
}

void Buttons_Init(void){
	Button_Register(GPIOB, GPIO_PIN_1);
	Button_Register(GPIOB, GPIO_PIN_15);
	Button_Register(GPIOB, GPIO_PIN_14);
	Button_Register(GPIOB, GPIO_PIN_13);
}

void Buttons_Update(void){
	for(uint8_t i =0; i < button_count;i++){
		uint8_t raw_state = HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin);

		//state changed?
		if(raw_state != buttons[i].previous_state){
			buttons[i].last_change_time = HAL_GetTick();
		}
		//debounce passed?
		if(HAL_GetTick() - buttons[i].last_change_time > DEBOUNCE_MS){
			if(raw_state != buttons[i].current_state){
				buttons[i].current_state = raw_state;

				//detect press
				if(raw_state == GPIO_PIN_RESET){
					buttons[i].pressed_event = 1;
				}else{
					buttons[i].released_event = 1;
				}
			}
		}
		buttons[i].previous_state = raw_state;
	}
}

uint8_t Button_Pressed(uint8_t index){

	if(index >= button_count)
	    return 0;

	if(buttons[index].pressed_event){
		buttons[index].pressed_event = 0;

		return 1;
	}
	return 0;
}
uint8_t Button_Released(uint8_t index){
	if(index >= button_count)
	    return 0;
    if(buttons[index].released_event)
    {
        buttons[index].released_event = 0;

        return 1;
    }
    return 0;
}







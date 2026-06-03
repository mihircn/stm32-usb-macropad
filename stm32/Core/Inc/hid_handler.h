/*
 * hid_engine.h
 *
 *  Created on: 21-May-2026
 *      Author: cnmih
 */

#ifndef HID_ENGINE_H
#define HID_ENGINE_H

#include "storage.h"

void HID_SendBind(Keybind_t* bind);
void HID_Release(Keybind_t* bind);



#endif /* INC_HID_ENGINE_H_ */

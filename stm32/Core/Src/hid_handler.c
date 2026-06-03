#include <hid_handler.h>
#include "tusb.h"
#include "usb_descriptors.h"


void HID_SendBind(Keybind_t* bind)
{
    if(bind->type == BIND_TYPE_KEYBOARD)
    {
        tud_hid_keyboard_report(
            REPORT_ID_KEYBOARD,
            bind->modifier,
            bind->keys
        );
    }
    else if(bind->type == BIND_TYPE_CONSUMER)
    {
        tud_hid_report(
            REPORT_ID_CONSUMER_CONTROL,
            &bind->consumer_key,
            sizeof(bind->consumer_key)
        );
    }
}

void HID_Release(Keybind_t* bind)
{
    if(bind->type == BIND_TYPE_KEYBOARD)
    {
        tud_hid_keyboard_report(
            REPORT_ID_KEYBOARD,
            0,
            NULL
        );
    }
    else if(bind->type == BIND_TYPE_CONSUMER)
    {
        uint16_t empty = 0;

        tud_hid_report(
            REPORT_ID_CONSUMER_CONTROL,
            &empty,
            sizeof(empty)
        );
    }
}

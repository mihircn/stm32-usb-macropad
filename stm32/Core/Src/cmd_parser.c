/*
 * cmd_parser.c
 *
 *  Created on: 24-May-2026
 *      Author: cnmih
 */
#include "cmd_parser.h"
#include "storage.h"
#include "cdc_handler.h"

#include "class/hid/hid.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct
{
    const char* name;

    uint8_t modifier;

} ModifierMap_t;

const ModifierMap_t modifier_map[] =
{
    {"CTRL",   KEYBOARD_MODIFIER_LEFTCTRL},
    {"SHIFT",  KEYBOARD_MODIFIER_LEFTSHIFT},
    {"ALT",    KEYBOARD_MODIFIER_LEFTALT},
    {"WIN",    KEYBOARD_MODIFIER_LEFTGUI},

    {"RCTRL",  KEYBOARD_MODIFIER_RIGHTCTRL},
    {"RSHIFT", KEYBOARD_MODIFIER_RIGHTSHIFT},
    {"RALT",   KEYBOARD_MODIFIER_RIGHTALT},
    {"RWIN",   KEYBOARD_MODIFIER_RIGHTGUI},
};
typedef struct
{
    const char* name;

    uint8_t keycode;

} KeyMap_t;

const KeyMap_t key_map[] =
{
    // Letters
    {"A", HID_KEY_A},
    {"B", HID_KEY_B},
    {"C", HID_KEY_C},
    {"D", HID_KEY_D},
    {"E", HID_KEY_E},
    {"F", HID_KEY_F},
    {"G", HID_KEY_G},
    {"H", HID_KEY_H},
    {"I", HID_KEY_I},
    {"J", HID_KEY_J},
    {"K", HID_KEY_K},
    {"L", HID_KEY_L},
    {"M", HID_KEY_M},
    {"N", HID_KEY_N},
    {"O", HID_KEY_O},
    {"P", HID_KEY_P},
    {"Q", HID_KEY_Q},
    {"R", HID_KEY_R},
    {"S", HID_KEY_S},
    {"T", HID_KEY_T},
    {"U", HID_KEY_U},
    {"V", HID_KEY_V},
    {"W", HID_KEY_W},
    {"X", HID_KEY_X},
    {"Y", HID_KEY_Y},
    {"Z", HID_KEY_Z},

    // Numbers
    {"1", HID_KEY_1},
    {"2", HID_KEY_2},
    {"3", HID_KEY_3},
    {"4", HID_KEY_4},
    {"5", HID_KEY_5},
    {"6", HID_KEY_6},
    {"7", HID_KEY_7},
    {"8", HID_KEY_8},
    {"9", HID_KEY_9},
    {"0", HID_KEY_0},

    // Control keys
    {"ENTER", HID_KEY_ENTER},
    {"ESC", HID_KEY_ESCAPE},
    {"BACKSPACE", HID_KEY_BACKSPACE},
    {"TAB", HID_KEY_TAB},
    {"SPACE", HID_KEY_SPACE},
    {"DELETE", HID_KEY_DELETE},
    {"INSERT", HID_KEY_INSERT},
    {"HOME", HID_KEY_HOME},
    {"END", HID_KEY_END},
    {"PAGEUP", HID_KEY_PAGE_UP},
    {"PAGEDOWN", HID_KEY_PAGE_DOWN},

    // Arrow keys
    {"UP", HID_KEY_ARROW_UP},
    {"DOWN", HID_KEY_ARROW_DOWN},
    {"LEFT", HID_KEY_ARROW_LEFT},
    {"RIGHT", HID_KEY_ARROW_RIGHT},

    // Function keys
    {"F1", HID_KEY_F1},
    {"F2", HID_KEY_F2},
    {"F3", HID_KEY_F3},
    {"F4", HID_KEY_F4},
    {"F5", HID_KEY_F5},
    {"F6", HID_KEY_F6},
    {"F7", HID_KEY_F7},
    {"F8", HID_KEY_F8},
    {"F9", HID_KEY_F9},
    {"F10", HID_KEY_F10},
    {"F11", HID_KEY_F11},
    {"F12", HID_KEY_F12},

    // Symbols
    {"MINUS", HID_KEY_MINUS},
    {"EQUAL", HID_KEY_EQUAL},
    {"LBRACKET", HID_KEY_BRACKET_LEFT},
    {"RBRACKET", HID_KEY_BRACKET_RIGHT},
    {"BACKSLASH", HID_KEY_BACKSLASH},
    {"SEMICOLON", HID_KEY_SEMICOLON},
    {"APOSTROPHE", HID_KEY_APOSTROPHE},
    {"GRAVE", HID_KEY_GRAVE},
    {"COMMA", HID_KEY_COMMA},
    {"DOT", HID_KEY_PERIOD},
    {"SLASH", HID_KEY_SLASH},

    // Lock keys
    {"CAPSLOCK", HID_KEY_CAPS_LOCK},
    {"NUMLOCK", HID_KEY_NUM_LOCK},
    {"SCROLLLOCK", HID_KEY_SCROLL_LOCK},

    // Keypad
    {"KP0", HID_KEY_KEYPAD_0},
    {"KP1", HID_KEY_KEYPAD_1},
    {"KP2", HID_KEY_KEYPAD_2},
    {"KP3", HID_KEY_KEYPAD_3},
    {"KP4", HID_KEY_KEYPAD_4},
    {"KP5", HID_KEY_KEYPAD_5},
    {"KP6", HID_KEY_KEYPAD_6},
    {"KP7", HID_KEY_KEYPAD_7},
    {"KP8", HID_KEY_KEYPAD_8},
    {"KP9", HID_KEY_KEYPAD_9},

    {"KP_PLUS", HID_KEY_KEYPAD_ADD},
    {"KP_MINUS", HID_KEY_KEYPAD_SUBTRACT},
    {"KP_MULTIPLY", HID_KEY_KEYPAD_MULTIPLY},
    {"KP_DIVIDE", HID_KEY_KEYPAD_DIVIDE},
    {"KP_ENTER", HID_KEY_KEYPAD_ENTER},
    {"KP_DOT", HID_KEY_KEYPAD_DECIMAL},

    // Misc
    {"PRINTSCREEN", HID_KEY_PRINT_SCREEN},
    {"PAUSE", HID_KEY_PAUSE},
    {"MENU", HID_KEY_APPLICATION},

	{"VOLUP", HID_USAGE_CONSUMER_VOLUME_INCREMENT},
	{"VOLDOWN", HID_USAGE_CONSUMER_VOLUME_DECREMENT},
};

typedef struct
{
    const char* name;

    uint16_t usage;

} ConsumerMap_t;

const ConsumerMap_t consumer_map[] =
{
    {"VOLUP", HID_USAGE_CONSUMER_VOLUME_INCREMENT},
    {"VOLDOWN", HID_USAGE_CONSUMER_VOLUME_DECREMENT},
    {"MUTE", HID_USAGE_CONSUMER_MUTE},

    {"PLAY", HID_USAGE_CONSUMER_PLAY_PAUSE},
    {"NEXT", HID_USAGE_CONSUMER_SCAN_NEXT_TRACK},
    {"PREV", HID_USAGE_CONSUMER_SCAN_PREVIOUS_TRACK},

	{"BRIGHTUP", HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT},
	{"BRIGHTDOWN", HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT},
};

uint16_t ParseConsumer(char* token)
{
    for(uint32_t i = 0;
        i < sizeof(consumer_map)/sizeof(ConsumerMap_t);
        i++)
    {
        if(strcmp(token, consumer_map[i].name) == 0)
        {
            return consumer_map[i].usage;
        }
    }

    return 0;
}

uint8_t ParseModifier(char* token){

	for(uint32_t i = 0;i<sizeof(modifier_map)/sizeof(ModifierMap_t);i++){
		if(strcmp(token, modifier_map[i].name) == 0){
			return modifier_map[i].modifier;
		}
	}
	return 0;
}

const char* ModifierToString(uint8_t modifier){
	static char buffer[64];

	buffer[0] = '\0';

	for(uint32_t i=0;i<sizeof(modifier_map)/sizeof(ModifierMap_t);i++){
		if(modifier & modifier_map[i].modifier){
			strcat(buffer, modifier_map[i].name);
			strcat(buffer, " ");
		}
	}
	return buffer;
}


uint8_t ParseKey(char* token){
	for(uint32_t i = 0;i < sizeof(key_map)/sizeof(KeyMap_t);i++){
		if(strcmp(token, key_map[i].name) == 0){
			return key_map[i].keycode;
		}
	}
	return 0;
}

const char * KeyToString(uint8_t keycode) {
	for(uint32_t i = 0;i<sizeof(key_map)/sizeof(KeyMap_t);i++){
		if(keycode == key_map[i].keycode){
			return key_map[i].name;
		}
	}
	return "UNKNOWN";
}
const char* ConsumerToString(uint16_t usage)
{
    for(uint32_t i = 0;
        i < sizeof(consumer_map)/sizeof(ConsumerMap_t);
        i++)
    {
        if(usage == consumer_map[i].usage)
        {
            return consumer_map[i].name;
        }
    }

    return "UNKNOWN";
}

void GetBindString(uint8_t button, char* out)
{
    Keybind_t* bind =
        &g_config.binds[button];

    out[0] = '\0';

    sprintf(out, "KEY:%d:", button);

    if(bind->type == BIND_TYPE_KEYBOARD)
    {
        strcat(out,
            ModifierToString(bind->modifier));

        for(uint8_t i = 0; i < 6; i++)
        {
            uint8_t key =
                bind->keys[i];

            if(key == 0)
                continue;

            strcat(out,
                KeyToString(key));

            if(i < 5 &&
               bind->keys[i + 1] != 0)
            {
                strcat(out, "+");
            }
        }
    }
    else if(bind->type == BIND_TYPE_CONSUMER)
    {
        strcat(out,
            ConsumerToString(bind->consumer_key));
    }

    strcat(out, "\r\n");
}

void Command_Parser(char* line){
	char* tokens[16];
	uint8_t token_count = 0;
	char* token = strtok(line, " ");
	while(token!= NULL && token_count < 16){
		tokens[token_count ++] = token;
		token = strtok(NULL, " ");
	}
	if(token_count == 0){
		return;
	}

	if(strcmp(tokens[0], "SET") == 0){
		if(token_count < 3){
			CDC_Write("ERR\r\n");
			return;
		}

		uint8_t button = atoi(tokens[1]);

		if(button >= MAX_BUTTONS){
			CDC_Write("BAD BUTTON\r\n");
			return;
		}

		Keybind_t bind = {0};

		uint8_t key_index = 0;

		for(uint8_t i = 2; i < token_count; i++)
		{
		    uint8_t mod =
		        ParseModifier(tokens[i]);

		    if(mod)
		    {
		        bind.modifier |= mod;

		        continue;
		    }

		    uint16_t consumer =
		        ParseConsumer(tokens[i]);

		    if(consumer)
		    {
		        bind.type = BIND_TYPE_CONSUMER;

		        bind.consumer_key = consumer;

		        continue;
		    }

		    uint8_t key =
		        ParseKey(tokens[i]);

		    if(key)
		    {
		        if(key_index < 6)
		        {
		            bind.keys[key_index++] = key;
		        }

		        continue;
		    }

		    CDC_Write("UNKNOWN TOKEN\r\n");

		    return;
		}
		g_config.binds[button] = bind;

		CDC_Write("OK\r\n");
	}else if(strcmp(tokens[0], "GET") == 0){
		if(token_count < 2){
			CDC_Write("ERR\r\n");
			return;
		}

		uint8_t button = atoi(tokens[1]);

		if(button >= MAX_BUTTONS){
			CDC_Write("BAD BUTTON\r\n");
			return;
		}
		char buffer[128];

		buffer[0] = '\0';

		Keybind_t* bind =
		    &g_config.binds[button];

		if(bind->type == BIND_TYPE_KEYBOARD)
		{
		    sprintf(
		        buffer,
		        ModifierToString(bind->modifier)
		    );

		    for(uint8_t i = 0; i < 6; i++)
		    {
		        uint8_t key = bind->keys[i];

		        if(key == 0)
		            continue;

		        strcat(buffer, KeyToString(key));

		        strcat(buffer, " ");
		    }
		}
		else if(bind->type == BIND_TYPE_CONSUMER)
		{
		    sprintf(
		        buffer,
		        ConsumerToString(bind->consumer_key)
		    );
		}

		strcat(buffer, "\r\n");

		CDC_Write(buffer);
	}else if(strcmp(tokens[0], "SAVE\r\n") == 0){
		Storage_SaveConfig();
		CDC_Write("SAVED\r\n");
	}else if(strcmp(tokens[0], "PING") == 0){
		CDC_Write("STM32_MACROPAD_V1\r\n");
	}else if(strcmp(tokens[0], "GETALL\r\n") == 0)
	{
	    char buffer[256];

	    CDC_Write("CONFIG_START\r\n");

	    for(uint8_t i = 0;
	        i < MAX_BUTTONS;
	        i++)
	    {
	        GetBindString(i, buffer);

	        CDC_Write(buffer);
	    }

	    CDC_Write("CONFIG_END\r\n");
	}















}





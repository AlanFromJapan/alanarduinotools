/*
 * Keyboard.c
 *
 * Created: 2015/07/25 12:09:04
 *  Author: Alan
 */ 

#include "Keyboard.h"

/************************************************************************/
/* Make keyboard report to send to USB                                  */
/************************************************************************/
void buildKeyboardReport(uint8_t pKey) {
	//default: nothing
	keyboard_report.modifier = 0;
	keyboard_report.keycode[0] = 0;
	
	//if caps, set the modifier and shift the character to the lchars
	if(pKey >= 'A' && pKey <= 'Z'){
		keyboard_report.modifier = MOD_SHIFT_LEFT;
		pKey = pKey + ('a' - 'A');
	}
	
	if(pKey >= 'a' && pKey <= 'z'){
		keyboard_report.keycode[0] = 4+(pKey-'a');
		return;
	}

	//Digits
	if(pKey >= '1' && pKey <= '9'){
		keyboard_report.keycode[0] = 30+(pKey-'1');
		return;
	}
	else{
		if(pKey == '0'){
			keyboard_report.keycode[0] = KEY_0_PARC;
			return;
		}
	}

	//The rest
	switch (pKey){
		case ' ': keyboard_report.keycode[0] = KEY_SPACE; break;
		case '!': keyboard_report.keycode[0] = KEY_1_EXCL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '@': keyboard_report.keycode[0] = KEY_2_AT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '#': keyboard_report.keycode[0] = KEY_3_SHARP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '$': keyboard_report.keycode[0] = KEY_4_DOL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '%': keyboard_report.keycode[0] = KEY_5_PCNT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '^': keyboard_report.keycode[0] = KEY_6_HAT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '&': keyboard_report.keycode[0] = KEY_7_AMP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '*': keyboard_report.keycode[0] = KEY_8_STAR; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '(': keyboard_report.keycode[0] = KEY_9_PARO; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ')': keyboard_report.keycode[0] = KEY_0_PARC; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '-': keyboard_report.keycode[0] = KEY_DASH_UNDERSCORE; break;
		case '_': keyboard_report.keycode[0] = KEY_DASH_UNDERSCORE; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '=': keyboard_report.keycode[0] = KEY_EQUAL_PLUS; break;
		case '+': keyboard_report.keycode[0] = KEY_EQUAL_PLUS; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ';': keyboard_report.keycode[0] = KEY_SEMIC_COLON; break;
		case ':': keyboard_report.keycode[0] = KEY_SEMIC_COLON; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ',': keyboard_report.keycode[0] = KEY_COMA_LT; break;
		case '<': keyboard_report.keycode[0] = KEY_COMA_LT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '.': keyboard_report.keycode[0] = KEY_DOT_GT; break;
		case '>': keyboard_report.keycode[0] = KEY_DOT_GT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '/': keyboard_report.keycode[0] = KEY_SLASH_QUEST; break;
		case '?': keyboard_report.keycode[0] = KEY_SLASH_QUEST; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		
		case '\t': keyboard_report.keycode[0] = KEY_TAB; break;
		case '\n': keyboard_report.keycode[0] = KEY_ENTER; break;
	}
}

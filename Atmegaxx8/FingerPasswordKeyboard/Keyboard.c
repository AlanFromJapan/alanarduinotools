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
			keyboard_report.keycode[0] = KEY_0;
			return;
		}
	}

#ifdef KEYBOARD_MODE_US
	//The rest
	switch (pKey){
		case ' ' : keyboard_report.keycode[0] = KEY_SPACE; break;
		case '!' : keyboard_report.keycode[0] = KEY_EXCL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '@' : keyboard_report.keycode[0] = KEY_AT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '#' : keyboard_report.keycode[0] = KEY_SHARP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '$' : keyboard_report.keycode[0] = KEY_DOL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '%' : keyboard_report.keycode[0] = KEY_PCNT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '^' : keyboard_report.keycode[0] = KEY_HAT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '&' : keyboard_report.keycode[0] = KEY_AMP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '*' : keyboard_report.keycode[0] = KEY_STAR; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '(' : keyboard_report.keycode[0] = KEY_PARO; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ')' : keyboard_report.keycode[0] = KEY_PARC; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '-' : keyboard_report.keycode[0] = KEY_DASH; break;
		case '_' : keyboard_report.keycode[0] = KEY_UNDERSCORE; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '=' : keyboard_report.keycode[0] = KEY_EQUAL; break;
		case '+' : keyboard_report.keycode[0] = KEY_PLUS; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ';' : keyboard_report.keycode[0] = KEY_SEMIC; break;
		case ':' : keyboard_report.keycode[0] = KEY_COLON; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ',' : keyboard_report.keycode[0] = KEY_COMA; break;
		case '<' : keyboard_report.keycode[0] = KEY_LT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '.' : keyboard_report.keycode[0] = KEY_DOT; break;
		case '>' : keyboard_report.keycode[0] = KEY_GT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '/' : keyboard_report.keycode[0] = KEY_SLASH; break;
		case '?' : keyboard_report.keycode[0] = KEY_QUEST; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '\'': keyboard_report.keycode[0] = KEY_QUOTE; break;
		case '"' : keyboard_report.keycode[0] = KEY_DBLQUOTE; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		
		case '\t': keyboard_report.keycode[0] = KEY_TAB; break;
		case '\n': keyboard_report.keycode[0] = KEY_ENTER; break;
	}
#endif //KEYBOARD_MODE_US

#ifdef KEYBOARD_MODE_JP
	//The rest
	/************************************************************************/
	/* Details here : http://web.stanford.edu/class/cs140/projects/pintos/specs/kbd/scancodes-7.html */
	/************************************************************************/
	switch (pKey){
		case ' ' : keyboard_report.keycode[0] = KEY_SPACE; break;
		case '!' : keyboard_report.keycode[0] = KEY_EXCL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '@' : keyboard_report.keycode[0] = KEY_AT; break; /* DIFFERENT*/
		case '#' : keyboard_report.keycode[0] = KEY_SHARP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '$' : keyboard_report.keycode[0] = KEY_DOL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '%' : keyboard_report.keycode[0] = KEY_PCNT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '^' : keyboard_report.keycode[0] = KEY_HAT; break; /* DIFFERENT*/
		case '&' : keyboard_report.keycode[0] = KEY_AMP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '*' : keyboard_report.keycode[0] = KEY_STAR; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '(' : keyboard_report.keycode[0] = KEY_PARO; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ')' : keyboard_report.keycode[0] = KEY_PARC; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '-' : keyboard_report.keycode[0] = KEY_DASH; break;
		case '_' : keyboard_report.keycode[0] = KEY_UNDERSCORE; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '=' : keyboard_report.keycode[0] = KEY_EQUAL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '+' : keyboard_report.keycode[0] = KEY_PLUS; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ';' : keyboard_report.keycode[0] = KEY_SEMIC; break;
		case ':' : keyboard_report.keycode[0] = KEY_COLON; break; /* DIFFERENT*/
		case ',' : keyboard_report.keycode[0] = KEY_COMA; break;
		case '<' : keyboard_report.keycode[0] = KEY_LT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '.' : keyboard_report.keycode[0] = KEY_DOT; break;
		case '>' : keyboard_report.keycode[0] = KEY_GT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '/' : keyboard_report.keycode[0] = KEY_SLASH; break;
		case '?' : keyboard_report.keycode[0] = KEY_QUEST; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '\'': keyboard_report.keycode[0] = KEY_QUOTE; keyboard_report.modifier = MOD_SHIFT_LEFT; break;/* DIFFERENT*/
		case '"' : keyboard_report.keycode[0] = KEY_DBLQUOTE; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
	
		case '\t': keyboard_report.keycode[0] = KEY_TAB; break;
		case '\n': keyboard_report.keycode[0] = KEY_ENTER; break;
	}
#endif //KEYBOARD_MODE_JP


}

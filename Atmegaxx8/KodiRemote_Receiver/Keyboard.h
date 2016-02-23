/*
 * Keyboard.h
 *
 * Created: 2014/12/20 16:16:24
 *  Author: Alan
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#include <avr/io.h>

//Choose if you want the keys for Japanese keyboard or US (default) keyboard
//#define KEYBOARD_MODE_US
#define KEYBOARD_MODE_JP



typedef struct {
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode[6];
} keyboard_report_t;


keyboard_report_t keyboard_report; // sent to PC


/* The LED states */
#define LED_NUM     0x01
#define LED_CAPS    0x02
#define LED_SCROLL  0x04
#define LED_COMPOSE 0x08
#define LED_KANA    0x10


/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 * 
 * Direct link (Aug2015): http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
 */
#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)

#define KEY_A       4
#define KEY_B       5
#define KEY_C       6
#define KEY_D       7
#define KEY_E       8
#define KEY_F       9
#define KEY_G       10
#define KEY_H       11
#define KEY_I       12
#define KEY_J       13
#define KEY_K       14
#define KEY_L       15
#define KEY_M       16
#define KEY_N       17
#define KEY_O       18
#define KEY_P       19
#define KEY_Q       20
#define KEY_R       21
#define KEY_S       22
#define KEY_T       23
#define KEY_U       24
#define KEY_V       25
#define KEY_W       26
#define KEY_X       27
#define KEY_Y       28
#define KEY_Z       29
#define KEY_1		30
#define KEY_2		31
#define KEY_3		32
#define KEY_4		33
#define KEY_5		34
#define KEY_6		35
#define KEY_7		36
#define KEY_8		37
#define KEY_9		38
#define KEY_0		39

#define KEY_F1      58
#define KEY_F2      59
#define KEY_F3      60
#define KEY_F4      61
#define KEY_F5      62
#define KEY_F6      63
#define KEY_F7      64
#define KEY_F8      65
#define KEY_F9      66
#define KEY_F10     67
#define KEY_F11     68
#define KEY_F12     69

#define KEY_ENTER				40
#define KEY_TAB					43
#define KEY_SPACE				44

#ifdef KEYBOARD_MODE_US
	#define KEY_EXCL			30
	#define KEY_AT				31
	#define KEY_SHARP			32
	#define KEY_DOL				33
	#define KEY_PCNT			34
	#define KEY_HAT				35
	#define KEY_AMP				36
	#define KEY_STAR			37
	#define KEY_PARO			38
	#define KEY_PARC			39

	#define KEY_DASH			45
	#define KEY_EQUAL			46
	#define KEY_SEMIC			51
	#define KEY_COMA			54
	#define KEY_DOT				55
	#define KEY_SLASH			56
	#define KEY_UNDERSCORE		45
	#define KEY_PLUS			46
	#define KEY_COLON			51
	#define KEY_LT				54
	#define KEY_GT				55
	#define KEY_QUEST			56
	
	#define KEY_QUOTE			52
	#define KEY_DBLQUOTE		52
#endif //KEYBOARD_MODE_US

#ifdef KEYBOARD_MODE_JP
	/************************************************************************/
	/* Details here : http://web.stanford.edu/class/cs140/projects/pintos/specs/kbd/scancodes-7.html */
	/************************************************************************/
	#define KEY_EXCL			30
	#define KEY_AT				47
	#define KEY_SHARP			32
	#define KEY_DOL				33
	#define KEY_PCNT			34
	#define KEY_HAT				46
	#define KEY_AMP				35
	#define KEY_STAR			52
	#define KEY_PARO			37
	#define KEY_PARC			38

	#define KEY_DASH			45
	#define KEY_EQUAL			45
	#define KEY_SEMIC			51
	#define KEY_COMA			54
	#define KEY_DOT				55
	#define KEY_SLASH			56
	#define KEY_UNDERSCORE		135
	#define KEY_PLUS			51
	#define KEY_COLON			52
	#define KEY_LT				54
	#define KEY_GT				55
	#define KEY_QUEST			56
	
	#define KEY_QUOTE			36
	#define KEY_DBLQUOTE		31
#endif //KEYBOARD_MODE_JP

#endif /* KEYBOARD_H_ */
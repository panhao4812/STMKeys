/*
 * Keycode.h
 *
 * Created: 2020/9/14 13:18:01
 *  Author: panhao4812
 */
#include <stdint.h>
#include <string.h>
#ifndef KEYCODE_H_
#define KEYCODE_H_

#define KEY_CTRL	0x01
#define KEY_SHIFT	0x02
#define KEY_ALT		0x04
#define KEY_GUI		0x08
#define KEY_LEFT_CTRL	0x01
#define KEY_LEFT_SHIFT	0x02
#define KEY_LEFT_ALT	0x04
#define KEY_LEFT_GUI	0x08
#define KEY_RIGHT_CTRL	0x10
#define KEY_RIGHT_SHIFT	0x20
#define KEY_RIGHT_ALT	0x40
#define KEY_RIGHT_GUI	0x80

#define KEY_A		4
#define KEY_B		5
#define KEY_C		6
#define KEY_D		7
#define KEY_E		8
#define KEY_F		9
#define KEY_G		10
#define KEY_H		11
#define KEY_I		12
#define KEY_J		13
#define KEY_K		14
#define KEY_L		15
#define KEY_M		16
#define KEY_N		17
#define KEY_O		18
#define KEY_P		19
#define KEY_Q		20
#define KEY_R		21
#define KEY_S		22
#define KEY_T		23
#define KEY_U		24
#define KEY_V		25
#define KEY_W		26
#define KEY_X		27
#define KEY_Y		28
#define KEY_Z		29
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
#define KEY_ENTER	40
#define KEY_ESC		41
#define KEY_BACKSPACE	42
#define KEY_TAB		43
#define KEY_SPACE	44
#define KEY_MINUS	45
#define KEY_EQUAL	46
#define KEY_LEFT_BRACE	47
#define KEY_RIGHT_BRACE	48
#define KEY_BACKSLASH	49
#define KEY_NUMBER	50
#define KEY_SEMICOLON	51
#define KEY_QUOTE	52
#define KEY_TILDE	53
#define KEY_COMMA	54
#define KEY_PERIOD	55
#define KEY_SLASH	56
#define KEY_CAPS_LOCK	57
#define KEY_F1		58
#define KEY_F2		59
#define KEY_F3		60
#define KEY_F4		61
#define KEY_F5		62
#define KEY_F6		63
#define KEY_F7		64
#define KEY_F8		65
#define KEY_F9		66
#define KEY_F10		67
#define KEY_F11		68
#define KEY_F12		69
#define KEY_PRINTSCREEN	70
#define KEY_SCROLL_LOCK	71
#define KEY_PAUSE	72
#define KEY_INSERT	73
#define KEY_HOME	74
#define KEY_PAGE_UP	75
#define KEY_DELETE	76
#define KEY_END		77
#define KEY_PAGE_DOWN	78
#define KEY_RIGHT	79
#define KEY_LEFT	80
#define KEY_DOWN	81
#define KEY_UP		82
#define KEY_NUM_LOCK	83
#define KEYPAD_SLASH	84	// /
#define KEYPAD_ASTERIX	85	// *
#define KEYPAD_MINUS	86
#define KEYPAD_PLUS	87
#define KEYPAD_ENTER	88
#define KEYPAD_1	89
#define KEYPAD_2	90
#define KEYPAD_3	91
#define KEYPAD_4	92
#define KEYPAD_5	93
#define KEYPAD_6	94
#define KEYPAD_7	95
#define KEYPAD_8	96
#define KEYPAD_9	97
#define KEYPAD_0	98
#define KEYPAD_PERIOD	99		//.

#define MOUSE_LEFT	1<<0
#define MOUSE_RIGHT	1<<1
#define MOUSE_MID	1<<2
#define MOUSE_4	    1<<3
#define MOUSE_5	    1<<4

#define KEY_FN 0

/* Generic Desktop Page(0x01) - system power control */
#define SYSTEM_POWER_DOWN       0x81
#define SYSTEM_SLEEP            0x82
#define SYSTEM_WAKE_UP          0x83
////////////////////////////////////////////////////////
#define AUDIO_MUTE              0xE2
#define AUDIO_VOL_UP            0xE9
#define AUDIO_VOL_DOWN          0xEA
#define TRANSPORT_RECORD        0xB2
#define TRANSPORT_FAST_FORWARD  0xB3
#define TRANSPORT_REWIND        0xB4
#define TRANSPORT_NEXT_TRACK    0xB5
#define TRANSPORT_PREV_TRACK    0xB6
#define TRANSPORT_STOP          0xB7
#define TRANSPORT_EJECT         0xB8
#define TRANSPORT_STOP_EJECT    0xCC
#define TRANSPORT_PLAY_PAUSE    0xCD
//application launch
#define AL_CC_CONFIG           0x83// 0x0183
#define AL_EMAIL               0x8A// 0x018A
#define AL_CALCULATOR          0x92// 0x0192
#define AL_LOCAL_BROWSER       0x94// 0x0194
#define AL_LOCK                0x9E// 0x019E
//application control
#define AC_SEARCH              0x21// 0x0221
#define AC_HOME                0x23// 0x0223
#define AC_BACK                0x24// 0x0224
#define AC_FORWARD             0x25// 0x0225
#define AC_STOP                0x26// 0x0226
#define AC_REFRESH             0x27// 0x0227
#define AC_BOOKMARKS           0x2A// 0x022A
#define AC_MINIMIZE            0x06// 0x0206

//前4个macro是被预定的，后四个自定义
#define MACRO0 0x01//full led
#define MACRO1 0x02//rgb led
#define MACRO2 0x04//esc ~
#define MACRO3 0x08//print eep
#define MACRO4 0x10
#define MACRO5 0x20
#define MACRO6 0x40
#define MACRO7 0x80



#define Flash_PAGE_0 (uint32_t)0x08000000
#define Flash_PAGE_1 (uint32_t)0x08000400
#define Flash_PAGE_2 (uint32_t)0x08000800
#define Flash_PAGE_3 (uint32_t)0x08000C00
#define Flash_PAGE_4 (uint32_t)0x08001000
#define Flash_PAGE_5 (uint32_t)0x08001400
#define Flash_PAGE_6 (uint32_t)0x08001800
#define Flash_PAGE_7 (uint32_t)0x08001C00
#define Flash_PAGE_8 (uint32_t)0x08002000
#define Flash_PAGE_9 (uint32_t)0x08002400
#define Flash_PAGE_10 (uint32_t)0x08002800
#define Flash_PAGE_11 (uint32_t)0x08002C00
#define Flash_PAGE_12 (uint32_t)0x08003000
#define Flash_PAGE_13 (uint32_t)0x08003400
#define Flash_PAGE_14 (uint32_t)0x08003800
#define Flash_PAGE_15 (uint32_t)0x08003C00
#define Flash_PAGE_16 (uint32_t)0x08004000
#define Flash_PAGE_17 (uint32_t)0x08004400
#define Flash_PAGE_18 (uint32_t)0x08004800
#define Flash_PAGE_19 (uint32_t)0x08004C00
#define Flash_PAGE_20 (uint32_t)0x08005000
#define Flash_PAGE_21 (uint32_t)0x08005400
#define Flash_PAGE_22 (uint32_t)0x08005800
#define Flash_PAGE_23 (uint32_t)0x08005C00
#define Flash_PAGE_24 (uint32_t)0x08006000
#define Flash_PAGE_25 (uint32_t)0x08006400
#define Flash_PAGE_26 (uint32_t)0x08006800
#define Flash_PAGE_27 (uint32_t)0x08006C00
#define Flash_PAGE_28 (uint32_t)0x08007000
#define Flash_PAGE_29 (uint32_t)0x08007400
#define Flash_PAGE_30 (uint32_t)0x08007800
#define Flash_PAGE_31 (uint32_t)0x08007C00
#define Flash_PAGE_32 (uint32_t)0x08008000
#define Flash_PAGE_33 (uint32_t)0x08008400
#define Flash_PAGE_34 (uint32_t)0x08008800
#define Flash_PAGE_35 (uint32_t)0x08008C00
#define Flash_PAGE_36 (uint32_t)0x08009000
#define Flash_PAGE_37 (uint32_t)0x08009400
#define Flash_PAGE_38 (uint32_t)0x08009800
#define Flash_PAGE_39 (uint32_t)0x08009C00
#define Flash_PAGE_40 (uint32_t)0x0800A000
#define Flash_PAGE_41 (uint32_t)0x0800A400
#define Flash_PAGE_42 (uint32_t)0x0800A800
#define Flash_PAGE_43 (uint32_t)0x0800AC00
#define Flash_PAGE_44 (uint32_t)0x0800B000
#define Flash_PAGE_45 (uint32_t)0x0800B400
#define Flash_PAGE_46 (uint32_t)0x0800B800
#define Flash_PAGE_47 (uint32_t)0x0800BC00
#define Flash_PAGE_48 (uint32_t)0x0800C000
#define Flash_PAGE_49 (uint32_t)0x0800C400
#define Flash_PAGE_50 (uint32_t)0x0800C800
#define Flash_PAGE_51 (uint32_t)0x0800CC00
#define Flash_PAGE_52 (uint32_t)0x0800D000
#define Flash_PAGE_53 (uint32_t)0x0800D400
#define Flash_PAGE_54 (uint32_t)0x0800D800
#define Flash_PAGE_55 (uint32_t)0x0800DC00
#define Flash_PAGE_56 (uint32_t)0x0800E000
#define Flash_PAGE_57 (uint32_t)0x0800E400
#define Flash_PAGE_58 (uint32_t)0x0800E800
#define Flash_PAGE_59 (uint32_t)0x0800EC00
#define Flash_PAGE_60 (uint32_t)0x0800F000
#define Flash_PAGE_61 (uint32_t)0x0800F400
#define Flash_PAGE_62 (uint32_t)0x0800F800
#define Flash_PAGE_63 (uint32_t)0x0800FC00

#endif /* KEYCODE_H_ */

/*
页0 0x0800 0000 - 0x0800 03FF 1K
页1 0x0800 0400 - 0x0800 07FF 1K
页2 0x0800 0800 - 0x0800 0BFF 1K
页3 0x0800 0C00 - 0x0800 0FFF 1K
页4 0x0800 1000 - 0x0800 13FF 1K
页5 0x0800 1400 - 0x0800 17FF 1K
页6 0x0800 1800 - 0x0800 1BFF 1K
页7 0x0800 1C00 - 0x0800 1FFF 1K
页8 0x0800 2000 - 0x0800 23FF 1K
页9 0x0800 2400 - 0x0800 27FF 1K
页10 0x0800 2800 - 0x0800 2BFF 1K
页11 0x0800 2C00 - 0x0800 2FFF 1K
页12 0x0800 3000 - 0x0800 33FF 1K
页13 0x0800 3400 - 0x0800 37FF 1K
页14 0x0800 3800 - 0x0800 3BFF 1K
页15 0x0800 3C00 - 0x0800 3FFF 1K
页16 0x0800 4000 - 0x0800 43FF 1K
页17 0x0800 4400 - 0x0800 47FF 1K
页18 0x0800 4800 - 0x0800 4BFF 1K
页19 0x0800 4C00 - 0x0800 4FFF 1K
页20 0x0800 5000 - 0x0800 53FF 1K
页21 0x0800 5400 - 0x0800 57FF 1K
页22 0x0800 5800 - 0x0800 5BFF 1K
页23 0x0800 5C00 - 0x0800 5FFF 1K
页24 0x0800 6000 - 0x0800 63FF 1K
页25 0x0800 6400 - 0x0800 67FF 1K
页26 0x0800 6800 - 0x0800 6BFF 1K
页27 0x0800 6C00 - 0x0800 6FFF 1K
页28 0x0800 7000 - 0x0800 73FF 1K
页29 0x0800 7400 - 0x0800 77FF 1K
页30 0x0800 7800 - 0x0800 7BFF 1K
页31 0x0800 7C00 - 0x0800 7FFF 1K
页32 0x0800 8000 - 0x0800 83FF 1K
页33 0x0800 8400 - 0x0800 87FF 1K
页34 0x0800 8800 - 0x0800 8BFF 1K
页35 0x0800 8C00 - 0x0800 8FFF 1K
页36 0x0800 9000 - 0x0800 93FF 1K
页37 0x0800 9400 - 0x0800 97FF 1K
页38 0x0800 9800 - 0x0800 9BFF 1K
页39 0x0800 9C00 - 0x0800 9FFF 1K
页40 0x0800 A000 - 0x0800 A3FF 1K
页41 0x0800 A400 - 0x0800 A7FF 1K
页42 0x0800 A800 - 0x0800 ABFF 1K
页43 0x0800 AC00 - 0x0800 AFFF 1K
页44 0x0800 B000 - 0x0800 B3FF 1K
页45 0x0800 B400 - 0x0800 B7FF 1K
页46 0x0800 B800 - 0x0800 BBFF 1K
页47 0x0800 BC00 - 0x0800 BFFF　1K
页48 0x0800 C000 - 0x0800 C3FF 1K
页49 0x0800 C400 - 0x0800 C7FF 1K
页50 0x0800 C800 - 0x0800 CBFF 1K
页51 0x0800 CC00 - 0x0800 CFFF 1K
页52　0x0800 D000 - 0x0800 D3FF 1K
页53 0x0800 D400 - 0x0800 D7FF 1K
页54 0x0800 D800 - 0x0800 DBFF 1K
页55 0x0800 DC00 - 0x0800 DFFF 1K
页56 0x0800 E000 - 0x0800 E3FF 1K
页57 0x0800 E400 - 0x0800 E7FF 1K
页58 0x0800 E800 - 0x0800 EBFF 1K
页59　0x0800 EC00 - 0x0800 EFFF 1K
页60 0x0800 F000 - 0x0800 F3FF 1K
页61 0x0800 F400 - 0x0800 F7FF 1K
页62 0x0800 F800 - 0x0800 FBFF 1K
页63 0x0800 FC00 - 0x0800 FFFF 1K
*/

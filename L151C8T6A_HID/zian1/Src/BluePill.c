#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"
#include "Functions.h"

///////////////////////PCB board////////////////////
uint8_t RGB_TYPE;
uint8_t RGB_STATE;
uint16_t delay_val;
uint8_t FN; //FN_key
uint8_t delay_after; //backswing 后摇
uint8_t delay_before; //windup 前摇
uint8_t color_r,color_g,color_b;
#if(WS2812_COUNT==64)
uint16_t rgb_rainbow[WS2812_COUNT]={
	14,31,47,64,81,98,115,132,149,166,183,200,217,234,
	14,39,56,73,90,107,124,141,158,174,191,208,225,242,
	14,43,60,77,94,111,128,145,162,179,196,213,230,
	14,47,64,81,98,115,132,149,166,183,200,217,234,251,
	14,35,56,77,183,200,217,234,251
};
uint8_t rgb_fixcolor[(WS2812_COUNT*3)]={
	237,32,50,222,13,91,198,0,118,103,0,120,77,29,136,41,70,160,19,134,196,2,191,219,17,179,98,49,182,48,90,190,10,212,234,31,234,197,26,247,151,19,
	237,32,50,211,1,116,138,0,119,96,8,125,54,55,151,36,83,167,3,187,226,10,184,153,24,179,82,74,185,14,147,211,20,222,232,31,245,165,21,245,110,10,
	237,32,50,210,0,118,114,0,120,87,18,130,46,64,157,28,105,180,1,192,229,14,181,121,35,180,67,79,186,8,188,226,27,227,215,29,247,159,20,
	237,32,50,198,0,118,103,0,120,77,29,136,41,70,160,19,134,196,2,191,219,17,179,98,49,182,48,90,190,10,212,234,31,234,197,26,247,151,19,243,63,1,
	237,32,50,216,6,106,138,0,119,87,18,130,90,190,10,212,234,31,234,197,26,247,151,19,243,63,1
};
uint8_t rgb_pos[WS2812_COUNT]={
63,62,61,60,59,58,57,56,55,54,53,52,51,50,
49,48,47,46,45,44,43,42,41,40,39,38,37,36,
35,34,33,32,31,30,29,28,27,26,25,24,23,
22,21,20,19,18,17,16,15,14,13,12,11,10,9,
8,7,6,5,4,3,2,1,0
};
#endif
#if(WS2812_COUNT==8)
uint16_t rgb_rainbow[WS2812_COUNT] = { 14, 31, 47, 64, 81, 98, 115, 132 };
uint8_t rgb_fixcolor[(WS2812_COUNT * 3)] = { 237, 32, 50, 222, 13, 91, 198, 0,
		118, 103, 0, 120, 77, 29, 136, 41, 70, 160, 19, 134, 196, 2, 191, 219 };
uint8_t rgb_pos[WS2812_COUNT] = { 7, 6, 5, 4, 3, 2, 1, 0 };
#endif
//////////////////////////////////////////////
void LED_Timer() {
	if (delay_val== MAX_DELAY_7) {
		ws2812Send();
	}
	if (delay_val>0) {
		delay_val--;
	} else {
		delay_val = MAX_DELAY_8;
	}
}
void Fix_LED(uint16_t delay_val_blink) {
	if (delay_val_blink == MAX_DELAY_5) {
		for (uint8_t i = 0; i < WS2812_COUNT; i++) {
			ws2812SetRGB(rgb_pos[i], rgb_fixcolor[i * 3],
					rgb_fixcolor[i * 3 + 1], rgb_fixcolor[i * 3 + 2]); //default
		}
	}
}
void Rainbow_LED(uint16_t delay_val_blink) {
	if (delay_val_blink == MAX_DELAY_3) {
		for (uint8_t i = 0; i < WS2812_COUNT; i++) {
			rgb_rainbow[i]++;
			if (rgb_rainbow[i] >= WS2812_COLOR_COUNT) {
				rgb_rainbow[i] = 0;
			}
			color_r = Rcolors[rgb_rainbow[i]];
			color_g = Gcolors[rgb_rainbow[i]];
			color_b = Bcolors[rgb_rainbow[i]];
			ws2812SetRGB(rgb_pos[i], color_r, color_g, color_b);
		}
	}
}
void resetLED() {
	RGB_STATE = RGB_TYPE; //默认开关状态
	ws2812Clear();
	ws2812Send();
	//
	/*
	//灯自检程序
	for (uint8_t kk = 0; kk < ROWS; kk++) {
		for (uint8_t jj = 0; jj < COLS; jj++) {
			if ((key_mask[ROWS - kk - 1][jj] & (~0x88)) == 0)
				continue;
			led_mask[ROWS - kk - 1][jj] = 0xFF;
			for (uint16_t ii = 0; ii <= (0x40 + jj + kk); ii++) {
				//MAX_DELAY_8==0x80
				//ii jj kk 需要单独定义 避免和函数内部的i j k r c混用导致冲突
				_delay_us(300);
				blink_LED(delay_val);
				LED_Timer(&delay_val);
			}
		}
	}
	////等所有灯光效果结束
	for (uint16_t ii = 0; ii <= (0xF0); ii++) {
		_delay_us(300);
		blink_LED(delay_val);
		LED_Timer(&delay_val);
	}

	ws2812Clear();
	ws2812Send2();
	//*/
	delay_val = MAX_DELAY_8;
}
void init_LED() {
	ws2812Setup();
	ws2812Clear();
	ws2812Send();
	delay_val = MAX_DELAY_8;
}
void update_LED() {
	if (keyboard_buffer.keyboard_leds & 0x02U) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
	if (RGB_STATE & (1 << 4)) {
		if ((RGB_STATE & 0x0F) == 0x01) {
			Rainbow_LED(delay_val);
		}
		else if ((RGB_STATE & 0x0F) == 0x00) {
			Fix_LED(delay_val);
		}
	} else {
		//closed/////////////////////////////////////////////
		ws2812Clear();
	}
	LED_Timer();
}
void open_LED() {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}
void close_LED() {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}
void qmkMode() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	releaseAllKeyboardKeys();
	releaseAllMousekeys();
	macro_buffer=0;
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET) {
		delay_after = DELAY_AFTER;
		pressMacroKey(MACRO4);
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET) {
		delay_after = DELAY_AFTER;
		pressKey(KEY_A);
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_RESET) {
		delay_after = DELAY_AFTER;
		pressMouseKey(MOUSE_MID);
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET) {
		delay_after = DELAY_AFTER;
		pressConsumerKey(AUDIO_MUTE);
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET) {
		delay_after = DELAY_AFTER;
		pressMacroKey(MACRO1);
	}
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET) {
		delay_after = DELAY_AFTER;
		pressMacroKey(MACRO5);
	}
	if (!isBufferClear()) {
		FN = 0xF0;
	} //Fix FN key state error
	if (usbMacroSendRequired()) {
		delay_before = DELAY_BEFORE;
	}
	if (usbKeyboardSendRequired()) {
		delay_before = DELAY_BEFORE;
	}
	if (usbMouseSendRequired()) {
		delay_before = DELAY_BEFORE;
	}
	if (delay_after == DELAY_AFTER && delay_before == 1) {
		usbMacroSend();
		usbKeyboardSend();
		usbMouseSend();
	}
	if (delay_after == 1) {
		usbMacroSend();
		usbKeyboardSend();
		usbMouseSend();
	}
	if (delay_after > 0) {
		delay_after -= 1;
	}
	if (delay_before > 0) {
		delay_before -= 1;
	}
}

int init_Main(void) {
	init_LED();		//插电亮灯会掉电，导致hub掉电不识别。所以要提前关灯。
	HAL_Delay(300);
	usbInit();
	HAL_Delay(500);//最低500ms启动

	/////////////////////////////////////////////
	enableReset = 1;
	keyboard_buffer.enable_pressing = 1;
	RGB_TYPE = 0x01;	///set default on & rainbow
	releaseAllKeyboardKeys();
	releaseAllMousekeys();
	//resetMatrixFormEEP();
	resetLED();
	FN = 0xF0;
	HAL_Delay(300);
	usbSend(KEYBOARD_ENDPOINT, (uint8_t*) &keyboard_report, 8, 50);

	while (1) {
		if(delay_before==0&&delay_after==0){flashWrite();}//eepWrite();}
		if (keyboard_buffer.enable_pressing == 2) {
			break;
		} else if (keyboard_buffer.enable_pressing == 1) {
			qmkMode();
			if(delay_before==0) {update_LED();}
		}
	}
	return 0;
}


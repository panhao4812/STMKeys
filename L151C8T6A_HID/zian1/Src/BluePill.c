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
uint16_t rgb_rainbow[WS2812_COUNT] = { 14, 31, 47, 64, 81, 98, 115, 132 };
uint8_t rgb_fixcolor[(WS2812_COUNT * 3)] = { 237, 32, 50, 222, 13, 91, 198, 0,
		118, 103, 0, 120, 77, 29, 136, 41, 70, 160, 19, 134, 196, 2, 191, 219 };
uint8_t rgb_pos[WS2812_COUNT] = { 7, 6, 5, 4, 3, 2, 1, 0 };
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
	for (uint8_t i = 0; i < WS2812_COUNT; i++) {
		if (rgb_rainbow[i] >= WS2812_COLOR_COUNT)
			rgb_rainbow[i] = 0;
		if (delay_val_blink == MAX_DELAY_4) {
			color_r = Rcolors[rgb_rainbow[i]];
			ws2812SetR(rgb_pos[i], color_r);
		}
		if (delay_val_blink == MAX_DELAY_3) {
			color_g = Gcolors[rgb_rainbow[i]];
			ws2812SetG(rgb_pos[i], color_g);
		}
		if (delay_val_blink == MAX_DELAY_2) {
			color_b = Bcolors[rgb_rainbow[i]];
			ws2812SetB(rgb_pos[i], color_b);
		}
		if (delay_val_blink == MAX_DELAY_1) {
			rgb_rainbow[i]++;
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
	HAL_Delay(500);
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
		if(delay_before==0&&delay_after==0){flashWrite();}
		if (keyboard_buffer.enable_pressing == 2) {
			break;
		} else if (keyboard_buffer.enable_pressing == 1) {
			qmkMode();
			if(delay_before==0) {update_LED();}
		}
	}
	return 0;
}


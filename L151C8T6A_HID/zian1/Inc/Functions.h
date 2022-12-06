
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "Keycode.h"
#include "stm32l1xx_hal.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"
#include "usbd_def.h"
extern USBD_HandleTypeDef hUsbDevice;//oop指针必须是static
//////////////////////usb desc///////////////////////
#define VENDOR_ID     0xF103
#define USBD_LANGID_STRING     1033
#define USBD_MANUFACTURER_STRING     "zian1"
#define PRODUCT_ID     0xC8F6
#define USBD_PRODUCT_STRING_FS     "zian1_keyboard_STM32"
#define USBD_CONFIGURATION_STRING_FS     "HID Config"
#define USBD_INTERFACE_STRING_FS     "HID Interface"
/////////////////////report desc///////////////////////////
#define LSB(n) (n & 0xFF)
#define MSB(n) ((n >> 8) & 0xFF)
#define KEYBOARD_HID_DESC_OFFSET (9+9)
#define MOUSE_HID_DESC_OFFSET    (9+9+9+7+9)
#define RAW_HID_DESC_OFFSET      (9+9+9+7+9+9+7+9)
#define CONFIG1_DESC_SIZE        (9+9+9+7+9+9+7+9+9+7+7)
///////////////endpoint//////////////
#define REPORT_ID_MOUSE     1U
#define REPORT_ID_SYSTEM    2U
#define REPORT_ID_CONSUMER  3U
#define ENDPOINT0_SIZE		8U
#define KEYBOARD_INTERFACE	0U
#define KEYBOARD_ENDPOINT	1U
#define KEYBOARD_SIZE		8U
#define MOUSE_INTERFACE KEYBOARD_INTERFACE+1U
#define MOUSE_ENDPOINT	KEYBOARD_ENDPOINT+1U
#define MOUSE_SIZE		8U
#define RAW_INTERFACE	 MOUSE_INTERFACE+1U
#define RAW_ENDPOINT_IN	 MOUSE_ENDPOINT+1U
#define RAW_ENDPOINT_OUT MOUSE_ENDPOINT+2U
#define RAW_EPSIZE  8U
//////////////////////eeprom//////////////////////////
#define ADD_INDEX (uint16_t)10
#define ADD_ROW (uint16_t)(ADD_INDEX+ROWS) //15
#define ADD_COL (uint16_t)(ADD_ROW+COLS) //29
#define ADD_KEYS1 (uint16_t)(ADD_COL+(ROWS*COLS)) //
#define ADD_KEYS2 (uint16_t)(ADD_KEYS1+(ROWS*COLS)) //
#define ADD_RGB (uint16_t)(ADD_KEYS2+(ROWS*COLS)) //
#define ADD_RGBTYPE (uint16_t)(ADD_RGB+(WS2812_COUNT*3))//
#define ADD_EEP (uint16_t)(ADD_RGBTYPE+6) //
/////////////////PCB board////////////////
#define BluePill
#if defined(BluePill)
#define ROWS  5
#define COLS  14
#define WS2812_COUNT 8
#define RGB_EFFECT_COUNT 2
#define START_FLASH (uint32_t)(Flash_PAGE_62)
#define ADD_FLASH (uint32_t)(START_FLASH+ADD_EEP)
#define MAX_FLASH (uint32_t)(START_FLASH+1024)
#define MAX_DELAY (uint32_t)(0x0010*6)
#endif
//////////////switch delay////////////////
//rgb_state和rgb_type定义相同,一个是实时，一个是默认状态。
//bit7->第1组 0 off, 1 on
//bit6->第2组 0 off, 1 on
//bit5->第full组 0 off, 1 on
//bit4->第RGB组 0 off, 1 on
//bit0-3->第1组 0 fix rgb_fixcolor[]，1 Rainbow rgb_rainbow[]，print
#define DELAY_AFTER (uint8_t)(0x08*6)
#define DELAY_BEFORE (uint8_t)(0x04*6)
#define MAX_DELAY_1 MAX_DELAY*1
#define MAX_DELAY_2 MAX_DELAY*2
#define MAX_DELAY_3 MAX_DELAY*3
#define MAX_DELAY_4 MAX_DELAY*4
#define MAX_DELAY_5 MAX_DELAY*5
#define MAX_DELAY_6 MAX_DELAY*6
#define MAX_DELAY_7 MAX_DELAY*7
#define MAX_DELAY_8 MAX_DELAY*8
////////////////////struct////////////////////////
typedef struct {
	uint8_t report_id;
	uint8_t buttons;
	int8_t x;
	int8_t y;
	int8_t v;
	int8_t h;
} __attribute__ ((packed)) report_mouse0_t;
typedef struct {
	uint8_t  report_id;
	uint16_t usage;
} __attribute__ ((packed)) report_extra_t;
typedef struct {
	report_mouse0_t mouse;
	report_extra_t system_keys;
	report_extra_t consumer_keys;
} __attribute__ ((packed)) report_mouse_t;
typedef struct {
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode[6];
}__attribute__ ((packed))  report_keyboard_t;
typedef  union  {
	uint16_t      word[RAW_EPSIZE/2];
	uint8_t       bytes[RAW_EPSIZE];
}__attribute__ ((packed))report_raw_t;
typedef union {
	uint16_t    word;
	uint8_t     bytes[2];
}__attribute__ ((packed))usbWord_t;
typedef struct {
	uint8_t mouse_keys;
	int8_t x;
	int8_t y;
	int8_t v;
	int8_t h;
	uint16_t system_keys;
	uint16_t consumer_keys;
	// protocol setting from the host.  We use exactly the same report
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use.
	uint8_t mouse_protocol;
	uint8_t Send_Required;
} __attribute__ ((packed)) buffer_mouse_t;
typedef struct {
	uint8_t keyboard_modifier_keys;
	uint8_t keyboard_keys[6];
	uint8_t keyboard_leds;
	uint8_t enable_pressing;
	// the idle configuration, how often we send the report to the
	// host (ms * 4) even when it hasn't changed
	uint8_t keyboard_idle_config;
	uint8_t keyboard_idle_count;
	// protocol setting from the host.  We use exactly the same report
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use
	uint8_t keyboard_protocol;
	uint8_t Send_Required;
}__attribute__ ((packed))  buffer_keyboard_t;
/////////////////Back//////////////
extern uint8_t macro_report;
extern uint8_t macro_buffer;
extern report_mouse_t mouse_report;
extern report_mouse_t print_mouse_report;
extern buffer_mouse_t mouse_buffer;
extern report_raw_t raw_report_in;
extern report_raw_t raw_report_out;
extern report_keyboard_t keyboard_report;
extern report_keyboard_t print_keyboard_report;
extern buffer_keyboard_t keyboard_buffer;
extern uint8_t enableReset;//eep change
//////////////////main//////////
extern uint16_t rgb_rainbow[WS2812_COUNT];
extern uint8_t rgb_pos[WS2812_COUNT];
extern uint8_t led_mask[ROWS][COLS];
extern uint8_t rgb_fixcolor[(WS2812_COUNT * 3)];
extern  uint8_t hexa_keys0[ROWS][COLS] ;
extern  uint8_t hexa_keys1[ROWS][COLS];
extern  uint8_t key_mask[ROWS][COLS];
extern  uint8_t row_pins[ROWS];
extern  uint8_t col_pins[COLS];
extern uint8_t RGB_TYPE;
extern uint8_t RGB_STATE;
extern uint16_t delay_val;
extern uint8_t FN;//FN_key
extern uint8_t delay_after;//backswing 后摇
extern uint8_t delay_before;//windup 前摇
extern uint8_t color_r,color_g,color_b;
////////////////////main_functions//////////
int init_Main(void);
void init_Cols(void);
void init_Rows(void);
void qmkMode(void);
void init_LED(void);
void reset_LED(void);
void open_LED(void);
void close_LED(void);
void update_LED(void);
void timer_LED(uint16_t* delay_val_blink);
void blink_LED(uint16_t delay_val_blink);
void rainbow_LED(uint16_t delay_val_blink);
void fix_LED(uint16_t delay_val_blink);
void keyPrintWord_EEP(uint16_t address);
void keyPrintWord_Flash(uint32_t address_t);
void resetMatrix_Flash();
void resetMatrix_EEP();
////////////////////////macro////////////////////////
void clearMacro();
void pressMacroKey(uint8_t key);
uint8_t usbMacroSendRequired();
uint8_t usbMacroSend();
//////////////////////////usb key/////////////////////////
void usbInit();			// initialize everything
uint8_t usbConfigured();		// is the USB port configured
void releaseAllKeyboardKeys();
uint8_t releaseKey(uint8_t key);
uint8_t pressKey(uint8_t key);
void pressModifierKeys(uint8_t key);
void releaseModifierKeys(uint8_t key);
void clearKeyboard();
void clearMouse();
void clearRaw();
void releaseAllMousekeys();
void releaseMouseKey(uint8_t key);
void pressMouseKey(uint8_t key);
void releaseSystemKey(uint8_t key);
void pressSystemKey(uint8_t key);
void releaseConsumerKey(uint8_t key);
void pressConsumerKey(uint8_t key);
uint8_t usbKeyboardSendRequired();		// initialize everything
uint8_t usbKeyboardSend();
uint8_t usbMouseSendRequired();
uint8_t usbMouseSend();
uint8_t usbSend(uint8_t endpoint,uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
uint8_t usbRecv(uint8_t endpoint,uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
uint8_t isBufferClear();
//////////flash_functions///////////
void flashWrite();
void eepWrite();
uint16_t flashReadHalfWord(uint32_t address);
////////////////Color////////////////////
#define WS2812_COLOR_COUNT 0x0200
extern uint8_t Rcolors [WS2812_COLOR_COUNT];
extern uint8_t Gcolors[WS2812_COLOR_COUNT];
extern uint8_t Bcolors[WS2812_COLOR_COUNT];
//////////////////ws2812///////////////////////

/*Power saver, divide light level with this.*/
void ws2812Setup(void);
void ws2812Clear(void);
void ws2812SetRGB(uint16_t led, uint8_t red, uint8_t green, uint8_t blue);
void ws2812Send(void);
void ws2812SetR(uint16_t led, uint8_t red);
void ws2812SetG(uint16_t led, uint8_t green);
void ws2812SetB(uint16_t led, uint8_t blue);

#endif /* INC_FUNCTIONS_H_ */

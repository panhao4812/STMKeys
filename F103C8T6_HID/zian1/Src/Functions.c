#include "Functions.h"
#include "stm32f1xx_hal.h"

static uint8_t HexToKey[16] = {
KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,
KEY_8,KEY_9,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F};
////////////////////keyprint////////////////////
void keyPrint0x(){
		print_keyboard_report.modifier = 0;
		print_keyboard_report.keycode[0] = KEY_0;
		usbSend(KEYBOARD_ENDPOINT, (uint8_t*) &print_keyboard_report, 8, 50);
		print_keyboard_report.keycode[0] = 0;
		usbSend(KEYBOARD_ENDPOINT, (uint8_t*) &print_keyboard_report, 8, 50);
		print_keyboard_report.keycode[0] = KEY_X;
		usbSend(KEYBOARD_ENDPOINT, (uint8_t*) &print_keyboard_report, 8, 50);
		print_keyboard_report.keycode[0] = 0;
		usbSend(KEYBOARD_ENDPOINT, (uint8_t*) &print_keyboard_report, 8, 50);
}
void keyPrintU4(uint8_t data)
{
	if(data>9){print_keyboard_report.modifier = KEY_RIGHT_SHIFT;}
	else{print_keyboard_report.modifier = 0;}
	print_keyboard_report.keycode[0] = HexToKey[data];
	usbSend(KEYBOARD_ENDPOINT, (uint8_t*) &print_keyboard_report, 8, 50);
	print_keyboard_report.modifier = 0;
	print_keyboard_report.keycode[0] = 0;
	usbSend(KEYBOARD_ENDPOINT, (uint8_t*) &print_keyboard_report, 8, 50);
}
void keyPrintU8(uint8_t data)
{
	uint8_t Data0 = data & 0x0F;
	uint8_t Data1 = (data >> 4) & 0x0F;
	keyPrintU4(Data1);keyPrintU4(Data0);
}
void keyPrintU16(uint16_t data)
{
	uint8_t Data0 = data & 0x00FF;
	uint8_t Data1 = (data >> 8) & 0x00FF;
	keyPrintU8(Data1);keyPrintU8(Data0);
}
void keyPrintU32(uint32_t data)
{
	uint8_t Data0 = data & 0x0000FFFF;
	uint8_t Data1 = (data >> 16) & 0x0000FFFF;
	keyPrintU16(Data1);keyPrintU16(Data0);
}
void keyPrintChinese(uint8_t data[5]){
	memset(print_keyboard_report.keycode,0,6);
	print_keyboard_report.modifier = 0x40;
	print_keyboard_report.keycode[0] =0;
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
	for(uint8_t i=0;i<5;i++){
		print_keyboard_report.keycode[0]=98;
		if(data[i]>0){print_keyboard_report.keycode[0]=data[i]+88;}
		usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
		print_keyboard_report.keycode[0] =0;
		usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
	}
	print_keyboard_report.modifier = 0;
	print_keyboard_report.keycode[0] =0;
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
}
void keyPrintEnglish(uint8_t data)
{
	if(data==0)return;
	memset(print_keyboard_report.keycode,0,6);
	print_keyboard_report.modifier = (data >> 7) ? 0x20 : 0x00;//shift加了128
	print_keyboard_report.keycode[0] =data & 0b01111111;//abs删除正负号
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
	print_keyboard_report.modifier = 0;
	print_keyboard_report.keycode[0] =0;
	usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&print_keyboard_report,8,50);
}
void keyPrintChar(uint16_t wrapdata){
	usbWord_t data=(usbWord_t)wrapdata;
	if(data.bytes[1]==0x00){
		keyPrintEnglish(data.bytes[0]);
		}else{
		uint16_t out=(uint16_t)data.word;
		//out|=0x8080;//汉字内码每个byte最高位为1
		uint8_t datachinese[5];
		datachinese[4]=out%10;out=out/10;
		datachinese[3]=out%10;out=out/10;
		datachinese[2]=out%10;out=out/10;
		datachinese[1]=out%10;out=out/10;
		datachinese[0]=out;
		keyPrintChinese(datachinese);
	}
}

#ifdef MAX_EEP
void keyPrintWord_EEP(uint16_t address_t){
	uint16_t len=eeprom_read_word((uint16_t *)address_t);
	for(uint16_t i=0;i<len;i++){
		uint16_t address=address_t+i*2+2;
		if(address>MAX_EEP-1)break;
		uint16_t data = eeprom_read_word((uint16_t *)address);
		keyPrintChar(data);
	}
}
#endif
////////////////usb repport///////////////
uint8_t macro_report;
uint8_t macro_buffer;
report_mouse_t mouse_report;
report_mouse_t print_mouse_report;
buffer_mouse_t mouse_buffer;
report_raw_t raw_report_in;
report_raw_t raw_report_out;
report_keyboard_t keyboard_report;
report_keyboard_t print_keyboard_report;
buffer_keyboard_t keyboard_buffer;
uint8_t enableReset;//eep change
uint8_t usbKeyboardSendRequired(){
	uint8_t send_required_t=0;
	if(keyboard_report.modifier!=keyboard_buffer.keyboard_modifier_keys)
	{keyboard_report.modifier = keyboard_buffer.keyboard_modifier_keys;send_required_t=1;}
	else if(keyboard_report.keycode[0]!=keyboard_buffer.keyboard_keys[0])
	{keyboard_report.keycode[0]=keyboard_buffer.keyboard_keys[0];send_required_t=1;}
	else if(keyboard_report.keycode[1]!=keyboard_buffer.keyboard_keys[1])
	{keyboard_report.keycode[1]=keyboard_buffer.keyboard_keys[1];send_required_t=1;}
	else if(keyboard_report.keycode[2]!=keyboard_buffer.keyboard_keys[2])
	{keyboard_report.keycode[2]=keyboard_buffer.keyboard_keys[2];send_required_t=1;}
	else if(keyboard_report.keycode[3]!=keyboard_buffer.keyboard_keys[3])
	{keyboard_report.keycode[3]=keyboard_buffer.keyboard_keys[3];send_required_t=1;}
	else if(keyboard_report.keycode[4]!=keyboard_buffer.keyboard_keys[4])
	{keyboard_report.keycode[4]=keyboard_buffer.keyboard_keys[4];send_required_t=1;}
	else if(keyboard_report.keycode[5]!=keyboard_buffer.keyboard_keys[5])
	{keyboard_report.keycode[5]=keyboard_buffer.keyboard_keys[5];send_required_t=1;}
	if(send_required_t)keyboard_buffer.Send_Required=send_required_t;
	return send_required_t;
}
uint8_t usbKeyboardSend()
{
	if(keyboard_buffer.Send_Required){
		keyboard_buffer.Send_Required=0;
		uint8_t send_required_t=usbSend(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		return send_required_t;
	}return 1;
}
uint8_t usbMouseSendRequired(){
	uint8_t send_required_t=0;
	if(mouse_report.mouse.buttons!=mouse_buffer.mouse_keys)
	{
		mouse_report.mouse.buttons=mouse_buffer.mouse_keys;
		send_required_t=REPORT_ID_MOUSE;
	}
	if(mouse_report.system_keys.usage!=mouse_buffer.system_keys)
	{
		mouse_report.system_keys.usage=mouse_buffer.system_keys;
		send_required_t=REPORT_ID_SYSTEM;
	}
	if(mouse_report.consumer_keys.usage!=mouse_buffer.consumer_keys)
	{
		mouse_report.consumer_keys.usage=mouse_buffer.consumer_keys;
		send_required_t=REPORT_ID_CONSUMER;
	}
	if(send_required_t)mouse_buffer.Send_Required=send_required_t;
	return send_required_t;
}
uint8_t usbMouseSend(){
	mouse_buffer.Send_Required&=0x03;
	if(mouse_buffer.Send_Required==0)return 1;
	uint8_t send_required_t=0;
	switch(mouse_buffer.Send_Required){
		case REPORT_ID_MOUSE:
		mouse_buffer.Send_Required=0;
		 send_required_t=usbSend(MOUSE_ENDPOINT,(uint8_t *)(&mouse_report.mouse),0x06U,50);
		return send_required_t;
		break;
		case REPORT_ID_SYSTEM:
		mouse_buffer.Send_Required=0;
		send_required_t=usbSend(MOUSE_ENDPOINT,(uint8_t *)(&mouse_report.system_keys),0x03U,50);
		return send_required_t;
		break;
		case REPORT_ID_CONSUMER:
		mouse_buffer.Send_Required=0;
		send_required_t=usbSend(MOUSE_ENDPOINT,(uint8_t *)(&mouse_report.consumer_keys),0x03U,50);
		return send_required_t;
		break;
	}
	return 0;
}
uint8_t isBufferClear(){
	if(mouse_buffer.mouse_keys!=0)return 1;
	if(mouse_buffer.system_keys!=0)return 1;
	if(mouse_buffer.consumer_keys!=0)return 1;
	for (uint8_t i=0; i < 6; i++) {
		if(keyboard_buffer.keyboard_keys[i] != 0)return 1;
	}
	if(keyboard_buffer.keyboard_modifier_keys!=0)return 1;
	if(macro_buffer!=0)return 1;
	return 0;
}
uint8_t usbMacroSendRequired(){
	if (macro_report!=macro_buffer){
		macro_report=macro_buffer;
	return 1;}
	return 0;
}
void clearMouse(){
	memset(&print_mouse_report, 0, sizeof(mouse_report));
	memset(&mouse_report, 0, sizeof(mouse_report));
	memset(&mouse_buffer,0,sizeof(mouse_buffer));
	mouse_report.mouse.report_id= REPORT_ID_MOUSE;
	mouse_report.system_keys.report_id= REPORT_ID_SYSTEM;
	mouse_report.consumer_keys.report_id= REPORT_ID_CONSUMER;
	mouse_buffer.mouse_protocol=1;
}
void clearKeyboard(){
	memset( &print_keyboard_report, 0,sizeof(keyboard_report));
	memset( &keyboard_report, 0,sizeof(keyboard_report));
	memset( &keyboard_buffer, 0,sizeof(keyboard_buffer));
	keyboard_buffer.enable_pressing=1;
	// protocol setting from the host.  We use exactly the same reportMOUSE_ENABLE
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use.
	keyboard_buffer.keyboard_protocol=1;
	// the idle configuration, how often we send the report to the
	// host (ms * 4) even when it hasn't changed
	keyboard_buffer.keyboard_idle_config=125;
	// count until idle timeout
	keyboard_buffer.keyboard_idle_count=0;
}
void clearRaw(){
	memset( &raw_report_in, 0,sizeof(raw_report_in));
	memset(&raw_report_out, 0,sizeof(raw_report_out));
}
uint8_t usbMacroSend() {
	// MACRO0 0x01//full led
	// MACRO1 0x02//rgb led
	// MACRO2 0x04//esc ~
	// MACRO3 0x08//print eep
	//rgb_state和rgb_type定义相同
	//bit7->第1组 0 off, 1 on
	//bit6->第2组 0 off, 1 on
	//bit5->第full组 0 off, 1 on
	//bit4->第RGB组 0 off, 1 on
	//bit0-3->第1组 0 fix rgb_fixcolor[]，1 Rainbow rgb_rainbow[]，print led_mask[ROWS][COLS]
	//RGB_EFFECT_COUNT<=0x0F
	if (macro_report & MACRO0) {
		RGB_STATE ^= 0x20;
	}
	if (macro_report & MACRO1) {
		RGB_STATE ^= 0x10;
	}
#ifdef MAX_EEP
	if(macro_report&MACRO3){
		keyPrintWordEEP(ADD_EEP);
		return 1;
	}
#endif
#ifdef MAX_FLASH
	if (macro_report & MACRO4) {
		//调试用途
		//keyPrintWord_Flash(ADD_FLASH);
		keyPrint0x();
		keyPrintU8(RGB_STATE);
		return 1;
	}
#endif
	if (macro_report & MACRO5) {
		RGB_STATE+=1;
		if ((RGB_STATE & 0x0F) >= RGB_EFFECT_COUNT){RGB_STATE &= 0xF0;}
		return 1;
	}
	return 0;
}
void clearMacro() {
	macro_buffer = 0;
	macro_report = 0;
}
///////////////keys action///////////////////
void pressMacroKey(uint8_t key){
	if(key==MACRO2){
		if(keyboard_report.modifier){
			//不能用keyboard buffer 因为buffer是记录不稳定状态
			//report 则记录稳定状态
			pressKey(KEY_TILDE);
		}
		else{
			pressKey(KEY_ESC);
		}
	}
	macro_buffer|=key;
}
uint8_t pressKey(uint8_t key)
{
	for (uint8_t i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == key) {
			return 1;
		}
	}
	for (uint8_t i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == 0) {
			keyboard_buffer.keyboard_keys[i] = key;
			return 1;
		}
	}
	return 0;
}
void pressModifierKeys(uint8_t key)
{
	keyboard_buffer.keyboard_modifier_keys|=key;
}
void pressMouseKey(uint8_t key){
	mouse_buffer.mouse_keys|=key;
}
void pressSystemKey(uint8_t key){
	mouse_buffer.system_keys=0x0000|key;
}
void pressConsumerKey(uint8_t key){
	uint8_t mask_t=key&0xF0;
	switch(mask_t){
		case 0xB0:
		mouse_buffer.consumer_keys=0x0000|key;
		break;
		case 0xC0:
		mouse_buffer.consumer_keys=0x0000|key;
		break;
		case 0xE0:
		mouse_buffer.consumer_keys=0x0000|key;
		break;
		case 0x80:
		mouse_buffer.consumer_keys=0x0100|key;
		break;
		case 0x90:
		mouse_buffer.consumer_keys=0x0100|key;
		break;
		case 0x20:
		mouse_buffer.consumer_keys=0x0200|key;
		break;
		case 0x00:
		mouse_buffer.consumer_keys=0x0200|key;
		break;
	}
}
uint8_t releaseKey(uint8_t key)
{
	uint8_t send_required=0;
	for (uint8_t i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == key) {
			keyboard_buffer.keyboard_keys[i] = 0;
			send_required=1;
			break;
		}
	}
	return send_required;
}
void releaseModifierKeys(uint8_t key)
{
	keyboard_buffer.keyboard_modifier_keys&=~key;
}
void releaseMouseKey(uint8_t key){
	mouse_buffer.mouse_keys&=~key;
}
void releaseAllMousekeys(){
	mouse_buffer.mouse_keys=0x00;
	mouse_buffer.system_keys=0x0000;
	mouse_buffer.consumer_keys=0x0000;
}
void releaseAllKeyboardKeys()
{
	for (uint8_t i=0; i < 6; i++) {
		keyboard_buffer.keyboard_keys[i] = 0;
	}
	keyboard_buffer.keyboard_modifier_keys=0;
}
////////////////////flash///////////////////
#ifdef MAX_FLASH
uint16_t flashReadHalfWord(uint32_t address)
{
return*(uint16_t*)address;
}
void resetMatrix(uint8_t mask,uint32_t address){
	uint8_t j=0;
	for (uint8_t r = 0; r < ROWS; r++) {
		for (uint8_t c = 0; c < COLS; c++) {
			switch (mask){
				case 0:
				hexa_keys0[r][c]=*((uint8_t *)((uint32_t)j+address));
				break;
				case 1:
				hexa_keys1[r][c]=*((uint8_t *)((uint32_t)j+address));
				break;
				case 2:
				key_mask[r][c]=*((uint8_t *)((uint32_t)j+address));
				break;
			}
			j++;
		}
	}
}
void resetMatrixFormFlash(){
	uint32_t page=START_FLASH;
	uint16_t address_row=*((uint16_t *)(0+page));
	uint16_t address_col=*((uint16_t *)(2+page));
	uint16_t address_hexakeys0=*((uint16_t *)(4+page));
	uint16_t address_hexaKeys1=*((uint16_t *)(6+page));
	uint16_t address_keyMask=*((uint16_t *)(8+page));
	///////////////////////////////////
	if(address_row!=ADD_INDEX){return;}
	if(address_col!=ADD_ROW){return;}
	if(address_hexakeys0!=ADD_COL){return;}
	if(address_hexaKeys1!=ADD_KEYS1){return;}
	if(address_keyMask!=ADD_KEYS2){return;}
	for(uint32_t j=0;j<ROWS;j++){row_pins[j]=*((uint8_t *)(j+(uint32_t)address_row+page));}
	for(uint32_t j=0;j<COLS;j++){col_pins[j]=*((uint8_t *)(j+(uint32_t)address_col+page));}
	resetMatrix(0,(uint32_t)address_hexakeys0+page);
	resetMatrix(1,(uint32_t)address_hexaKeys1+page);
	resetMatrix(2,(uint32_t)address_keyMask+page);
	for(uint32_t j=0;j<(WS2812_COUNT * 3);j++){rgb_fixcolor[j]=*((uint8_t *)(j+(uint32_t)ADD_RGB+page));}
	RGB_TYPE=*((uint8_t *)((uint32_t)ADD_RGBTYPE+page));
	//rgb_type&=0x11;
}
static uint8_t SramTemp[1024]={0};
void FLASHPageWrite() {
	HAL_StatusTypeDef status = HAL_ERROR;
	HAL_FLASH_Unlock();
	SET_BIT(FLASH->CR, FLASH_CR_PER);
	WRITE_REG(FLASH->AR, START_FLASH);
	SET_BIT(FLASH->CR, FLASH_CR_STRT);
	while (status != HAL_OK) {
		status = FLASH_WaitForLastOperation((uint32_t) FLASH_TIMEOUT_VALUE);
		CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	}
	for (int i = 0; i < 1024; i += 4) {
		status = HAL_ERROR;
		while (status != HAL_OK) {
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
			START_FLASH + i, *(uint32_t*) (SramTemp + i));
		}
	}
	HAL_FLASH_Lock();
}
void FLASHPageCopy() {
	//复制一份page31
	for (int i = 0; i < 1024; i += 4) {
		*(uint32_t*) (SramTemp + i) = *(uint32_t*) (START_FLASH + i);
	}
}
void flashWrite() {
	//应该是先写入内存再写入flash
	//	address,word1,word2,word3
	if (enableReset == 0) {
		uint16_t address = raw_report_out.word[0];
		if (address == 0xF1FF && keyboard_buffer.enable_pressing == 1) {
			open_LED();
			keyboard_buffer.enable_pressing = 0;
			FLASHPageCopy();
		} else if (address == 0xF2FF && keyboard_buffer.enable_pressing == 0) {
			FLASHPageWrite();
			keyboard_buffer.enable_pressing = 2;
			close_LED();
		} else {
			if (keyboard_buffer.enable_pressing == 0) {
				//openLED();
				if (address < MAX_FLASH) {
					*(uint16_t*) (SramTemp + address)=raw_report_out.word[1];
				}
				if ((address + 2) < MAX_FLASH) {
					*(uint16_t*) (SramTemp + address+2)=raw_report_out.word[2];
				}
				if ((address + 4) < MAX_FLASH) {
					*(uint16_t*) (SramTemp + address+4)=raw_report_out.word[3];
				}
				//closeLED();
			}
		}
		memset(&raw_report_out, 0, sizeof(raw_report_out));
		enableReset = 1;
	}else{
		//openLED();
	}
}
void keyPrintWord_Flash(uint32_t address_t){
	uint16_t len=*((uint16_t *)address_t);
	if(len<1)return;
	address_t+=2;
	if(address_t>MAX_FLASH)return;
	for(uint16_t i=0;i<len;i++){
		uint16_t data = *((uint16_t*)address_t);
		address_t+=2;
		if(address_t>MAX_FLASH)return;
		if(data==0xFFFF)continue;//保护不要读写空白数据
		keyPrintChar(data);
	}
}
#endif

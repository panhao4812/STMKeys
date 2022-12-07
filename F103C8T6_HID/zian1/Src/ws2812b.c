#include "tim.h"
#include "Functions.h"

#define CODE_1       (58)       //1码定时器计数次数 58/72=0.805
#define CODE_0       (25)       //0码定时器计数次数 25/72=0.347

////1.25us*24=30us
#define Treset        1
/////////////需要抗抖，否则第一个灯绿光闪烁
static uint32_t ws2812_buffer[Treset+WS2812_COUNT+Treset][24];
 uint32_t isDAMReady;

 void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
 	HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_1);
 	isDAMReady=1;
 }
void ws2812Setup(void) {
	//最后2行装在24个0
	uint8_t i,j;
	for (i = 0; i < 24; i++) {
		for(j = 0; j < Treset; j++) {
		ws2812_buffer[Treset+WS2812_COUNT+j][i] = 0;
		ws2812_buffer[j][i] = 0;
		}
	}
	isDAMReady=1;
}
void ws2812Clear(void) {
	for (uint8_t j = 0; j < WS2812_COUNT; j++) {
		ws2812SetRGB(j,0,0,0);
	}
	ws2812Setup();
}
void ws2812Send(void){
	if(isDAMReady==1){
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)ws2812_buffer,(Treset+WS2812_COUNT+Treset)*24);
	isDAMReady=0;
	}
}
void ws2812SetRGB(uint16_t led, uint8_t red, uint8_t green, uint8_t blue) {
	uint8_t i;
	if(led > WS2812_COUNT)return; //avoid overflow 防止写入ID大于LED总数
	for(i=0;i<8;i++) ws2812_buffer[Treset+led][i]   = ( (green & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
	for(i=8;i<16;i++) ws2812_buffer[Treset+led][i]  = ( (red & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
	for(i=16;i<24;i++) ws2812_buffer[Treset+led][i] = ( (blue & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}
void ws2812SetR(uint16_t led, uint8_t red) {
	if (led > WS2812_COUNT) {
		return;
	}
//		for(uint8_t i=0;i<8;i++) ws2812_buffer[led][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
		for(uint8_t i=8;i<16;i++) ws2812_buffer[led][i]  = ( (red & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
//		for(i=16;i<24;i++) ws2812_buffer[led][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}
void ws2812SetG(uint16_t led, uint8_t green) {
	if (led > WS2812_COUNT) {
		return;
	}
		for(uint8_t i=0;i<8;i++) ws2812_buffer[led][i]   = ( (green & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
	//	for(uint8_t i=8;i<16;i++) ws2812_buffer[led][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
	//	for(i=16;i<24;i++) ws2812_buffer[led][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}
void ws2812SetB(uint16_t led, uint8_t blue) {
	if (led > WS2812_COUNT) {
		return;
	}
	//	for(i=0;i<8;i++) ws2812_buffer[led][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
	//	for(i=8;i<16;i++) ws2812_buffer[led][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
		for(uint8_t i=16;i<24;i++) ws2812_buffer[led][i] = ( (blue & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}
/////////////////////Color//////////////////////
 uint8_t Rcolors [WS2812_COLOR_COUNT]=
{
	243,243,243,243,243,243,243,243,243,242,242,242,242,242,242,242,241,241,241,241,241,240,240,240,240,
	240,239,239,239,239,239,238,238,238,238,238,237,237,237,237,237,237,237,236,236,236,236,236,236,236,
	236,236,236,236,236,236,235,235,235,235,234,234,233,233,232,232,231,230,230,229,228,228,227,226,225,
	224,224,223,222,221,220,220,219,218,217,217,216,215,215,214,214,213,213,212,212,211,211,211,210,210,
	210,210,210,210,210,209,209,208,207,206,204,203,201,199,197,195,192,190,187,185,182,179,176,173,169,
	166,163,160,156,153,150,147,144,140,137,134,131,128,126,123,121,118,116,114,112,110,109,107,106,105,
	104,104,103,103,103,103,103,102,102,101,101,100,99,98,97,95,94,93,91,90,88,86,85,83,81,
	79,77,75,73,71,69,67,66,64,62,60,58,56,55,53,51,50,48,47,46,45,44,43,42,41,
	40,40,39,39,39,39,39,39,39,38,38,38,37,37,36,35,35,34,33,32,31,30,29,28,27,
	26,25,24,23,22,20,19,18,17,16,15,14,13,11,10,9,8,8,7,6,5,4,4,3,3,
	2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5,5,6,
	6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15,15,16,16,16,17,17,
	17,17,18,18,18,18,18,18,18,18,18,19,19,20,21,21,22,23,24,26,27,28,30,31,33,
	34,36,38,40,41,43,45,47,49,51,53,54,56,58,60,62,63,65,66,68,69,71,72,73,74,
	75,76,77,77,78,78,79,79,79,79,79,80,81,82,83,85,87,89,91,93,96,99,102,105,109,
	113,116,120,124,128,132,137,141,145,150,154,158,162,167,171,175,179,183,186,190,194,197,200,203,206,
	208,210,212,214,216,217,218,219,220,220,220,220,220,220,220,221,221,221,222,222,223,223,224,224,225,
	226,226,227,228,228,229,230,231,232,233,233,234,235,236,237,237,238,239,240,240,241,242,242,243,244,
	244,245,245,245,246,246,246,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,246,246,
	246,246,246,246,246,246,246,246,245,245,245,245,245,245,245,245,244,244,244,244,244,244,244,244,244,
	244,243,243,243,243,243,243,243,243,243,243,243
};
 uint8_t Gcolors[WS2812_COLOR_COUNT] =
{
	57,57,57,57,57,56,56,56,55,55,54,54,53,52,52,51,50,50,49,48,47,46,45,44,43,
	43,42,41,40,39,38,37,36,35,35,34,33,32,32,31,30,30,29,29,28,28,28,27,27,27,
	27,27,27,27,27,27,26,26,26,25,25,25,24,24,23,22,22,21,20,20,19,18,17,17,16,
	15,14,13,13,12,11,10,9,8,8,7,6,6,5,4,4,3,3,2,2,1,1,1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,1,1,2,3,4,5,6,7,8,10,11,13,15,17,19,21,23,25,
	27,29,31,33,36,38,40,42,44,46,49,51,53,55,56,58,60,61,63,64,66,67,68,69,70,
	70,71,71,72,72,72,72,73,73,74,75,76,78,79,81,83,86,88,91,93,96,99,102,106,109,
	112,116,120,123,127,131,134,138,142,145,149,152,156,159,162,165,168,171,174,176,179,181,183,185,187,
	188,189,190,191,192,192,192,192,192,192,192,192,192,191,191,191,191,190,190,190,190,189,189,189,188,
	188,187,187,187,186,186,185,185,184,184,184,183,183,182,182,182,181,181,180,180,180,180,179,179,179,
	179,179,178,178,178,178,178,178,178,178,178,178,178,178,178,178,179,179,179,179,179,179,180,180,180,
	180,180,181,181,181,181,182,182,182,182,183,183,183,183,183,184,184,184,184,185,185,185,185,185,185,
	185,186,186,186,186,186,186,186,186,186,186,186,187,187,187,188,189,189,190,191,192,193,194,196,197,
	198,199,201,202,204,205,207,208,210,212,213,215,216,218,219,221,222,224,225,226,227,229,230,231,232,
	233,234,234,235,236,236,236,237,237,237,237,237,237,236,236,235,234,233,232,231,229,228,226,225,223,
	221,219,217,215,212,210,208,206,203,201,198,196,194,191,189,187,184,182,180,178,176,174,172,170,169,
	167,166,165,163,162,161,161,160,160,159,159,159,159,159,158,157,156,155,154,153,151,149,147,145,143,
	141,138,136,133,130,128,125,122,119,115,112,109,106,103,100,97,94,91,88,85,82,80,77,75,72,
	70,68,66,65,63,62,60,59,59,58,57,57
};
  uint8_t Bcolors[WS2812_COLOR_COUNT] =
{
	0,0,0,1,1,1,2,3,4,4,5,7,8,9,10,12,13,15,17,18,20,22,23,25,27,
	29,31,33,34,36,38,40,41,43,45,46,48,49,51,52,53,54,55,56,57,58,58,59,59,60,
	60,60,60,60,60,61,61,62,62,63,64,65,66,67,68,70,71,73,74,76,77,79,81,82,84,
	86,88,89,91,93,95,96,98,100,101,103,105,106,107,109,110,111,112,113,114,115,116,116,117,117,
	118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,119,119,119,119,119,
	119,119,119,119,119,119,119,119,119,119,119,119,120,120,120,120,120,120,120,120,120,120,120,120,120,
	120,120,120,120,120,120,120,120,121,121,122,122,123,123,124,125,126,127,127,128,130,131,132,133,134,
	135,137,138,139,140,142,143,144,145,146,148,149,150,151,152,153,154,155,156,157,157,158,159,159,160,
	160,160,161,161,161,161,161,161,162,162,163,163,164,165,166,167,169,170,172,173,175,176,178,180,182,
	184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,217,219,220,222,223,224,225,226,
	227,227,228,228,229,229,229,229,228,228,227,226,224,223,221,219,216,214,211,208,205,202,198,194,191,
	187,183,179,175,171,166,162,158,154,149,145,141,137,133,129,125,122,118,115,112,109,106,103,101,99,
	97,95,94,92,91,91,90,90,90,90,89,89,88,87,86,85,84,83,81,80,78,76,74,72,70,
	68,66,63,61,58,56,54,51,48,46,43,41,39,36,34,31,29,27,25,23,21,19,18,16,15,
	13,12,11,10,9,9,8,8,8,8,8,8,8,8,9,9,9,10,10,10,11,11,12,13,13,
	14,14,15,16,16,17,18,19,19,20,21,21,22,23,24,24,25,26,26,27,27,28,29,29,30,
	30,30,31,31,31,32,32,32,32,32,32,32,32,32,32,32,32,31,31,31,31,31,30,30,30,
	30,29,29,29,28,28,28,27,27,26,26,26,25,25,25,24,24,24,23,23,23,22,22,22,22,
	21,21,21,21,21,20,20,20,20,20,20,20,20,20,20,20,19,19,19,19,18,18,18,17,17,
	16,16,15,15,14,14,13,13,12,11,11,10,10,9,8,8,7,7,6,6,5,4,4,3,3,
	3,2,2,1,1,1,1,0,0,0,0,0
};


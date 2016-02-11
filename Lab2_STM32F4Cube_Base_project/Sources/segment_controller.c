//#include "stm32f4xx_hal.h"
//#include "segment_controller.h"

//volatile int displayTick = 0;

///* GPIO configuration */
//void Display_GPIO_Config(void) {
//	
//	//Enable clock for GPOIG
//	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
//	
//	//Initialize struct
//	GPIO_InitTypeDef GPIO_InitDef;
//	 
//	//All will have same mode
//	GPIO_InitDef.Pin = segA | segB | segC | segD | segE | segF | segG | segDP | sel1 | sel2 | sel3 | sel4;
//	//Mode output
//	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;   //push pull
//	//Without pull resistors
//	GPIO_InitDef.Pull = GPIO_NOPULL;
//	//pin speed
//	GPIO_InitDef.Speed = GPIO_SPEED_FREQ_MEDIUM;// max frequency for our processor is 84MHz
//	 
//	HAL_GPIO_Init(GPIOB, &GPIO_InitDef);
//}

//void display(float value) {
//	int number = getDigit(value, displayTick);
//	setPins(number);
//}

//int getDigit(float value, int place) {
//	int tmp = (int) value * 100;
//	switch (place) {
//		case 0:
//			return (tmp - tmp % 1000) / 1000;
//		case 1:
//			return (tmp % 1000 - tmp % 100) / 100;
//		case 2:
//			return (tmp % 100 - tmp % 10) / 10;
//		case 3:
//			return tmp % 10;
//		default:
//			return 0;
//	}
//}

//void setPins(int number) {
//	uint8_t pattern = patterns[number];
//		
//	int i = 8;
//	for (;i > 0; i--) {
//		if ((pattern & ( 1 << i )) >> i == 1) {
//			GPIO_SetBits(GPIOB, segments[i]);
//		} else {
//			GPIO_ResetBits(GPIOB, segments[i]);
//		}
//	}
//}

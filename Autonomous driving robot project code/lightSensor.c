
#include <stdint.h>
#include "stm32f10x.h"
#include "lightSensor.h"


//This function continuously checks for the light sensor intensity detected by the Photoelectric sensor
uint32_t lightSensor(void) {
	  uint32_t lightIntensity;
	  lightIntensity = 0;
		while((GPIOB->IDR & GPIO_IDR_IDR2) == GPIO_IDR_IDR2)
		{
			lightIntensity = lightIntensity+5;
		}
		lightIntensity = (lightIntensity*10)/58;		
		return lightIntensity; 	
}
#include <stdint.h>
#include "stm32f10x.h"
#include "drive.h"




void delay(uint32_t delay)
{
  uint32_t i=0;
	for(i=0; i< delay; ++i)
	{
	}
}

void drive(void)
{
	  
	  uint32_t timePulse;
  	uint32_t timePulse1;
	  timePulse = 0;
	  timePulse1 = 0;
	
			while(FrontUSsensor() > 0x1F4) //keep moving forward as long as the front sensor is not blocked
			{ 
				//0x3E8 1v
				//0x2EE 0.750v
				//0x1F4 0.5v
				timePulse++;
				allOff();
				if (timePulse >= 40) //A time pulse is utilized to ensure the credibility of the Ultra Sonic sensor's signal. If the sensor 
													   //continuously returns the same signal for 40 pulses move on with powering the motors
					{
							uint32_t on= 7; //This value controls the positive duty cycle of the motor
							uint32_t i= 0;
							for(i=0; i< on; ++i) //we use this for loop to create a duty cycle to control the speed of the motor
							{
								GPIOA->BSRR = Right_Forward_ON; //right forward wheel
								GPIOA->BSRR = Left_Forward_ON;  //left forward wheel
								delay(36000);
								
							}
							allOff(); //The allOff() function ensures that all four motor signal pins are off to ensure that the forwards and reverse 
							          //motor signals are not triggered at the same time as it can cause damage to the driver
					}
					
			}
			while (FrontUSsensor() < 0x1F4){ //The program only enters this while loop if the front sensor detects a blockade
						timePulse1++;
							
							allOff(); //Ensure that the motor trigger pins are turned off
							delay(360000); //A delay is introduced to give the robot some time to slow down before turning
						  while (LeftUSsensor() < 50) { //If the left Sensor detects a blockade turn right
								turnRight();
							}								
							while (RightUSsensor() < 50) { //If the right Sensor detects a blockade turn left
								turnLeft();
							}								
								
					
				}
			timePulse1 = 0;
			if (lightSensor() > 50)
			{
				GPIOA->BSRR = GPIO_BSRR_BS11; //pin PA11
			}
			else if ((lightSensor() < 50))
			{
			  GPIOA->BSRR = GPIO_BSRR_BR11;
			}
				
				
				
}



void allOff(void)
{
	//Turns all motor drivers off to make sure forward and reverse are not turned on at the same time to
	//ensure the driver does not get damaged
	  
		GPIOA->BSRR = Right_Forward_OFF; 
		GPIOA->BSRR = Left_Forward_OFF;
	  GPIOA->BSRR = Right_Reverse_OFF;
  	GPIOA->BSRR = Left_Reverse_OFF;
		
}


//This function uses differential steering to enable the robot to turn left by turning the left wheel forward and right wheel backward
void turnLeft(void)
{	
		allOff();
	  uint32_t timePulseLeft;
	  timePulseLeft = 0;
		while (RightUSsensor() < 50) //If there is a blockade infront of the right sensor
		{
			timePulseLeft++;
			if (timePulseLeft > 10)
			{

				GPIOA->BSRR = Left_Forward_ON; //Red led right
				GPIOA->BSRR = Right_Reverse_ON; //Red led left
			}
		}
		
}


//This function uses differential steering to enable the robot to turn right by turning the right wheel forward and left wheel backward
void turnRight(void)
{	
		uint32_t timePulseRight;
	  timePulseRight = 0;
		allOff();
		while (LeftUSsensor() < 50) //If there is a blockade infront of the left sensor
		{
				GPIOA->BSRR = Right_Forward_ON; //Red led right
				GPIOA->BSRR = Left_Reverse_ON; //Red led left
			 
		}
		allOff();
		
}

//This function turns on the PWM feature of the STM32. This is used to send a trigger pulse in the Ultra Sonic sensors.
void tim3GpioSetup(void)
{
	TIM3->CR1 |= TIM_CR1_CEN; // Enable Timer3
	TIM3->EGR |= TIM_EGR_UG; // Reinitialize the counter
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // PWM mode 1
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1FE; // Preload Enable, Fast Enable
	TIM3->CCER |= TIM_CCER_CC1E; // Enable CH1
	TIM3->PSC = 0x00000018; //0x095F Divide 24 MHz by 2400 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
	TIM3->ARR = 100; // 100 counts = 10 ms or 100 Hz
	TIM3->CCR1 = 50; // 50 counts = 5 ms = 50% duty cycle
	TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN; // Enable Timer
}


//Uses the Analog-to-digital feature of the STM32 to convert the pulses being returned from the ultra sonic sensor
uint32_t FrontUSsensor(void)
{
	uint32_t distance;
  distance = 0;
	
	ADC1->SQR3 |= (ADC_SQR3_SQ1_2);
	ADC1->CR2 |= (ADC_CR2_ADON); //Turning on the ADC system again to start conversion
	while (!(ADC1->SR & ADC_SR_EOC));
	distance = ADC1->DR;
	return distance; 
}

//Measures the duration of time the ultra sonic sensor echo pin returns a high signal and converts it into distance
uint32_t RightUSsensor(void)
{  
		uint32_t distanceRight;
	  distanceRight = 0;
		while((GPIOA->IDR & GPIO_IDR_IDR15) == GPIO_IDR_IDR15)
		{
			delay(10);
			distanceRight = distanceRight+5;
		}
		distanceRight = (distanceRight*10)/58;		
		return distanceRight;
}


//Measures the duration of time the ultra sonic sensor echo pin returns a high signal and converts it into distance
uint32_t LeftUSsensor(void)
{
	  uint32_t distanceLeft;
  	distanceLeft = 0;
		while((GPIOA->IDR & GPIO_IDR_IDR14) == GPIO_IDR_IDR14)
		{
			delay(10);
			distanceLeft = distanceLeft+5;
		}
		distanceLeft = (distanceLeft*10)/58;		
		return distanceLeft;
}

#include <stdint.h>


void drive(void);
void tim3GpioSetup(void);
void turnLeft(void);
void turnRight(void);
void allOff(void);
void delay(uint32_t delay);


uint32_t FrontUSsensor(void);
uint32_t RightUSsensor(void);
uint32_t LeftUSsensor(void);
uint32_t lightSensor(void);


//These pins are used to trigger the signal that enable both the front wheels to either move forwards of backwards 
 #define Right_Forward_ON 0X00000080 //PA7
 #define Right_Forward_OFF 0x00800000
 
 #define Right_Reverse_ON 0X00000100 // PA8 
 #define Right_Reverse_OFF 0x01000000
 
 #define Left_Forward_ON 0X00000200 // PA9
 #define Left_Forward_OFF 0x02000000
 
 #define Left_Reverse_ON 0X00000400 // PA10
 #define Left_Reverse_OFF 0x04000000
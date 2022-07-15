#include "util.h"
#include "Initialize.h"
#include "stm32f10x.h"
#include "drive.h"


int main (void)
{
    clockInit();
		tim3GpioSetup();
		while(1) //this loop always remains true
		{
			drive(); //drive forward
		}
}

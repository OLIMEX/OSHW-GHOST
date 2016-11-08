//#include <htc.h>
#include <xc.h>
// CONFIG
#pragma config WDTE = ON        // Watchdog Timer (WDT enabled)
#pragma config CP = OFF         // Code Protect (Code protection off)
#pragma config MCLRE = OFF       // Master Clear Enable (GP3/MCLR pin function  is MCLR)
#define	RESET_WATCHDOG()	{\
								asm ("CLRWDT");\
								OPTION = 0xDD;\
							}



#define	DELAY	5	// number of cycles with the same duty cycle
#define	CYCLES	(long int)DELAY * 256 * (2*NUMBER_OF_STEPS - 2);	// Index: 1-->29; 28-->0 ==> 58 cycles


#define	LED_ON	GPIO = GPIO | 0x04	// GPIO2 --> 1
#define	LED_OFF	GPIO = GPIO & 0xFB	// GPIO2 --> 0
void Delay (volatile unsigned int Period);
void Pulse (unsigned char NumberOfPulses)
{
	while (NumberOfPulses--)
	{


			RESET_WATCHDOG();
	
            LED_ON;
            Delay(32);
			LED_OFF;
            Delay(64);

	}
	return;
}

void Delay (volatile unsigned int Period)
{
 unsigned char a=255,b = Period;
 while (a--){
  b=Period;     
  while (--b)
		RESET_WATCHDOG();
 }   
}


char Button ()
{
	char Timer = 0, Counter;
	char Flag = 1;
	RESET_WATCHDOG();
	TRISGPIO = 0x09;
	//TRISGPIO = TRISGPIO & 0xFD;	// GPIO1 --> Output
	//TRISGPIO = TRISGPIO | 0x01;	// GPIO0 --> Input
	GPIO = GPIO & 0xFC;				// GPIO1 --> 0; GPIO0 --> 0
	GPIO = 0x02;	// GPIO1 --> 1
	RESET_WATCHDOG();
	Timer = 0;
	while (!(GPIO & 0x01))	// wait until GPIO0 is 1 and count the time
		Timer++;
	RESET_WATCHDOG();

	if (Timer < 2)		// if the time is less than 2 units the capacity button isn't touched
		Flag = 0;
	TRISGPIO = 0x08;	// GPIO1 --> Output; GPIO0 --> Output
	GPIO = GPIO & 0xFC;			// GPIO1 --> 0; GPIO0 --> 0;
	Delay (100);
	RESET_WATCHDOG();
	TRISGPIO = 0x09;
	RESET_WATCHDOG();
	return Flag;
}

int main ()
{
	RESET_WATCHDOG();
	TRISGPIO = 0x08;
    CMCON0 = 0xF7;    //CMPON = 0x00;
	RESET_WATCHDOG();
	if (Button())
		Pulse (10);
	RESET_WATCHDOG();
	TRISGPIO = 0x08;
	RESET_WATCHDOG();
	GPIO = 0;
	asm("SLEEP");
	return 0;
}

#include <reg932.h>
#include <stdlib.h>
#include <project2.h>


// main driver function
void main()
{
	
		
	P0M1 = 0x00;
	P1M1 = 0x02;
	P2M1 = 0x00;
	P1M2 = 0x01;
	
	// timer for random sequence	
	TMOD = 0x02; //timer0, 8bit mode
	TH0 = 0x01;	 //inital value of TH0
	TR0 = 1;	 //turn timer on

	uart_init(); //initialize serial transmission

	while(1)
	{
		simon();
	}
}
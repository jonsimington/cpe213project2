#include <uart.h>
#include <stdlib.h>

//Global Variables
char EASY = 1;  //1 = EASY
unsigned int SEQ_SIZE = 2;
unsigned char seed;
unsigned char random_num;
unsigned char k;
unsigned char intro[17] = "Simon Board Game";
unsigned char displaydiff[5];
unsigned char arraylength = 0;
unsigned char SEQUENCE[5]; // array of LEDs
unsigned char counter = 0;	// counter used to iterate through sequence
unsigned char button_pressed = 0;	// flag to represent if a button is pressed
unsigned char i, j;	// loop counters
unsigned char input_match; // set to true when the user presses the correct button
unsigned char high_score = 0;

//Ports used to display score on 7-segment display
sbit ss_A = P0^2;
sbit ss_B = P1^2;
sbit ss_C = P1^3;
sbit ss_D = P2^1;

//static bit mtxbusy;

// Bottom LEDs
sbit BOT_YEL_LED = P2^5;
sbit BOT_GRN_LED = P0^7;
sbit BOT_ORNG_LED = P2^6;

// Yellow LED
sbit YEL_LED = P0^5;
sbit YEL_BUTTON = P0^1;

// Green LED
sbit GRN_LED = P2^7;
sbit GRN_BUTTON = P2^3;

// Blue LED
sbit BLU_LED = P1^6;
sbit BLU_BUTTON = P1^4;

// Red LED
sbit RED_LED = P0^4;
sbit RED_BUTTON = P0^0;

// Speaker
sbit SPEAKER = P1^7;

// Start Button
sbit START_BUTTON = P2^2;

//Hard Difficulty Button
sbit DIFFICULTY_BUTTON = P0^3;

// Function Prototypes	 
void check_sequence(char color);
void play_sequence();
void delay(unsigned int itime);
void flash_bot_leds();
void C1(int time);
void E1(int time);
void G1(int time);
void B1(int time);
void C2(int time);
void DSharp2(int time);
void E2(int time);
void F2(int time);
void gameover();
void gamelosesound();
void gamewinsound();
void generatesequence();
void simon();
void display_score(char score);

// This function checks to see if the user pressed the button which corresponds
// to the current color in the sequence
void check_sequence(char color)
{
	if(SEQUENCE[j] == color)
	{
		input_match = 1;
	}
	else
	{
		input_match = 0;
	}
}

// This function shows the sequence up to counter on the LEDs
void play_sequence()
{
	for(i = 0; i <= counter; i++)
	{
		if(SEQUENCE[i] == 'Y')
		{
			YEL_LED = 0;
			if(EASY)
			{
			  C1(200);
			}
			else
			{
			  C1(100);
			}
			YEL_LED = 1;
		}
	
		if(SEQUENCE[i] == 'G')
		{
			GRN_LED = 0;
			if(EASY)
			{
			  G1(300);
			}
			else
			{
			  G1(150);
			}
			GRN_LED = 1;
		}
	
		if(SEQUENCE[i] == 'B')
		{
			BLU_LED = 0;
		    if(EASY)
			{
			  C2(400);
			}
			else
			{
			  C2(200);
			}
			BLU_LED = 1;
		}
	
		if(SEQUENCE[i] == 'R')
		{
			RED_LED = 0;
			if(EASY)
			{
			  F2(600);
			}
			else
			{
			  F2(300);
			}
			RED_LED = 1;
		}
	
	    if(EASY)
		{
		  delay(250);
		}
		else
		{
		  delay(125);
		}
	}
}

// Delay with itime being in milliseconds
void delay(unsigned int itime)
{
	unsigned int l;
	for(l = 0; l < itime; l++)
	{
		TMOD = 0x10;
		TH1 = -3687 >> 8;
		TL1 = -3687;
		TR1 = 1;
		while(!TF1);
		TR1 = 0;
		TF1 = 0; 
	}
}

// This function flashes the bottom row of LEDs once
void flash_bot_leds()
{
	// LED on
	BOT_YEL_LED = 0;
	BOT_GRN_LED = 0;
	BOT_ORNG_LED = 0;
	delay(100);
	// LED off
	BOT_YEL_LED = 1;
	BOT_GRN_LED = 1;
	BOT_ORNG_LED = 1;

}

// This function flashes the bottom row of LEDs three times and 
// plays a sound when the user loses the game
void gameover()
{
	delay(500);

    if(counter == SEQ_SIZE) //Checking for win condition
    {
      //game winning tune
    } 
    else
    {
      gamelosesound();
    }

	// flash bottom LEDs
	flash_bot_leds();
	delay(100);
	flash_bot_leds();
	delay(100);
	flash_bot_leds();

	// reset counter upon losing the game
   	counter = 0;
}

void generatesequence()
{
	seed = TL0;
	srand(seed);

	for(k = 0; k < SEQ_SIZE; k++)
	{
		random_num = (RAND_MAX * rand()) % 4;

		switch(random_num)
		{
			case 0:
				SEQUENCE[k] = 'Y';
				break;
			case 1:
				SEQUENCE[k] = 'G';
				break;
			case 2:
				SEQUENCE[k] = 'R';
				break;
			case 3:
				SEQUENCE[k] = 'B';
				break;
		}
	}
}

// This function plays the simon says game
void simon()
{
	//display current highscore
	display_score(high_score);

	// wait until user presses start button
	arraylength = 16;
	for(i = 0; i < arraylength; i++)
	{
	  //uart_transmit(intro[i]);
	}

	while(START_BUTTON)
	{
	  if(!DIFFICULTY_BUTTON)
	  {
	    BOT_GRN_LED = !BOT_GRN_LED;
		EASY = !EASY;
		while(!DIFFICULTY_BUTTON){}
	  }
	}

	if(EASY == 1)
	{
	  displaydiff[0] = 'E';
	  displaydiff[1] = 'a';
	  displaydiff[2] = 's';
	  displaydiff[3] = 'y';
	}
	else
	{
	  displaydiff[0] = 'H';
	  displaydiff[1] = 'a';
	  displaydiff[2] = 'r';
	  displaydiff[3] = 'd';
	}
	arraylength = 4;

	for(i = 0; i < arraylength; i++)
	{
	  //uart_transmit(displaydiff[i]);
	}

	BOT_GRN_LED = 1;

	arraylength = 4;

	generatesequence(); //generates random sequence

	while(1)
	{
		display_score(counter);		

		delay(250);
		// play sequence up to counter
		play_sequence();

		// listen for button presses
		for(j = 0; j <= counter; j++)
		{
			while(!button_pressed)
			{
				// yellow button was pressed
				if(YEL_BUTTON == 0)
				{
					YEL_LED = 0;
					C1(200);
					while(YEL_BUTTON == 0){}
					// blink LED on button press
					YEL_LED = 1;

					check_sequence('Y');

					button_pressed = 1;
						
				}

				// green button was pressed
				if(GRN_BUTTON == 0)
				{
					GRN_LED = 0;
					G1(300);
					while(GRN_BUTTON == 0){}
					// blink LED on button press
					GRN_LED = 1;
					
					check_sequence('G');
	
					button_pressed = 1;			
				}

				// blue button was pressed
				if(BLU_BUTTON == 0)
				{
					BLU_LED = 0;
					C2(400);
					while(BLU_BUTTON == 0){}
					// blink LED on button press
					BLU_LED = 1;
					
					check_sequence('B');
	
					button_pressed = 1;				
				}

				// red button was pressed
				if(RED_BUTTON == 0)
				{
					RED_LED = 0;
					F2(600);
					while(RED_BUTTON == 0){}
					//blink LED on button press
					RED_LED = 1;
				
					check_sequence('R');
	
					button_pressed = 1;				
				}
			}

			if(input_match != 1)
			{
				button_pressed = 0;
				break;
			}
			else
			{
				input_match = 0;
			}
			//reset button_pressed flag
			button_pressed = 0;
			delay(125);
		}

		// user matched all of sequence, up to counter, inc counter
		if(j == counter + 1)
		{
			counter++;
			if(counter == SEQ_SIZE)
			{
				//win condition
				gamewinsound();
				display_score(counter);
				high_score = counter;
				gameover();
				return;
			}
		}
		// user didn't match the sequence at some point, gameover
		else
		{
			//update high_score variable if appropriate
			if(counter > high_score)
			{
				high_score = counter;
			}			

			gameover();
			return;
		}
	}
}

void C1(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -3523 >> 8;
    TL1 = -3523;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void E1(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -2796 >> 8;
    TL1 = -2796;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void G1(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -2351 >> 8;
    TL1 = -2351;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void B1(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -1866 >> 8;
    TL1 = -1866;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void C2(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -1761 >> 8;
    TL1 = -1761;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void DSharp2(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -1481 >> 8;
    TL1 = -1481;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void E2(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -1398 >> 8;
    TL1 = -1398;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void F2(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -1319 >> 8;
    TL1 = -1319;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void GSharp2(int time)
{
  int i;
  TMOD = 0x10;
  for(i=0; i < time; i++)
  {
    TH1 = -1387 >> 8;
    TL1 = -1387;
    TR1 = 1;
    while(!TF1);
    TR1 = 0;
    TF1 = 0;
	SPEAKER = !SPEAKER;
  }
}

void gamelosesound()
{
  B1(180);
  delay(38);
  F2(300);
  delay(75);
  F2(300);
  delay(75);
  F2(300);
  delay(75);
  F2(300);
  delay(75);
  E2(270); 
  delay(75);
  DSharp2(250);
  delay(75);
  C2(200);
  delay(60);
  G1(150);
  delay(113);
  E1(121);
  delay(60);
  C1(100);
}

void gamewinsound()
{
  G1(150);
  delay(38);
  C1(100);
  delay(75);
  E1(121);
  delay(75);
  GSharp2(300);
  delay(75);
  G1(150);
  delay(75);
  C1(100); 
  delay(75);
  E1(121);
  delay(75);
  G1(300);
}

void display_score(char score)
{
	switch(score)
	{
		case 0:
			ss_A = 0; ss_B = 0; ss_C = 0; ss_D = 0;
			break;
		case 1:
			ss_A = 1; ss_B = 0; ss_C = 0; ss_D = 0;
			break;
		case 2:
			ss_A = 0; ss_B = 1; ss_C = 0; ss_D = 0;
			break;
		case 3:
			ss_A = 1; ss_B = 1; ss_C = 0; ss_D = 0;
			break;
		case 4:
			ss_A = 0; ss_B = 0; ss_C = 1; ss_D = 0;
			break;
		case 5:
			ss_A = 1; ss_B = 0; ss_C = 1; ss_D = 0;
			break;
		case 6:
			ss_A = 0; ss_B = 1; ss_C = 1; ss_D = 0;
			break;
		case 7:
			ss_A = 1; ss_B = 1; ss_C = 1; ss_D = 0;
			break;
		case 8:
			ss_A = 0; ss_B = 0; ss_C = 0; ss_D = 1;
			break;
		case 9:
			ss_A = 1; ss_B = 0; ss_C = 0; ss_D = 1;
			break;
	}
}
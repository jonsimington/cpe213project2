//Global Variables
char EASY = 1;               //1 = easy mode, 0 = hard mode
unsigned int SEQ_SIZE = 15;  // the size of the  simon sequence

unsigned char seed;          // a seed for rand()
unsigned char random_num;    // random number from 0-3 to generate sequence
unsigned char k;             // counter used in generate_sequence()

unsigned char SEQUENCE[15];  // the simon sequence
unsigned char counter = 0;	 // counter used to iterate through sequence
unsigned char button_pressed = 0;	// flag to represent if a button is pressed
unsigned char i, j;	         // loop counters
unsigned char input_match;   // set to true when the user presses the correct button

// Bottom row of LEDs
sbit BOT_YEL_LED = P2^5;
sbit BOT_GRN_LED = P0^7;
sbit BOT_ORNG_LED = P2^6;

// Yellow simon LED / button
sbit YEL_LED = P0^5;
sbit YEL_BUTTON = P0^1;

// Green simon LED / button
sbit GRN_LED = P2^7;
sbit GRN_BUTTON = P2^3;

// Blue simon LED / button
sbit BLU_LED = P1^6;
sbit BLU_BUTTON = P1^4;

// Red simon LED / button
sbit RED_LED = P0^4;
sbit RED_BUTTON = P0^0;

// Onboard Speaker
sbit SPEAKER = P1^7;

// Simon Start Button
sbit START_BUTTON = P2^2;

//Hard Difficulty Button
sbit DIFFICULTY_BUTTON = P0^3;

// Function Prototypes
// In order of how they appear below
void simon();
void check_sequence(char color);
void play_sequence();
void delay(unsigned int itime);
void flash_bot_leds();
void gameover();
void generatesequence();
void C1(int time);
void E1(int time);
void G1(int time);
void B1(int time);
void C2(int time);
void DSharp2(int time);
void E2(int time);
void F2(int time);


// This function plays the simon says game
void simon()
{
  // wait until user presses start button
  while(START_BUTTON)
  {
    // if difficulty button is pressed, light LED and change mode
    if(!DIFFICULTY_BUTTON)
    {
      BOT_GRN_LED = !BOT_GRN_LED;
      EASY = !EASY;
      while(!DIFFICULTY_BUTTON){}
    }
  }

  // turn difficulty LED back off
  BOT_GRN_LED = 1;

  generatesequence(); //generates random sequence

  while(1)
  {
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
          // blink LED, play sound on button press
          YEL_LED = 0;
          C1(200);
          YEL_LED = 1;

          check_sequence('Y');

          button_pressed = 1;

        }

        // green button was pressed
        if(GRN_BUTTON == 0)
        {
          // blink LED, play sound on button press
          GRN_LED = 0;
          G1(300);
          GRN_LED = 1;

          check_sequence('G');

          button_pressed = 1;
        }

        // blue button was pressed
        if(BLU_BUTTON == 0)
        {
          // blink LED, play sound on button press
          BLU_LED = 0;
          C2(400);
          BLU_LED = 1;

          check_sequence('B');

          button_pressed = 1;
        }

        // red button was pressed
        if(RED_BUTTON == 0)
        {
          //blink LED, play sound on button press
          RED_LED = 0;
          F2(600);
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
        //play different tune than gameover
        gameover();
        return;
      }
    }
    // user didn't match the sequence at some point, gameover
    else
    {
      gameover();
      return;
    }
  }
}

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
// speed of game depends on EASY flag
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
	unsigned int i, j;
	for(i = 0; i < itime; i++)
	{
		for(j = 0; j < 1275; j++);
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
	// flash bottom LEDs
	flash_bot_leds();
	delay(100);
	flash_bot_leds();
	delay(100);
	flash_bot_leds();

	// make gameover tune

	// reset counter upon losing the game
   	counter = 0;
}

// This function generates a random sequence of LEDs
void generate_sequence()
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

// This function plays a C1 tone through the speaker
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

// This function plats an E1 tone through the speaker
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

// This function plays a G1 tone through the speaker
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

// This function plays a B1 tone through the speaker
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

// This function plays a C2 tone through the speaker
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

// This function plays a DSharp2 tone through the speaker
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

// This function plays an E2 tone through the speaker
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

// This function plays an F2 tone through the speaker
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

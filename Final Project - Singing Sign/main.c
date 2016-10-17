#include <msp430.h>
/*
 * The code can be used to interface with the HC06 Bluetooth adpator board.
 * The code waits for data to be received which then calls the UART interrupt,
 * the received buffer is then assigned to the variable Rx_Data.  Rx_Data is
 * used in a switch case statment to action various functions depending on the
 * ASCII code received over Bluetooth.
 */

#include <msp430g2553.h>
#include "lcd.h"
#include <string.h>
#include <stdio.h>


#define SPK BIT6  //TA0.1
#define c 3824
#define cSH 3608
#define d 3405
#define dSH 3214
#define dH 1702
#define e 3034
#define eH 1517
#define f 2863
#define fSH 2703
#define fH 1431
#define g 2551
#define gSH 2408
#define gH 1275
#define a 2273
#define aSH 2145
#define b 2025
#define cH 1911
unsigned int dur=100;
unsigned int stopper=100000000000;


void tone(int tone,int duration)  //Tone macro and duration in millisecs.Play other frq simply divide it (smclk/notef) and feed it into the function
{   int i;
	for(i=0;i<duration;i++)
	{ CCR0=tone;       //Tone length
	  CCR1=tone/2;     //Half Tone ON and Half tone OFF i.e 50% duty!
	  _delay_cycles(1000);
	}
}
void delay_ms(unsigned int ms )
{
      unsigned int i;
      for (i = 0; i<= ms; i++)
        __delay_cycles(1000);               //Built-in function that suspends the execution for 500 cycles
}

void notone(int i)        //To stop the tone can`t use delay_ms as its a hardware PWM not software!
{

	CCR0=0; //make main period zero..it doesnt work when you set CCR1 to zero because it will always set-reset at zero.
	delay_ms(i);
}

void toneInit()       //Initialise The Timers
{

			 CCTL1 = OUTMOD_7;          // CCR1 reset/set

			 TACTL = TASSEL_2 + MC_1;   // SMCLK, up mode
}
void seq1()
{
tone(d,400);
notone(dur);
tone(g,400);
notone(dur);
tone(g,200);
notone(dur);
tone(a,200);
notone(dur);
tone(g,200);
notone(dur);
tone(fSH,200);
notone(dur);//One Segment
tone(e,400);
notone(dur);
tone(c,400);
notone(dur);
tone(e,400);
notone(dur);
tone(a,400);
notone(dur);
tone(a,200);
notone(dur);
tone(b,200);
notone(dur);//Two Segment
tone(a,200);
notone(dur);
tone(g,200);
notone(dur);
tone(fSH,400);
notone(dur);
tone(d,400);
notone(dur);
tone(d,400);
notone(dur);
tone(b,400);
notone(dur);//Three Segment
tone(b,200);
notone(dur);
tone(cH,200);
notone(dur);
tone(b,200);
notone(dur);
tone(a,200);
notone(dur);
tone(g,400);
notone(dur);
tone(e,400);
notone(dur);
tone(d,200);
notone(dur);
tone(d,200);
notone(dur);
tone(e,400);
notone(dur);
tone(a,400);
notone(dur);
tone(fSH,400);
notone(dur);
tone(g,800);
notone(dur);
}

void seq2()
{
	tone(f,200);
	notone(dur);
	tone(dSH,200);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(aSH,400);
	notone(dur);//One Segment
	tone(cH,400);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(dSH,400);
	notone(dur);
	tone(f,200);
	notone(dur);
	tone(dSH,200);
	notone(dur);
	tone(gSH,400);
	notone(dur);//Two Segment
	tone(gSH,400);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(gSH,400);
	notone(500);//Three Segment
	tone(f,200);
	notone(dur);
	tone(dSH,200);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(cH,400);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(dSH,400);
	notone(dur);
	tone(f,200);
	notone(dur);
	tone(dSH,200);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(gSH,400);
	notone(dur);
	tone(aSH,200);
	notone(dur);
	tone(aSH,200);
	notone(dur);
	tone(gSH,400);
	notone(dur);
}

void seq3()
{
	tone(g,900);
	notone(dur);
	tone(cH,900);
	notone(dur);
	tone(cH,400);
	notone(dur);
	tone(cH,400);
	notone(dur);
	tone(cH,400);
	notone(dur);
	tone(cH,400);
	notone(dur);//One Segment
	tone(cH,400);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(g,200);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(cH,400);
	notone(dur);
//	tone(b,400);
//	notone(dur);
	tone(cH,400);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(g,200);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(cH,400);
	notone(dur);
//	tone(b,400);
//	notone(dur);
	tone(cH,400);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(g,200);
	notone(dur);
	tone(aSH,400);
	notone(dur);
	tone(cH,400);
	notone(dur);
//	tone(b,400);
//	notone(dur);
	tone(cH,800);
	notone(dur);
	tone(aSH,800);
	notone(dur);
}

void seq4()
{
	tone(b,100);
	notone(dur);
	tone(b,100);
	notone(dur);
	tone(b,100);
	notone(dur);
	tone(b,100);
	notone(dur);

	tone(a,200);
	notone(dur);
	tone(a,200);
	notone(dur);

	tone(fSH,200);
	notone(dur);

	tone(fSH,200);
	notone(dur);
	tone(a,300);
	notone(dur);
}

void seq5()
{
	tone(cH,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(g,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(eH,200);
	notone(dur);
	tone(dH,400);
	notone(dur);
	tone(eH,400);
	notone(1400);
	tone(cH,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(g,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(eH,200);
	notone(dur);
	tone(dH,800);
	notone(1400);
	tone(cH,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(g,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(eH,200);
	notone(dur);
	tone(fH,200);
	notone(dur);
	tone(eH,200);
	notone(dur);
	tone(cH,400);
	notone(200);
	tone(g,200);
	notone(dur);
	tone(fH,200);
	notone(dur);
	tone(eH,200);
	notone(dur);
	tone(cH,400);
	notone(200);
	tone(cH,200);
	notone(dur);
	tone(cH,200);
	notone(dur);

	tone(g,200);
	notone(dur);
	tone(cH,200);
	notone(dur);
	tone(eH,200);
	notone(dur);
	tone(gH,800);
	notone(dur);
}


unsigned char Rx_Data = 0;					// Byte received via UART
unsigned int count = 0;						// Used for the flashing LED demonstration
const char msg1[]="Hello Welcome!";
const char msg2[]="Cya!";
const char msg3[]="Merry Christmas!";
const char msg4[]="The Doctor is in";
const char msg5[]="Beware Ninjas Today";
const char msg6[]="Dango Please";
const char msg7[]="No Blizzard Elsa!";

int main(void)
{
	/*** Set-up system clocks ***/
	WDTCTL = WDTPW + WDTHOLD;				// Stop WDT
	if (CALBC1_1MHZ == 0xFF)				// If calibration constant erased
			{
			while (1);						// do not load, trap CPU!
			}
	DCOCTL = 0;								// Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;					// Set DCO
	DCOCTL = CALDCO_1MHZ;
	/*** Set-up GPIO ***/
	P1SEL = BIT1 + BIT2;					// P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2;					// P1.1 = RXD, P1.2=TXD
	P1DIR |= BIT6 + BIT0;					// P1.6 set as output
	P1OUT &= ~(BIT6 + BIT0);				// P1.6 set low
	/*** Set-up USCI A ***/
	UCA0CTL1 |= UCSSEL_2;					// SMCLK
	UCA0BR0 = 104;							// 1MHz 9600
	UCA0BR1 = 0;							// 1MHz 9600
	UCA0MCTL = UCBRS0;						// Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;					// Initialize USCI state machine
	IE2 |= UCA0RXIE;						// Enable USCI_A0 RX interrupt
	LCD_setup();

	__bis_SR_register(LPM0_bits + GIE);		// Enter LPM0, interrupts enabled

  while(1)
  {
		LCD_init();
		toneInit();

	  				switch (Rx_Data)
	  				{
	  					case 0x41:							// ON Command
	  						//while (!(IFG2&UCA0TXIFG));	// USCI_A0 TX buffer ready?
	  						//UCA0TXBUF = 0x41;				// Send 8-bit character
	  					    TA0CCTL0 &= ~CCIE;				// Disable Timer0_A interrupts
	  					    P1SEL &= ~BIT6;					// P1.6 selected as GPIO
	  						P1OUT |= BIT6 + BIT0;			// P1.0 and P1.6 set high
	  						LCD_send_string((char *) msg1);
	  						break;

	  					case 0x42:							// OFF Command
	  						//while (!(IFG2&UCA0TXIFG));	// USCI_A0 TX buffer ready?
	  						//UCA0TXBUF = 0x41;				// Send 8-bit character
	  					    TA0CCTL0 &= ~CCIE;				// Disable Timer0_A interrupts
	  					    P1SEL &= ~BIT6;					// P1.6 selected as GPIO
	  						P1OUT |= BIT6 + BIT0;			// P1.0 and P1.6 set high
	  						LCD_send_string((char *) msg2);
	  						break;

	  					case 0x46:							// FLASH Command
	  						//while (!(IFG2&UCA0TXIFG));	// USCI_A0 TX buffer ready?
	  						//UCA0TXBUF = 0x42;				// Send 8-bit character
	  					    TA0CCTL0 &= ~CCIE;				// Disable Timer0_A interrupts
	  						P1DIR|=SPK; //Spk as output
	  						P1SEL|=SPK; //Speaker as TA0.1
	  						toneInit();
	  						LCD_send_string((char *) msg3);
	  						seq1();
	  						delay_ms(500);
	  						break;

	  					case 0x3E:							// INCREASE Command
	  						 TA0CCTL0 &= ~CCIE;				// Disable Timer0_A interrupts
	  						 P1DIR|=SPK; //Spk as output
	  						 P1SEL|=SPK; //Speaker as TA0.1
	  						 LCD_send_string((char *) msg5);
	  						 seq4();
	  						 delay_ms(500);
	  						break;

	  					case 0x3C:							// DECREASE Command

	  						 TA0CCTL0 &= ~CCIE;				// Disable Timer0_A interrupts
	  						 P1DIR|=SPK; //Spk as output
	  						 P1SEL|=SPK; //Speaker as TA0.1
	  						 LCD_send_string((char *) msg7);
	  						 seq5();
	  						 delay_ms(500);

	  						break;

	  					case 0x31:							// 1 Command

	  					    TA0CCTL0 &= ~CCIE;				// Disable Timer0_A interrupts
	  						P1DIR|=SPK; //Spk as output
	  						P1SEL|=SPK; //Speaker as TA0.1
	  						LCD_send_string((char *) msg4);
	  						seq3();
	  						delay_ms(500);


	  						break;

	  					case 0x32:							// 2 Command
	  						break;

	  					case 0x33:							// 3 Command
	  					    TA0CCTL0 &= ~CCIE;				// Disable Timer0_A interrupts
	  						P1DIR|=SPK; //Spk as output
	  						P1SEL|=SPK; //Speaker as TA0.1
	  						LCD_send_string((char *) msg6);
	  						seq2();
	  						delay_ms(500);
	  						break;

	  					case 0x34:							// 4 Command
							break;

	  					case 0x35:							// 5 Command

	  						break;

	  					default: break;
	  				}
						 __bis_SR_register(LPM0_bits);	// Enter LPM0, interrupts enabled
  }
}

//  USCI A interrupt handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
	Rx_Data = UCA0RXBUF;					// Assign received byte to Rx_Data
	__bic_SR_register_on_exit(LPM0_bits);	// Wake-up CPU
}

#pragma vector=TIMER0_A0_VECTOR     // Timer0 A0 interrupt service routine
   __interrupt void Timer0_A0 (void) {

	   count++;
	   if (count == 10)
		   {
		   P1OUT ^= BIT0 + BIT6;					// P1.0 Toggle (Red LED)
		   count =0;
		   }
}

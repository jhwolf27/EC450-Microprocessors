/*
*
*	HW5
*	By Jackson Hsu & Krystal Kallarackal
*
*/

#include "msp430g2553.h"
#define SPEAKER_MASK 0x01	//bit mask for speaker
#define STATE 67
#define SPI_CLK 0x20
#define SPI_SOMI 0x40
#define SPI_SIMO 0x80
#define BRLO (BIT_RATE_DIVISOR &  0xFF)
#define BRHI (BIT_RATE_DIVISOR / 0x100)
#define BIT_RATE_DIVISOR 32
volatile unsigned short data = 0;	//data variable
volatile unsigned long data_count = 0;	//data variable counter

void init_spi(void);

void main(){

    WDTCTL = WDTPW + WDTHOLD;	//stop watchdog timer
	BCSCTL1 = CALBC1_8MHZ;
  	DCOCTL  = CALDCO_8MHZ;
    
    P1DIR |= SPEAKER_MASK;	//set speaker direction
    P1OUT &= ~SPEAKER_MASK;
    init_spi();	//setup spi
    _bis_SR_register(GIE+LPM0_bits);

}

void interrupt spi_rx_handler(){ //interrupt handler

    data = UCB0RXBUF;	//record data
    ++data_count;	//increment data counter
    IFG2 &= ~UCB0RXIFG;	//reset flag
    if (data > STATE) { //if data is greater than the state, then make sound
        P1OUT |= SPEAKER_MASK;
    } else { //if data is less than or equal to the state, then don't make a sound
        P1OUT &= ~SPEAKER_MASK;
    }

}

void init_spi(){ //initialize spi

    UCB0CTL1 = UCSWRST;
    UCB0CTL0 = UCCKPH + UCMODE_0 + UCSYNC;
    UCB0BR0 = BRLO;
    UCB0BR1 = BRHI;
    UCB0CTL1 &= ~UCSWRST;
    IFG2 &= ~UCB0RXIFG;
    IE2 |= UCB0RXIE;
    P1SEL = SPI_CLK + SPI_SOMI + SPI_SIMO;
    P1SEL2= SPI_CLK + SPI_SOMI + SPI_SIMO;

}

ISR_VECTOR(spi_rx_handler, ".int07") //interrupt handler

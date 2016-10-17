
//Jackson Hsu and Krystal Kallarackal

#include "msp430g2553.h"

#define ADC_INPUT_BIT_MASK 0x10
#define ADC_INCH INCH_4
#define ACTION_RESET 1
#define BIT_RATE_DIVISOR 32
#define SPI_CLK 0x20
#define SPI_SOMI 0x40
#define SPI_SIMO 0x80
#define BRLO (BIT_RATE_DIVISOR &  0xFF)
#define BRHI (BIT_RATE_DIVISOR / 0x100)
#define SCALE_COEFFICIENT 3                     // Coefficient to scale the results

/* Global Variables */
volatile short updated_data;               // RECORD result
volatile short data_scaled;               // Result after being scaled
volatile unsigned long updates=0;
volatile unsigned long tmit_counter = 0;        // Counter to count number of transmits
volatile unsigned int actionscount = ACTION_RESET;

/* Function Declarations */
void init_adc(void);
void start_conversion(void);
void init_spi(void);
void init_wdt(void);

/*
 * The main program just initializes everything and leaves the action to
 * the interrupt handlers!
 */
void main(){
	WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
	BCSCTL1 = CALBC1_8MHZ;
  	DCOCTL  = CALDCO_8MHZ;

    init_adc();                 // Set up ADC
  	init_spi();                 // Set up SPI
  	init_wdt();                 // Set up WDT
 	_bis_SR_register(GIE + LPM0_bits);
}

/* =============== Functions =============== */
/* Function to setup ADC */
void init_adc(){
    ADC10CTL1 = ADC_INCH + SHS_0 + ADC10DIV_4 + ADC10SSEL_0 + CONSEQ_0;
    ADC10AE0 = ADC_INPUT_BIT_MASK;
    ADC10DTC1 = 1;
    ADC10CTL0= SREF_0 + ADC10SHT_3 + ADC10ON + ENC;
}

/* Function to fetch result */
void start_conversion(){
    if ((ADC10CTL1&ADC10BUSY) == 0){
        ADC10CTL0 |= ADC10SC;
        ADC10SA=(unsigned) &updated_data;
        ++updates;
    }
}

/* Function to setup WDT */
void init_wdt(){
    WDTCTL =(WDTPW + WDTTMSEL + WDTCNTCL);
    IE1 |= WDTIE;
}

/* Function to setup SPI */
void init_spi(){
    UCB0CTL1 = UCSSEL_2 + UCSWRST;
    UCB0CTL0 = UCCKPH + UCMST + UCMODE_0 + UCSYNC;
    UCB0BR0 = BRLO;
    UCB0BR1 = BRHI;
    UCB0CTL1 &= ~UCSWRST;
    P1SEL = SPI_CLK + SPI_SOMI + SPI_SIMO;
    P1SEL2 = SPI_CLK + SPI_SOMI + SPI_SIMO;
}
/* ========================================= */

/* ==================== Interrupt Handler ================= */
interrupt void WDT_interval_handler(){
    start_conversion();
    if (--actionscount==0){
    	data_scaled = (updated_data/SCALE_COEFFICIENT);  // Scale the result
        UCB0TXBUF=data_scaled;                        // Return UPDATED result
        ++tmit_counter;
        actionscount=ACTION_RESET;                 // Reset counter
    }
}

ISR_VECTOR(WDT_interval_handler, ".int10")
/* ======================================================== */

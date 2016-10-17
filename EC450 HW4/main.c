asm(" .length 10000");
asm(" .width 132");
// TimerTone0 == produces a 1 Khz tone using TimerA, Channel 0
// Toggled on and off with a button
// Using the timer in up mode with NO INTERRUPT
//

#include "msp430g2553.h"
//-----------------------
// The following definitions allow us to adjust some of the port properties
// for the example:

// define the bit mask
#define TA0_BIT 0x02

//on brd LEDs 
#define GREEN 0x40
#define RED 0x01
// define the location for the button (this is the built in button)
#define BUTTON_BIT 0xB8 // initialize all pins
#define BUTTON_BIT_FAST_FORWARD 0x16//fast forward
#define BUTTON_BIT_SLOW_DOWN 0x32 // slowdown
#define BUTTON_BIT_PLAY 0x80 // play
#define BUTTON_BIT_PAUSE 0x08 // pause
//----------------------------------

// speed of song
unsigned volatile int play_speed = 2;

///joy to the world freq array & duration of note
unsigned const int joy_to_the_world_freq[59] = {956,1012,1136,1275, 1433, 1515,1706, 1908, 1275,1136, 1136, 1012,1012, 956,956, 956,1012, 1136, 1275,1275,1433, 1515, 956, 956,1012, 1136,1275, 1275,1433,1515 , 1515,1515,1515,1515,1515,1433, 1275, 1, 1433, 1515, 1706,1706,1706,1706,1515, 1433, 1515,1706, 1515,956, 1136,1275,1433,1515, 1433,1515,1706,1908,1};
unsigned const int joy_to_the_world_note[59] = {164,108,68,201, 67, 134,  134, 201,67, 177, 91,177,91, 177, 91, 67,67,67,67, 77,37, 77,77,67,67,67,67,77,38,77, 77, 67,67,67, 33,34, 134,67, 33,34,67,67,67,34,35, 201,  33,34, 67,  134, 67,  77,37,77,77, 134, 134, 268 ,525};
/// pokemon theme song
unsigned const int poketheme_freq[59] = {852,1,852,1,852,1,852,1,852, 956,1136,1433,1,1433,1,1433,852,1,852,956,1073,956,1,804,1,804,1,804,1,804, 852,1908,1073,1,1073,1,1073, 852,1,852,1908, 1073,1,852,1,852,1,852,1,852,1,852,1,852,956,1136,1275,1, 852,1,852,956,1073,956, 759,1,563,1,506,1 };
unsigned const int poketheme_note[59] = {70,10,70,10,70,30,70,30,33,70,33,34,10,70,10,67,67,10,67,33,34,67,134,67,10,67,10,67,10,67, 33,67,33,10,67,10,34, 67,10,33,67,33,10,201, 268,33,10,33,10,33,10,67,10,33, 67,34,67,67, 33,10,67,67,33,34, 60,10,60,10,60, 525};

/// 
unsigned volatile int array_counter = 0;  // counter for moving through pairs of array
unsigned volatile int count_down = 0; ///count down for duration of each note
///

volatile int state_sound = 0;// state to skip to next 0 is joy, 1 is pokemon
////
void timer_init(void); // initialization for timer
void button_init(void); // initialization for button

// ++++++++++++++++++++++++++
void main(){
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;    // 1Mhz calibration for clock
    DCOCTL  = CALDCO_1MHZ;
    WDTCTL =(WDTPW + // (bits 15-8) password
                  // bit 7=0 => watchdog timer on
                  // bit 6=0 => NMI on rising edge (not used here)
                  // bit 5=0 => RST/NMI pin does a reset (not used here)
          WDTTMSEL + // (bit 4) select interval timer mode
          WDTCNTCL +  // (bit 3) clear watchdog timer counter
             0 // bit 2=0 => SMCLK is the source
             +1 // bits 1-0 = 01 => source/8K
      );
    ///////////////
    //initialize first count down 
    if(state_sound == 0){
        count_down = joy_to_the_world_note[array_counter] / play_speed;
    }
    if(state_sound == 1){
        count_down = poketheme_note[array_counter] / play_speed;
    }

    IE1 |= WDTIE;  // enable the WDT interrupt (in the system interrupt register IE1)
    timer_init();  // initialize timer
    button_init(); // initialize the button
   
    _bis_SR_register(GIE+LPM0_bits);// enable general interrupts and power down CPU
}

// +++++++++++++++++++++++++++
// Sound Production System
void timer_init(){              // initialization and start of timer
    TA0CTL |= TACLR;              // reset clock
    TA0CTL = TASSEL_2+ID_1+MC_1;  // clock source = SMCLK
                                // clock divider=1
                                // UP mode
                                // timer A interrupt off
    TA0CCTL0 = 0; // compare mode
    //////////////////
    //initialize the TAOCRRO to the first note
    if(state_sound == 0){
        TA0CCR0 = joy_to_the_world_freq[array_counter] - 1; //(1 / ((2 * joy_to_the_world_freq[0]) / 1000000) - 1); // in up mode TAR=0... TACCRO-1
    } else if(state_sound == 1){
        TA0CCR0 = poketheme_freq[array_counter] - 1;
    } else {

    }
    ////////////////
    P1SEL|=TA0_BIT; // connect timer output to pin
    P1DIR|=TA0_BIT;
}

// +++++++++++++++++++++++++++
// Button input System
// Button toggles the state of the sound (on or off)
// action will be interrupt driven on a downgoing signal on the pin
// no debouncing (to see how this goes)

void button_init(){
// All GPIO's are already inputs if we are coming in after a reset
    P1OUT |= (BUTTON_BIT+GREEN+RED); // pullup
    P1REN |= BUTTON_BIT; // enable resistor
    P1IES |= BUTTON_BIT; // set for 1->0 transition
    P1IFG &= ~BUTTON_BIT;// clear interrupt flag
    P1IE  |= BUTTON_BIT; // enable interrupt
    P1DIR |= (GREEN+RED); //enable leds
}

//
interrupt void WDT_interval_handler(){
    // use watch dog timer to set up countdown to play notes
    if (--count_down == 0){
        if(state_sound == 0){
            //next duratio set up
            count_down = joy_to_the_world_note[array_counter] / play_speed;
            //next note to be played
            TA0CCR0 = joy_to_the_world_freq[array_counter] - 1;
        }else if(state_sound == 1){
            //same as above except for pokemon
            count_down = poketheme_note[array_counter] / play_speed;
            TA0CCR0 = poketheme_freq[array_counter] - 1;
        } else {

        }
        array_counter +=1; //increment counter
        P1OUT ^= GREEN; //toggle LED to beat of the song - signals if playing and speed
    }
    if(state_sound == 0){
        if(array_counter > 59){
            array_counter = 0; //reset array when song is done
        }
    } else if(state_sound == 1){
        if(array_counter > 69){ //reset array
            array_counter = 0;
        }
    } else {

    }
}
// DECLARE function WDT_interval_handler as handler for interrupt 10
// using a macro defined in the msp430g2553.h include file
ISR_VECTOR(WDT_interval_handler, ".int10")

void interrupt button_handler(){
// check that this is the correct interrupt
// (if not, it is an error, but there is no error handler)
    if (P1IFG & BUTTON_BIT_PAUSE){ // pause button
        play_speed = 2;
        P1IFG &= ~BUTTON_BIT_PAUSE; // reset the interrupt flag
        //TACCTL0 ^= OUTMOD_4; // toggle outmod between 0 and 4 (toggle)
        TACCTL0 = 0; //toggles mode 0 for pause
        P1OUT ^= RED; //toggle red led for pause aka not outputting
     }
    if(P1IFG & BUTTON_BIT_PLAY){
        play_speed = 2;
        P1IFG &= ~BUTTON_BIT_PLAY;
        TACCTL0 = OUTMOD_4; //toggles up mode for playing
        //P1IES ^= BUTTON_BIT_PLAY;
        P1OUT = ~RED; //shut off red led
    }
    if(P1IFG & BUTTON_BIT_FAST_FORWARD){
        //this if statement allows moving ot next song
        if(play_speed == 4){
            if(state_sound == 0){
                state_sound = 1;
                array_counter = 0;
                play_speed = 2;
            } else if(state_sound == 1){
                state_sound = 0;
                array_counter = 0;
                play_speed = 2;
            } else {

            }
        }
        P1OUT = ~RED;
        play_speed = 4; //speed up song 2x
        P1IFG &= ~BUTTON_BIT_FAST_FORWARD;
        P1IES ^= BUTTON_BIT_FAST_FORWARD;
    }
    if(P1IFG & BUTTON_BIT_SLOW_DOWN) {
        //slows down song by half
        P1OUT = ~RED;
        play_speed = 1;
        P1IFG &= ~BUTTON_BIT_SLOW_DOWN;
        P1IES ^= BUTTON_BIT_SLOW_DOWN;
    }
}
ISR_VECTOR(button_handler,".int02") // declare interrupt vector
// +++++++++++++++++++++++++++


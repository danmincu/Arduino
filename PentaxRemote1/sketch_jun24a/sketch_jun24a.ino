#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>


#define DDRLED DDRD
#define PORTLED PORTD

#define DDRBUZ DDRC
#define PORTBUZ PORTC
#define BUZ A5

#define DDRENC DDRC
#define PINENC PINC
#define ENCVAL (PINENC & 0x03) // PC0 and PC1

//***************************************************************
// Rotary encoder management
//***************************************************************

void nothing() { }

void (* enc_inc)() = &nothing;
void (* enc_dec)() = &nothing;

void adjust_nothing() {
	enc_inc = &nothing;
	enc_dec = &nothing;
}


int enc_last = 0;
void pool_enc() {
	int val =  ENCVAL;        
	if (val == enc_last) return;
	else if (enc_last == 1 && val == 3) (*enc_dec)();
	else if (enc_last == 1 && val == 0) (*enc_inc)();
	else if (enc_last == 2 && val == 0) (*enc_dec)();
	else if (enc_last == 2 && val == 3) (*enc_inc)();
        Serial.println(enc_last + ">>" + val);
	enc_last = val;
}

//***************************************************************
// Buzzer routines
//***************************************************************

void beep(int time) {
	int i;
	PORTBUZ |= _BV(BUZ); // 1
	for(i=0; i<time; i++) _delay_ms(1);
	PORTBUZ &=~_BV(BUZ); // 0
}

void start_beep() {
	int i;
	PORTLED = 0xFF;
	beep(80);
	PORTLED = 0x00;
	for(i=0; i<80; i++) _delay_ms(1);
	PORTLED = 0xFF;
	beep(500);
	PORTLED = 0x00;
}

//***************************************************************
// Initialization
//***************************************************************

void init() {
	DDRLED = 0xFF; // led outputs
	DDRBUZ |= _BV(BUZ); // output
	DDRENC &= ~_BV(0x03); // PC0 and PC1 as inputs

	EIMSK= (0<<INT1)|(0<<INT0); //INT0 masked

}

//***************************************************************
// Power management
//***************************************************************

volatile int pwclk = 0;
void pwm_act() { pwclk = 0; }

// wake up

ISR(PCINT1_vect)
{
	PCICR = 0x00;
	PCMSK1 = 0x00;
}

void shutdown() {

	PORTLED = 0x00;
	DDRLED = 0x00; // all as input
	DDRBUZ &= ~_BV(BUZ); // input

	/*     The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings */
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); //SLEEP_MODE_PWR_DOWN
	sleep_enable();


	//EICRA = 0x01; // Interrupt on rising edge
	//EIMSK= (0<<INT1)|(1<<INT0); //INT0 unmasked

	PCICR = 0x02;
	PCMSK1 = 0x03;

	sleep_mode();
	sleep_disable();

	pwm_act();
	init();
	start_beep();

	sei();

}

void pwm_tic() {
	pwclk++;
	if(pwclk > 1500) shutdown();
}

//***************************************************************
// Define a variable that can be adjusted
//***************************************************************
#define NONE 0
#define INC 1
#define DEC 2
volatile int value = 0;
volatile int last = NONE;
void inc_value() {
	if(last == INC) {
		value++;
		if (value>7) value = 0; pwm_act();
		beep(1);
		last = NONE;
	}
	else {
		last = INC;
	}
}
void dec_value() {
	if(last == DEC) {
		value--;
		if (value<0) value = 7;
		pwm_act();
		beep(1);
		last = NONE;
	}
	else {
		last = DEC;
	}
}
void adjust_value() {enc_inc = &inc_value; enc_dec = &dec_value;}

//***************************************************************
// Main
//***************************************************************

int pixel[8];

int main() {
  
  Serial.begin(9600);
  Serial.println("Ready for spin");
  
	init();
	adjust_value();
	sei();
	start_beep();
	pwm_act();

	int cpt = 0;
        int prevCpt = 0;
	int i;
	while (1) {

		pool_enc();

		for(i=0; i<8; i++) {
			if (pixel[i] <= cpt) PORTLED &= ~(1<<i);
		}


		if(cpt >= 25) {
			pwm_tic();
//                        Serial.println(cpt);
			cpt = 0;
			for(i=0; i<8; i++) {
				if (pixel[i] > 0) PORTLED |= (1<<i);
				if(value == i && pixel[i] < 25) pixel[i] += 5;
				if(value != i && pixel[i] > 0)  pixel[i] -= 1;
			}
		}
		else cpt++;
	}
	return 0;
}


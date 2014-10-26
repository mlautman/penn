/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: penn.c
 *  Author: Mike Lautman
 *********************************************
 *********************************************
 */

#include "penn.h"

#define		ADCpins		2
#define 	p_length 	12
#define 	chan		1
#define	 	RX_add 		0x25
#define 	TX_add 		0x17

int receive_buffer;


volatile bool new_packet = false;

bool setup(void){
	// setup main loop timer
	return false;
}

void run(bool usb_debug){
	if (new_packet){
        cli();
		new_packet = false;
        sei();
	}
}

int main(void) {
	// Variables
	bool usb_debug = true;

	// Setup
	bool setup_success = setup();

	// Run
	run(usb_debug);

}



////////////////////////////////////////////////////
//
//	Interupt methods (rf)
//
////////////////////////////////////////////////////

ISR(INT2_vect){
	m_red(2);
	m_rf_read(receive_buffer , p_length);
	new_packet = true;
}

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

char* receive_buffer[p_length];


volatile bool new_packet = false;

bool setup(void){
	// setup main loop timer
	return false;
}

void run(){
}

int main(void) {
	// Variables
	bool usb_debug = true;

	// Setup
	bool setup_success = setup();

	// Run
	while(1){
		run();
	}
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

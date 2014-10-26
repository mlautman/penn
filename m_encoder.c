// -----------------------------------------------------------------------------
// functions to monitor two quadrature encoders
// version: 0.1
// date: Oct 24, 2012
// author: J. Fiene
// -----------------------------------------------------------------------------
// Attach the encoders as follows (reverse A and B to invert polarity)
// B0 = encoder 1, channel A
// B1 = encoder 1, channel B
// B2 = encoder 2, channel A
// B3 = encoder 2, channel B
// -----------------------------------------------------------------------------

#include "m_encoder.h"

//static long encoders[2] = {0,0};
static long* encoders;
static int ticks[2];

void m_encoder_init(long* buffer)
{
    encoders = buffer;      // point to their buffer

	ticks[0] = PINB & 0x03; // get initial encoder tick states
	ticks[1] = PINB & 0x0C; // ^
	
	set(PCICR,PCIE0);		// enable pin-change interrupts
	set(PCMSK0,PCINT0);		// demask B0
	set(PCMSK0,PCINT1);		// demask B1
	set(PCMSK0,PCINT2);		// demask B2
	set(PCMSK0,PCINT3);		// demask B3
	
	sei();                  // enable system interrupts
}

// reset an encoder value to zero
void m_encoder_zero(char channel){
	encoders[(int)channel-1] = 0;
}

// handle encoder input state changes
ISR(PCINT0_vect)
{
	static char lookup[4][4] = {{0,1,-1,0},{-1,0,0,1},{1,0,0,-1},{0,-1,1,0}};
	static int old_ticks[2];
	
	old_ticks[0] = ticks[0];
	ticks[0] = PINB & 0x03;	
	//encoders[1] += lookup[old_ticks[0]][ticks[0]];
	encoders[0] += lookup[old_ticks[0]][ticks[0]];
	
	old_ticks[1] = ticks[1];
	ticks[1] = (PINB & (0x0C))>>2;	
	//encoders[0] += lookup[old_ticks[1]][ticks[1]];
	encoders[1] += lookup[old_ticks[1]][ticks[1]];
}

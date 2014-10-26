/*********************************************
 *********************************************
 *
 *	Project: Penn
 * 	Script: penn.h
 *  Author: Mike Lautman
 *********************************************
 *********************************************
 */

#ifndef __penn__
#define __penn__

#include <avr/io.h>
#include "m_imu.h"
#include "m_bus.h"
#include "m_usb.h"
#include "m_general.h"
#include "m_rf.h"
#include <stdlib.h>

#define		ADCpins		2
#define 	p_length 	12
#define 	chan		1
#define	 	RX_add 		0x25
#define 	TX_add 		0x17


bool usb_debug;

////////////////////////////////////////////////
//
//		IMU helpers
//
////////////////////////////////////////////////



////////////////////////////////////////////////
// init_imu
//
// FUNCTIONALITY
// 		initialize the 9-DOF mIMU board
//		Basically a wrapper for the m_imu_init function
//
// PARAMETERs:
// 		accel_scale: 0 = +/-2G, 1 = +/-4G, 2 = +/-8G, 3 = +/-16G
// 		gyro_scale: 0 = +/-250d/s, 1 = +/-500d/s, 2 = +/-1000d/s, 3 = +/-2000d/s
//
// RETURNS:
// 		1 : success
// 		0 : communication error

#define _a_0 0
#define _a_2 1
#define _a_2 2
#define _a_3 3

// accelerometer_scale
// _a_0 = +/-2G
// _a_1 = +/-4G
// _a_2 = +/-8G
// _a_3 = +/-16G

// gyro_scale:
// _g_0 = +/-250d/s,
// _g_1 = +/-500d/s,
// _g_2 = +/-1000d/s,
// _g_3 = +/-2000d/s

bool init_imu(int accel_scale, int gyro_scale);


////////////////////////////////////////////////
//
//		RF helpers
//
////////////////////////////////////////////////

////////////////////////////////////////////////
// TX_RF_PACKET
//
// FUNCTIONALITY:
//		transmits the newest data to the receiver
//		basically a wrapper function for m_rf_send()
//
// PARAMETERS:
//		time_stamp	: time delta since penn hit paper
//		imu_data 	: raw sensor data from imu
//
// RESPONSE
//		true	: success
// 		false	: communication error
bool tx_rf_packet(unsigned long time_stamp, char* imu_data);


////////////////////////////////////////////////
//
//		MEAT
//
////////////////////////////////////////////////

////////////////////////////////////////////////
// SETUP
//
// Functionality : Takes care of all initiation for the penn.
//	Timers
//   - setup main timer (timer_3) to run at loop_frequency
//   - setup timer 4 for time delta counting
//	IMU
//	 - calls setup imu with correct scaling
//	RF
//   - sets up RF
//	 - sends test com packet (to see if there is a reply)
//  DEBUG
//	 - if usb_debug is enabled, sets up usb com and blocks
//
bool setup(bool usb_debug, int loop_frequency, char imu_scale, char gyro_scale);

////////////////////////////////////////////////
// RUN
//
// Functionality: Repeatedly runs the main loop.
//	 - Runs at given frequency as was set in SETUP
//	 - When the pen tip has just been put to paper,
// 		> grab IMU readings
//		> mash them over rf
//		> if no receipt, add 1 to dropped packets counter.
// 	 - When tip released,
//		> send end of packet msg.

void run();



#endif

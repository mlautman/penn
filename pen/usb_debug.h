/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: usb_debug.h
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */

#ifndef __USB_DEBUG__PENN__
#define __USB_DEBUG__PENN__

#include "m_general.h"
#include "timers.h"
#include "m_usb.h"
#include "imu.h"
#include "wireless.h"

////////////////////////////////////////////////
// USB_DEBUG
//
// Functionality :
//   - set whether usb debugger is used
//
// Parameters
//   - bool activate : whether or not to use usb debugger
//
// Returns
//   - usb_debug state
bool usb_debug(bool activate);


////////////////////////////////////////////////
// USB_DEBUG_send_setupMask
//
// Functionality :
//   - send success or failure of setup ops muxed into single value
//  [
//      wireless_init,
//      wireless_isConnected,
//      imu_init,
//      loop_timer,
//      stopWatch_timer,
//      N/A,
//      N/A,
//      N/A,
//  ]
//
// Parameters
//   - int8_t setup_mask  : mask for setup success/fail bits
//
// Returns
//   - success/failure of transmission
bool usb_debug_send_setupMask();


////////////////////////////////////////////////
// USB_DEBUG_IMU_TX
//
// Functionality :
//   - Sends Imu values over usb instead of wireless
//
// Parameters
//   - int16_t* imu_data
//   - int8_t dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_imu_tx(int16_t* imu_data, int8_t dataLen);


////////////////////////////////////////////////
// USB_DEBUG_RF_TX
//
// Functionality :
//   - Sends rf packets values over usb instead of wireless
//
// Parameters
//   - int8_t* rf_data : data that would normally be sent over rf
//   - int8_t dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
// bool usb_debug_rf_data(int8_t* data, int8_t dataLen);
bool usb_debug_rf_data(char* data, int8_t dataLen);


bool usb_tx_data(char packet_type, uint32_t time_stamp, int8_t* data, uint16_t data_len, char button);

////////////////////////////////////////////////
// USB_DEBUG_RF_DROP_COUNT
//
// Functionality :
//   - Sends number of dropped packets
//
// Parameters
//   - uint16_t drop_cnt : number of dropped packets as cnted by wireless
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_drop_count(uint16_t drop_cnt);


///////////////////////////////////////////////
// USB_DEBUG_STOPWATCH
//
// Functionality :
//   - Sends stopWatch value
//
// Parameters
//   - uint16_t now : stopWatch cnt value
//
// Returns
//   - success/failure of transmission
bool usb_debug_stopWatch(uint32_t now);



#endif

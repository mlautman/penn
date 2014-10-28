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

#import "m_general.h"
#import "m_usb.h"
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
//   - char setup_mask  : mask for setup success/fail bits
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
//   - int* imu_data
//   - char dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_imu_tx(int* imu_data, char dataLen);


////////////////////////////////////////////////
// USB_DEBUG_RF_TX
//
// Functionality :
//   - Sends rf packets values over usb instead of wireless
//
// Parameters
//   - char* rf_data : data that would normally be sent over rf
//   - char dataLen : lenght of rf_data
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_data(char* data, char dataLen);


////////////////////////////////////////////////
// USB_DEBUG_RF_DROP_COUNT
//
// Functionality :
//   - Sends number of dropped packets
//
// Parameters
//   - unsigned int drop_cnt : number of dropped packets as cnted by wireless
//
// Returns
//   - success/failure of transmission
bool usb_debug_rf_drop_count(unsigned int drop_cnt);

#endif

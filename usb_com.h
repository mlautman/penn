/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: usb_com.h
 *  Authors: Mike Lautman
 *********************************************
 *********************************************
 */


////////////////////////////////////////////////
// USB_DEBUG_INIT
//
// Functionality :
//   - setup
//
// Parameters
//   - frequency desired as an integer. (1000 -> 1khz)
//
// Returns
//   - success or failure
bool usb_debug_init(bool activate);

bool usb_debug_setup(bool* setup_mask);

bool usb_debug_imu_tx(int* imu_data);

bool usb_debug_rf_data(char* data);

bool usb_debug_rf_drop_count(unsigned long count);


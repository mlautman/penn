/*********************************************
 *********************************************
 *
 *  Project: Penn
 *  Script: imu.h
 *  Authors: Mike Lautman
 *
 *********************************************
*********************************************
*/
#ifndef __IMU__PENN__
#define __IMU__PENN__

#include "m_general.h"

////////////////////////////////////////////////
// IMU_ISINIT
//
// Functionality :
//   - check to see if the imu is setup
//
// Parameters
//   -
//
// Returns
//   - success or failure
bool imu_isInit(void);

////////////////////////////////////////////////
// IMU_ISINIT
//
// Functionality :
//   - imu setup
//
// Parameters
//   -
//
// Returns
//   - success or failure
bool imu_init(void);


////////////////////////////////////////////////
// IMU_rawData_get
//
// Functionality :
//   - check to see if the imu is setup
//
// Parameters
//   - char data[9] : pointer to array where data will be stored
//
// Returns
//   - success or failure
bool imu_rawData_get(char* data);

#endif
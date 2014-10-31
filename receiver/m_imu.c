 // -----------------------------------------------------------------------------
// M2 9-DOF IMU interface
// version: 2.0
// date: Jun 16, 2012
// author: J. Fiene
// -----------------------------------------------------------------------------

#include "m_imu.h"

#define MIMU            0x69

// private function prototypes

unsigned char m_imu_init(unsigned char accel_scale, unsigned char gyro_scale)
{	    
    // ensure that scales are within range
    if( (accel_scale < 0) || (accel_scale > 3) ) return 0;
    if( (gyro_scale < 0) || (gyro_scale > 3) ) return 0;

	m_bus_init();
    m_wait(300);

	// confirm device is connected
    if(m_read_register(MIMU,0x75) != 0x68) return 0;
    
    // SET THE CLOCK SOURCE TO X-AXIS GYRO
    // reg: 0x68 (PWR_MGMT_1)
    // bits 0-2 control clock source
    // value: 0x01 (clock to PLL on X-axis gyro reference)
    m_write_register(MIMU,0x6B,0x01);

    // SET THE ACCEL FULL-SCALE +/- RANGE (0=2G, 1=4G, 2=8G, 3=16G)
    // reg: 0x1C (ACCEL_CONFIG)
    // bits: 3-4 set +/- full-scale range
    m_write_register(MIMU,0x1C,accel_scale<<3);

    // SET THE GYRO FULL-SCALE +/- RANGE (0=250, 1=500, 2=1000, 3=2000 DEG/SEC)
    // reg: 0x1B (GYRO_CONFIG)
    // bits: 3-4 set +/- full-scale range
    // value: 0x00
    m_write_register(MIMU,0x1B,gyro_scale<<3); // this shouldn't be doing anything
      
    // SET THE MPU INTO I2C BYPASS MODE
    // reg: 0x37 (INT_PIN_CFG)
    // bit: 1 (1=bypass, 0=normal op)
    m_write_register(MIMU,0x37,0x02); // switch the MPU into bypass mode
    
    // CONFIGURE THE MAGNETOMETER
    // address: 0x1E (for the magnetometer)
    // reg: 2 (mode register)
    // val: 0 (continuous output)
    m_write_register(0x1E,2,0); // set the mag. to continuous output mode
    
    // SET THE MPU TO NORMAL I2C MODE
    // reg: 0x37 (INT_PIN_CFG)
    // bit: 5 (1=master, 0=passthrough)
    m_write_register(MIMU,0x37,0);    // switch the MPU out of bypass mode

    // SET THE AUXILLIARY I2C SAMPLE RATE
    // reg: 0x34 (I2C_SLC4_CTRL) 
    // value: 0x1E (30 > 8000/30 = 266.6Hz)
    // reg: 0x67 (MST_DELAY_CTRL)
    // bit: 0 (1=slowed down, 0=full speed)
    m_write_register(MIMU,0x34,0x1E);
    m_write_register(MIMU,0x67,0x01);

    // ADJUST the auxilliary I2C port clock speed, etc.
    // reg: 0x24 (MST_CTRL)
    // bit: 4 = 1 (stop then start), 2 (restart)
    // bit: 0-3 = clock prescaler (0x0D = 400kHz)
    // not necessary, perhaps.  We shall see?
    m_write_register(MIMU,0x24,0x1D);

    // SET THE MAGNETOMETER ADDRESS
    // reg: 0x25 (I2C_SLC0_ADDR)
    // value: 0x1E (from the magnetometer datasheet)
    // notes: for read from slave, set bit 7 to 1; to write, set bit 7 to 0
    m_write_register(MIMU,0x25,0x9E);

    // SET THE MAGNETOMETER DATA START ADDRESS
    // reg: 0x26 (I2C_SLV0_REG)
    // val: 3 (X high byte)
    m_write_register(MIMU,0x26,3);
    
    // OTHER AUXILLIARY I2C SETTINGS
    // reg: 0x27 (I2C_SLV0_CTRL
    // bit: 0-3 (# of bytes to ingest) = 6
    //      4: byte/word grouping control
    //      6: swap bytes
    //      7: enable slave (1=on, 0=off)
    m_write_register(MIMU,0x27,0x96);
    
    // START THE AUXILLIARY I2C PORT
    // reg: 0x6A (USER_CTRL)
    m_write_register(MIMU,0x6A,0x20); // enable master mode
    
    return 1;   // SUCCESS!
}

// MPU-6050 RESULT SCHEMA
// (3B:3C) AXH:AXL, (3D:3E) AYH:AYL, (3F:40) AZH:AZL
// (41:42) TH:TL
// (43:44) GXH:GXL, (45:46) GYH:GYL, (47:48) GZH:GZL
// (49:4A) MXH:MXL, (4B:4C) MYH:MYL, (4D:4E) MZH:MZL

unsigned char m_imu_raw(int* raw_data)
{
    unsigned char buffer[20]; 
    int i;
    
    for(i=0;i<20;i++) // read 20 bytes (6 accel + 2 temp + 6 gyro + 6 mag)
    {
        buffer[i] = m_read_register(MIMU, (0x4E - i) ); // go in reverse to get L:H order
    }
    // buffer: [MZL, MZH, MYL, MYH, MXL, MXH, GZL, GZH, GYL, GYH, GXL, GXH, TL, TH, AZL, AZH, AYL, AYH, AXL, AXH]
    for(i=0;i<3;i++)
    {
        raw_data[2-i] = *(int*)&buffer[14+2*i]; // [2] = [AZL:AZH], [1] = [AYL:AYH], [0] = [AXL:AXH]
        raw_data[5-i] = *(int*)&buffer[6+2*i];  // [5] = [GZL:GZH], [4] = [GYL:GYH], [3] = [GXL:GXH]
        raw_data[8-i] = *(int*)&buffer[2*i];    // [8] = [MZL:MZH], [7] = [MYL:MYH], [6] = [MXL:MXH]
    }
    return 1;
}

unsigned char m_imu_accel(int* raw_data)
{
    int i;
    unsigned char buffer[6];    
    for(i=0;i<6;i++) // read 6 bytes
    {
        buffer[i] = m_read_register(MIMU, (0x40 - i) ); // go in reverse to get L:H order
    }
    // buffer: [AZL, AZH, AYL, AYH, AXL, AXH]
    for(i=0;i<3;i++)
    {
        raw_data[2-i] = *(int*)&buffer[2*i]; // [2] = [AZL:AZH], [1] = [AYL:AYH], [0] = [AXL:AXH]
    }
    return 1;
}

unsigned char m_imu_gyro(int* raw_data)
{
    int i;
    unsigned char buffer[6];    
    for(i=0;i<6;i++) // read 6 bytes
    {
        buffer[i] = m_read_register(MIMU, (0x48 - i) ); // go in reverse to get L:H order
    }
    // buffer: [GZL, GZH, GYL, GYH, GXL, GXH]
    for(i=0;i<3;i++)
    {
        raw_data[2-i] = *(int*)&buffer[2*i]; // [2] = [GZL:GZH], [1] = [GYL:GYH], [0] = [GXL:GXH]
    }
    return 1;
}

unsigned char m_imu_mag(int* raw_data)
{
    int i;
    unsigned char buffer[6];    
    for(i=0;i<6;i++) // read 6 bytes
    {
        buffer[i] = m_read_register(MIMU, (0x4E - i) ); // go in reverse to get L:H order
    }
    // buffer: [MZL, MZH, MYL, MYH, MXL, MXH]
    for(i=0;i<3;i++)
    {
        raw_data[2-i] = *(int*)&buffer[2*i]; // [2] = [MZL:MZH], [1] = [MYL:MYH], [0] = [MXL:MXH]
    }
    return 1;
}
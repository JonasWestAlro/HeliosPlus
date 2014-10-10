#pragma once
#include "MPU9150.hpp"

MPU9150::MPU9150(): i2c(SensorI2C::get_instance()){

	uint8_t data;
	//i2c.read_register8(MPU9150_ADDRESS, MPU9150_RA_WHO_AM_I, 1, &data);

	while(1){
		if(setup() == 0){
			break;
		}else{
			i2c.restart();
			setup();
			break;
		}
	}

	//i2c.set_clock_speed(400000);
	//i2c.restart();
}

bool MPU9150::is_alive(){
	return true;
};
bool MPU9150::is_error(){
	return false;
};


void    MPU9150::restart(){
	i2c.restart();
}

/***************************************************
 * 				ACCELEROMETER METHODS
 ***************************************************/

void    MPU9150::set_acc_calibration(AccelerometerCalibration& new_calibration){
	acc_calibration = new_calibration;
}

void    MPU9150::get_acc_calibration(AccelerometerCalibration& return_calibration){
	return_calibration = acc_calibration;
}


int16_t MPU9150::get_acc_data(float& x, float& y, float& z){
	uint8_t buffer[6];
	int16_t x_temp = 0;
	int16_t y_temp = 0;
	int16_t z_temp = 0;

	//If it is > 1ms since we downloaded data last time, then do it again.
	//else just return the last downloaded values.
	if(Time.get_time_since_us(acc_timestamp) > 1000){
		if(!i2c.read_register8(MPU9150_ADDRESS, MPU9150_RA_ACCEL_XOUT_H, 6, buffer))
			return 0;

		x_temp = (buffer[0]<<8) | buffer[1];
		y_temp = (buffer[2]<<8) | buffer[3];
		z_temp = (buffer[4]<<8) | buffer[5];

		last_acc_values[0] = ((float)x_temp) * ACCRESOLUTION - acc_calibration.offset_x;
		last_acc_values[1] = ((float)y_temp) * ACCRESOLUTION - acc_calibration.offset_y;
		last_acc_values[2] = ((float)z_temp) * ACCRESOLUTION - acc_calibration.offset_z;

		acc_timestamp = Time.get_timestamp();
	}

	x = last_acc_values[0];
	y = last_acc_values[1];
	z = last_acc_values[2];

	return 1;
}

void 	MPU9150::start_acc_calibration(void){}
void 	MPU9150::stop_acc_calibration(void){}


/***************************************************
 * 				GYROSCOPE METHODS
 ***************************************************/
void    MPU9150::set_gyro_calibration(GyroscopeCalibration& new_calibration){
	gyro_calibration = new_calibration;
}

void    MPU9150::get_gyro_calibration(GyroscopeCalibration& return_calibration){
	return_calibration = gyro_calibration;
}


int16_t MPU9150::get_gyro_data(float& x, float& y, float& z){
	uint8_t buffer[6];
	int16_t x_temp = 0;
	int16_t y_temp = 0;
	int16_t z_temp = 0;

	//If it is > 1ms since we downloaded data last time, then do it again.
	//else just return the last downloaded values.
	if(Time.get_time_since_us(gyro_timestamp) > 1000){
		if(!i2c.read_register8(MPU9150_ADDRESS, MPU9150_RA_GYRO_XOUT_H, 6, buffer))
			return 0;

		x_temp = (buffer[0]<<8) | buffer[1];
		y_temp = (buffer[2]<<8) | buffer[3];
		z_temp = (buffer[4]<<8) | buffer[5];

		last_gyro_values[0] = ((float)x_temp) * GYRORESOLUTION - gyro_calibration.offset_x;
		last_gyro_values[1] = ((float)y_temp) * GYRORESOLUTION - gyro_calibration.offset_y;
		last_gyro_values[2] = ((float)z_temp) * GYRORESOLUTION - gyro_calibration.offset_z;

		gyro_timestamp = Time.get_timestamp();
	}

	x = last_gyro_values[0];
	y = last_gyro_values[1];
	z = last_gyro_values[2];

	if(gyro_calibrating) process_gyro_calibration();

	return 1;
}

 void MPU9150::start_gyro_calibration(void){
	gyro_calibration_sum[0] = 0;
	gyro_calibration_sum[1] = 0;
	gyro_calibration_sum[2] = 0;
	gyro_calibration_points = 0;
	gyro_calibrating = true;

	return;
}



void MPU9150::process_gyro_calibration(){
	gyro_calibration_sum[0] += last_gyro_values[0];
	gyro_calibration_sum[1] += last_gyro_values[1];
	gyro_calibration_sum[2] += last_gyro_values[2];
	gyro_calibration_points++;
}

 void MPU9150::stop_gyro_calibration(void){

	 if(!gyro_calibrating) return;
	//Add the new offsets:
	gyro_calibration.offset_x = (gyro_calibration_sum[0]+ gyro_calibration_points*gyro_calibration.offset_x) / gyro_calibration_points;
	gyro_calibration.offset_y = (gyro_calibration_sum[1]+ gyro_calibration_points*gyro_calibration.offset_y) / gyro_calibration_points;
	gyro_calibration.offset_z = (gyro_calibration_sum[2]+ gyro_calibration_points*gyro_calibration.offset_z) / gyro_calibration_points;

	gyro_calibrating = false;
}




/***************************************************
 * 					PRIVATE METHODS
 ***************************************************/

bool MPU9150::setup(){
	uint8_t data[6];

	// Reset device
	data[0] = 1<<MPU9150_PWR1_DEVICE_RESET_BIT;
	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_PWR_MGMT_1, 1, data))
		return -1;

	Time.delay_ms(100);

	// Power up device (this should happen no matter what, but just to be sure)
	data[0] = 0;
	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_PWR_MGMT_1, 1, data))
		return -1;

	// Set Gyroscope FSR to 2000 degree per second
	if(set_gyro_fsr(2000))
		return -1;

	// Set Accel FSR to 8g
	if(set_accel_fsr(8))
		return -1;

	// Set digital lowpass filter
	if(set_lpf(42))
		return -1;

	// Set gyro/acc sampling rate to 1000 (no divider)
	if(set_sample_rate(1000))
		return -1;

	// Disable interrupt
	data[0] = 0;
	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_INT_ENABLE, 1, data))
		return -1;

	// Setup Compass
	if(setup_compass())
		return -1;

	if(set_compass_sample_rate(100))
		return -1;

	// Enable sensors
	if(set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS))
		return -1;

	return 0;
}

int8_t MPU9150::set_gyro_fsr(uint16_t fsr){
	uint8_t data;

	switch (fsr) {
		case 250:
			data = MPU9150_GYRO_FS_250 << 3;
			break;
		case 500:
			data = MPU9150_GYRO_FS_500 << 3;
			break;
		case 1000:
			data = MPU9150_GYRO_FS_1000 << 3;
			break;
		case 2000:
			data = MPU9150_GYRO_FS_2000 << 3;
			break;
		default:
			return -1;
	}

	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_GYRO_CONFIG, 1, &data))
		return -1;

	return 0;
}

int8_t MPU9150::set_accel_fsr(uint8_t fsr)
{
	uint8_t data;

	switch (fsr) {
		case 2:
			data = MPU9150_ACCEL_FS_2 << 3;
			break;
		case 4:
			data = MPU9150_ACCEL_FS_4 << 3;
			break;
		case 8:
			data = MPU9150_ACCEL_FS_8 << 3;
			break;
		case 16:
			data = MPU9150_ACCEL_FS_16 << 3;
			break;
		default:
			return -1;
	}

	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_ACCEL_CONFIG, 1, &data))
		return -1;

	return 0;
}

int8_t MPU9150::set_sample_rate(uint16_t rate)
{
	if (rate < 4) rate = 4;
	else if (rate > 1000) rate = 1000;

	uint8_t data = 1000 / rate - 1;
	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_SMPLRT_DIV, 1, &data))
		return -1;

	if(rate > 100) set_compass_sample_rate(100);
	else set_compass_sample_rate(rate);

	// Automatically set LPF to 1/2 sampling rate.
	set_lpf(rate/2);
	return 0;

}

int8_t MPU9150::set_sensors(uint8_t sensors)
{
	unsigned char data;
	unsigned char user_ctrl;

	if (sensors & INV_XYZ_GYRO)
		data = 1; // CLK_PLL
	else if (sensors)
		data = 0;
	else
		data = BIT_SLEEP;

	Time.delay_ms(100);
	if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_PWR_MGMT_1, 1, &data)) {
		return -1;
	}

	data = 0;
	if (!(sensors & INV_X_GYRO))
		data |= BIT_STBY_XG;
	if (!(sensors & INV_Y_GYRO))
		data |= BIT_STBY_YG;
	if (!(sensors & INV_Z_GYRO))
		data |= BIT_STBY_ZG;
	if (!(sensors & INV_XYZ_ACCEL))
		data |= BIT_STBY_XYZA;

	Time.delay_ms(100);
	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_PWR_MGMT_2, 1, &data))
		return -1;

	if (sensors && (sensors != INV_XYZ_ACCEL))
		// Latched interrupts only used in LP accel mode.
		if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_INT_PIN_CFG, 1, 0))
			return -1;


	if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_USER_CTRL, 1, &user_ctrl))
		return -1;
	// Handle AKM power management.
	if (sensors & INV_XYZ_COMPASS) {
		data = AKM_SINGLE_MEASUREMENT;
		user_ctrl |= BIT_AUX_IF_EN;
	} else {
		data = AKM_POWER_DOWN;
		user_ctrl &= ~BIT_AUX_IF_EN;
	}
	user_ctrl &= ~BIT_DMP_EN;


	Time.delay_ms(100);
	if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV1_DO, 1, &data))
		return -1;

	// Enable/disable I2C master mode.
	Time.delay_ms(100);
	if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_USER_CTRL, 1, &user_ctrl))
		return -1;

	Time.delay_ms(50);
	return 0;
}

int8_t MPU9150::set_lpf(uint8_t lpf){
    uint8_t data;

    if (lpf >= 188)
        data = MPU9150_DLPF_BW_188;
    else if (lpf >= 98)
        data = MPU9150_DLPF_BW_98;
    else if (lpf >= 42)
        data = MPU9150_DLPF_BW_42;
    else if (lpf >= 20)
        data = MPU9150_DLPF_BW_20;
    else if (lpf >= 10)
        data = MPU9150_DLPF_BW_10;
    else
        data = MPU9150_DLPF_BW_5;

	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_CONFIG, 1, &data))
		return -1;

	return 0;
}


int8_t MPU9150::set_compass_sample_rate(uint16_t rate){
    if (rate > 100)
        return -1;

    // The '1000' should be the the sample rate set with MPU9150_SetSampleRate()
    uint8_t div = 1000 / rate - 1;

	if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV4_CTRL, 1, &div))
		return -1;

    return 0;
}

int8_t MPU9150::setup_compass(void){
	 unsigned char data[4], akm_addr;

		set_bypass(1);

	    // Find compass. Possible addresses range from 0x0C to 0x0F.
	    for (akm_addr = 0x0C; akm_addr <= 0x0F; akm_addr++) {
	        int result;
	        result = i2c.read_register8(akm_addr, AKM_REG_WHOAMI, 1, data);
	        if (result && (data[0] == AKM_WHOAMI))
	            break;
	    }

	    if(akm_addr > 0x0F)
	    	return -1;

	    data[0] = AKM_POWER_DOWN;
	    if (!i2c.write_register8(akm_addr, AKM_REG_CNTL, 1, data))
	        return -2;
	    Time.delay_ms(1);

	    data[0] = AKM_FUSE_ROM_ACCESS;
	    if (!i2c.write_register8(akm_addr, AKM_REG_CNTL, 1, data))
	        return -3;
	    Time.delay_ms(1);


	    // Get sensitivity adjustment data from fuse ROM. (Not needed)
	    uint16_t mag_sens_adj[3];
	    if (!i2c.read_register8(akm_addr, AKM_REG_ASAX, 3, data))
	        return -4;
	    mag_sens_adj[0] = (long)data[0] + 128;
	    mag_sens_adj[1] = (long)data[1] + 128;
	    mag_sens_adj[2] = (long)data[2] + 128;


	    data[0] = AKM_POWER_DOWN;
	    if (!i2c.write_register8(akm_addr, AKM_REG_CNTL, 1, data))
	        return -5;
	    Time.delay_ms(1);

		set_bypass(0);

	    // Set up master mode, master clock, and ES bit.
	    data[0] = 0x40;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_MST_CTRL, 1, data))
	        return -6;

	    // Slave 0 reads from AKM data registers.
	    data[0] = BIT_I2C_READ | akm_addr;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV0_ADDR, 1, data))
	        return -7;

	    // Compass reads start at this register.
	    data[0] = AKM_REG_ST1;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV0_REG, 1, data))
	        return -8;

	    // Enable slave 0, 8-byte reads.
	    data[0] = BIT_SLAVE_EN | 8;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV0_CTRL, 1, data))
	        return -9;

	    // Slave 1 changes AKM measurement mode.
	    data[0] = akm_addr;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV1_ADDR, 1, data))
	        return -10;

	    // AKM measurement mode register.
	    data[0] = AKM_REG_CNTL;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV1_REG, 1, data))
	        return -11;

	    // Enable slave 1, 1-byte writes.
	    data[0] = BIT_SLAVE_EN | 1;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV1_CTRL, 1, data))
	        return -12;

	    // Set slave 1 data.
	    data[0] = AKM_SINGLE_MEASUREMENT;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_SLV1_DO, 1, data))
	        return -13;

	    // Trigger slave 0 and slave 1 actions at each sample.
	    data[0] = 0x03;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_I2C_MST_DELAY_CTRL, 1, data))
	        return -14;

	    // For the MPU9150, the auxiliary I2C bus needs to be set to VDD.
	    data[0] = BIT_I2C_MST_VDDIO;
	    if (!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_YG_OFFS_TC, 1, data))
	        return -15;

	    return 0;
}

int8_t MPU9150::set_bypass(uint8_t bypass_on){
	uint8_t tmp;
		if(!i2c.read_register8(MPU9150_ADDRESS, MPU9150_RA_USER_CTRL, 1, &tmp))
			return -1;

		if(bypass_on){
			tmp &= ~BIT_AUX_IF_EN;
			if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_USER_CTRL, 1, &tmp))
				return -1;
			Time.delay_ms(3);
			tmp = BIT_BYPASS_EN | BIT_ACTL;
			if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_INT_PIN_CFG, 1, &tmp))
				return -1;
		} else {
			tmp |= BIT_AUX_IF_EN;
			if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_USER_CTRL, 1, &tmp))
				return -1;
			Time.delay_ms(3);
			tmp = BIT_ACTL;
			if(!i2c.write_register8(MPU9150_ADDRESS, MPU9150_RA_INT_PIN_CFG, 1, &tmp))
				return -1;
		}

		return 0;
}

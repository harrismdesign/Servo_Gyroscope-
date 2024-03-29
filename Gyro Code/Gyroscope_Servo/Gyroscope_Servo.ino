
//Adapted by @harrism_design from Michael Schoeffler

#include <Servo.h>

#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data


char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;

}
Servo servox;
Servo servoy;



void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //servox.attach(2);
  //servoy.attach(3);
  pinMode(8,INPUT);
  

  
  
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable

  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
 
 
 int round_x = (round(gyro_x/1000))*1000;  //filters out data by rounding to nearest 100 
 int round_y = (round(gyro_y/1000))*1000;
 int x_pos = map(round_x,-15900,16800,0,180); //maps for a useful number for servo
 int y_pos = map(round_y,-16660,15900,0,180);
  

  Serial.print(" | Raw X = "); Serial.print(convert_int16_to_str(gyro_x));
  Serial.print(" | Raw Y = "); Serial.print(convert_int16_to_str(gyro_y));
  
  Serial.print("  | X angle position = "); Serial.print(convert_int16_to_str(x_pos));
  Serial.print("  | Y angle position = "); Serial.print(convert_int16_to_str(y_pos));

  servox.write(x_pos); //the 2 defined servos move to the coresponding position
  servoy.write(y_pos);

  

  delay(100);
}

 

/*****************************************************************************/
//	Function:    Get the accelemeter of X/Y/Z axis and print out on the 
//					serial monitor.
//  Hardware:    3-Axis Digital Accelerometer(��16g)
//	Arduino IDE: Arduino-1.0
//	Author:	 Frankie.Chu		
//	Date: 	 Jan 11,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include <TonyS_X1.h>
#include <TonyS_X1_ExternalModule.h>


TONY_ADXL345 TonyAc; 

void setup(){
  
  Serial.begin(9600);
  Tony.begin();  //----  begin Library
  TonyAc.powerOn();

  //set activity/ inactivity thresholds (0-255)
  TonyAc.setActivityThreshold(75); //62.5mg per increment
  TonyAc.setInactivityThreshold(75); //62.5mg per increment
  TonyAc.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  TonyAc.setActivityX(1);
  TonyAc.setActivityY(1);
  TonyAc.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  TonyAc.setInactivityX(1);
  TonyAc.setInactivityY(1);
  TonyAc.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  TonyAc.setTapDetectionOnX(0);
  TonyAc.setTapDetectionOnY(0);
  TonyAc.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  TonyAc.setTapThreshold(50); //62.5mg per increment
  TonyAc.setTapDuration(15); //625us per increment
  TonyAc.setDoubleTapLatency(80); //1.25ms per increment
  TonyAc.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  TonyAc.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  TonyAc.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  TonyAc.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  TonyAc.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  TonyAc.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  TonyAc.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  TonyAc.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  TonyAc.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  TonyAc.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  TonyAc.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  TonyAc.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  TonyAc.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}

void loop(){
  
 //Boring accelerometer stuff   
  int x,y,z;  
  TonyAc.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
  // Output x,y,z values 
  Serial.print("values of X , Y , Z: ");
  Serial.print(x);
  Serial.print(" , ");
  Serial.print(y);
  Serial.print(" , ");
  Serial.println(z);
  
  double xyz[3];
  double ax,ay,az;
  TonyAc.getAcceleration(xyz);
  ax = xyz[0];
  ay = xyz[1];
  az = xyz[2];
  Serial.print("X=");
  Serial.print(ax);
    Serial.println(" g");
  Serial.print("Y=");
  Serial.print(ay);
    Serial.println(" g");
  Serial.print("Z=");
  Serial.print(az);
    Serial.println(" g");
  Serial.println("**********************");
  delay(500);
 
}
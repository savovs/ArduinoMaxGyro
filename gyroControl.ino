/******************************************************************************
Audio & Interactivity project by Vlady, Artur & Mantas - Glasgow Caledonian University


This project and all libraries in it are released under the [MIT License](http://opensource.org/licenses/MIT).

******************************************************************************/

#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include "six_axis_comp_filter.h"

LSM6DS3 myIMU; // Default constructor for the motion sensor


// (below) Constructor for the Complementary Filter CompSixAxis(deltaTime, tau)

// deltaTime - The time delta update period expressed in seconds. This value
//	should be set to how often the filter is updated with new values,
//	which should be on a regular interval. The smaller this value is,
//	the less noise there will be on the comp. filter's output.
//
// tau - Max allowable time until gyro drifts too far and comp. filter
//      shifts its weight to the accelerometer expressed in seconds. This
//      value is usually based on the drift rate of the gyro. For example,
//      if the gyro drifts at a rate of 0.5 degrees per second and the
//      tolerance of max allowable drift is 1 degree, then tau should be set
//      to 2 seconds as it will take 2 seconds to drift 1 degree. The larger
//      this value is, the less noise there will be on the comp. filter's
//      output. In exchange, it will drift further from the correct angular
//      position.
CompSixAxis CompFilter(0.1, 2);

float accelX, accelY, accelZ, // variables to store sensor values
	  gyroX, gyroY, gyroZ,
	  xAngle, yAngle;       // variables for angles calculated by filter

int potPin1 = A0, // Pins in which the potentiometers are connected
	potPin2 = A2,
	pot1 = 0,     // Pot values are stored here
	pot2 = 0,
	buttonPin = 4, // Button pin number
	inByte = 0,    // Max MSP - Incoming serial byte that will establish connection (handshaking)
	button;

void setup() {

	// Set up pins for the potentiometers and button
	pinMode(buttonPin, INPUT);
	pinMode(potPin1, INPUT);
	pinMode(potPin2, INPUT);

	// Start serial communication
	Serial.begin(9600);
	while (!Serial) {
		// wait for serial port to connect. Needed for native USB port only
	}

	//Call .begin() to configure the IMU (Inertial Measurement Unit)
	myIMU.begin();

	// send a byte to establish contact until receiver responds
	establishContact();

	// Check if MaxMSP has ordered the values correctly
	testMaxMspValues();

}


void loop()
{

	if (Serial.available() > 0) {
		// get incoming byte:
		inByte = Serial.read();

		//	Get all motion sensor parameters:
		accelX = myIMU.readFloatAccelX();
		accelY = myIMU.readFloatAccelY();
		accelZ = myIMU.readFloatAccelZ();

		gyroX = myIMU.readFloatGyroX();
		gyroY = myIMU.readFloatGyroY();
		gyroZ = myIMU.readFloatGyroZ();


		// Convert these values into something useful, using the Complementary Filter

		CompFilter.CompAccelUpdate(accelX, accelY, accelZ);
		CompFilter.CompGyroUpdate(gyroX, gyroY, gyroZ);
		CompFilter.CompUpdate();
		CompFilter.CompStart();

		// Get angle relative to X axis
		xAngle = CompFilter.CompRadiansToDegrees(
			CompFilter.compAngleX
		);

		// Map it between 0 and 255
		if (xAngle <= 180) {
			xAngle = map(xAngle, 0, 180, 0, 255);
		} else {
			xAngle = map(xAngle, 181, 359, 255, 0);
		}


		// Get angle relative to Y axis
		yAngle = CompFilter.CompRadiansToDegrees(
			CompFilter.compAngleY
		);

		// Map it between 0 and 255
		if (yAngle <= 180) {
			yAngle = map(yAngle, 0, 180, 0, 255);
		} else {
			yAngle = map(yAngle, 181, 359, 255, 0);
		}


		// Read potentiometes
		pot1 = analogRead(potPin1);
		pot1 = map(pot1, 0, 1023, 255, 0); // Map to 0 - 255 (Max understands this range only), default is 0 - 679

		pot2 = analogRead(potPin2);
		pot2 = map(pot2, 0, 1023, 255, 0);

		button = digitalRead(buttonPin);
		button = map(button, 0, 1, 0, 255);

		Serial.write((int)xAngle);
		Serial.write((int)yAngle);
		Serial.write(pot1);
		Serial.write(pot2);
		Serial.write(button);

		// How often we get updates from the sensors
		delay(10);
	}
}

void establishContact() {
	while (Serial.available() <= 0) {
		Serial.print('A');	 // send a capital A
		delay(300);
	}
}


// Writes numbers to serial port in order to see if MaxMSP orders them correctly
void testMaxMspValues() {
	Serial.write(1);
	delay(5);
	Serial.write(2);
	delay(5);
	Serial.write(3);
	delay(5);
	Serial.write(4);
	delay(5);
	Serial.write(5);
	delay(1000);
}

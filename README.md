This is an example of a wireless motion controller for [Max MSP](https://cycling74.com/products/max/)

It uses a motion controller, a Bluetooth receiver and a couple of potentiometers:
![](http://i.imgur.com/n5XX8z5.png)



## Guide:

### 1. [LSM6DS3 sensor](https://www.sparkfun.com/products/13339)

### 2. A Bluetooth module, the one in the example is a [ZS-040](http://www.domirobot.com/pictures/201511810388__mg_5293.jpg)

### 3. Install the [LSM6DS3 Arduino Library](https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library)

### 4. Install the [6 DOF Complementary Filter](https://github.com/tcleg/Six_Axis_Complementary_Filter)
The filter converts gyro data to angles like this: ![](http://www.pieter-jan.com/images/equations/CompFilter_Eq.gif)

serial_mpu9150
==============

Using serial library and FreeIMU library with the sensors MPU9150 and Adafruit MPL115A2, integrated with ROS. Got the processing code found inside the FreeIMU library and translate Java coding to C++ so it could be used with the serial library.

Thanks to Gary Servin who made a simple example using the serial library with ROS.

Links bellow:

Serial Library: http://wjwwood.io/serial/

Gary Servin's source code: https://github.com/garyservin/serial-example

Adafruit MPL115A2: https://github.com/adafruit/Adafruit_MPL115A2

FreeIMU library: https://github.com/mjs513/FreeIMU-Updates

In order to install the serial library in ROS, run the following code in console:

    $sudo apt-get install ros-<distro>-serial


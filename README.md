serial_mpu9150
==============

Using serial library (http://wjwwood.io/serial/) and FreeIMU library with the sensors MPU9150 and Adafruit MPL115A2 (https://github.com/adafruit/Adafruit_MPL115A2), integrated with ROS. Got the processing code found inside the FreeIMU library and modified from java to c++ so it could be used with the serial library.


### Install Instructions

Get the code: 

    git clone https://github.com/douglaskataki/serial_mpu9150

Change directory (or the your src on your catkin_workspace):

    cd ~/catkin/src
    
Make a directory:

    mkdir serial_mpu
    
Change to the new dir:

    cd serial_mpu
    
Build:

    make

Build and run the tests:

    make test

Install:

    make install

Uninstall:

    make uninstall


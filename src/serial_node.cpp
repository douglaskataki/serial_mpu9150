#include "serial_mpu9150.h"
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <iostream>
#define BAUDRATE 57600 //Define your baudrate here!

using std::string;
int cont;
static int burst=50; //must be the same as the arduino code
serial::Serial ser; //Serial object created

void write_callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO_STREAM("Writing to serial port" << msg->data);
    ser.write(msg->data);
}

int main (int argc, char** argv){
    // int cont;
    ros::init(argc, argv, "serial_example_node");
    ros::NodeHandle n;

    //set a subscriber to write data to the node
    ros::Subscriber write_sub = n.subscribe("write", 1000, write_callback);

    //set a publisher to read data from the node
    ros::Publisher read_pub = n.advertise<std_msgs::String>("read", 1000);

    //Change here the serial port
    std::string serialPort = "/dev/ttyACM0";

    try{
        ser.setPort(serialPort);
        ser.setBaudrate(BAUDRATE);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
        
    }catch (serial::IOException& e){
        ROS_ERROR_STREAM("Unable to open port!");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port: "<<serialPort<<" was initialized");
    }
    else{
        return -1;
    }

    while(ser.available() == 0){
        ser.write("z");
        // my_sleep(20);
        // ser.write("z");
        ROS_INFO_STREAM(ser.available());
        my_sleep(500); //500ms
        cont=0;
        
    }
    
// 1AF4713F,B468FF3D,377A903E,C261DE3D,2B99DFC0,776A55BF,53FE7C42,797D8B3A,00000000,00000000,EC747C3E,BB5A973E,E083563E,E2214741,00000000,9FC1AC41,A0028DC2,00000000,
    //burst=28; for counter arduino = 28;
//2015C842,849BA541,72F77F3F,CA4352BC,479A153C,0A1D83BC,
//D0997F3F,27890CBD,DBF81EBD,81FAD43C,AA3FDBC0,424EF2C0,DCD17B42,797D8BBA,797D8BBA,00000000,C6818A3E,6EF1A53D,CA13E83D,511AA741,602BC442,F612A8C2,


    ros::Rate loop_rate(500);
 
    while(ros::ok()){
        ros::spinOnce();
        if(ser.available()){
            //ROS_INFO_STREAM("Reading from serial port");
            // ser.write("z");
            std_msgs::String result;
            //for string[18] -> args = 163
            //for string[6] -> args = 55
            //for string[16] -> args = 145
            //int a = result.data.find('\n');
            result.data = ser.read(145);
            ROS_INFO_STREAM("I heard:"<<result.data);
            read_pub.publish(result);
            ROS_INFO_STREAM("cont="<<cont);
            cont++;
            if(cont==burst){
                ser.write("z");
                // my_sleep(20);
                // ser.write("z");
                my_sleep(10); //100ms
                cont=0;
                
                //ROS_INFO_STREAM("HOPE!");
            }
        }
        loop_rate.sleep();

    }
    ros::spin();
}

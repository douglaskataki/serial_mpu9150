#include "serial_mpu9150.h"
#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <iostream>

using std::string;
int cont;
static int burst=50;
serial::Serial ser; //Serial object created

void write_callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO_STREAM("Writing to serial port" << msg->data);
    ser.write(msg->data);
}

int main (int argc, char** argv){
    // int cont;
    ros::init(argc, argv, "serial_example_node");
    if (argc != 3){ROS_ERROR("Problem! Serial port or Baud Rate! Set it at the launch file."); 
    return -1;
    };

    std::string serialPort = argv[1];
    int BaudRate = atoi(argv[2]);

    ros::NodeHandle n;

    //set a subscriber to write data to the node
    ros::Subscriber write_sub = n.subscribe("write", 1000, write_callback);

    //set a publisher to read data from the node
    ros::Publisher read_pub = n.advertise<std_msgs::String>("serial_topic", 1000);

    //Change here the serial port
    

    try{
        ser.setPort(serialPort);
        ser.setBaudrate(BaudRate);
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
        ROS_INFO_STREAM(ser.available());
        my_sleep(1000); //1000ms
        cont=0;
        
    }
    
    ros::Rate loop_rate(30);
 
    while(ros::ok()){
        ros::spinOnce();
        if(ser.available()){
            std_msgs::String result;
            result.data = ser.read(145); //size of the string
            ROS_INFO_STREAM("I heard:"<<result.data);
            read_pub.publish(result);
            cont++;
            if(cont==burst){
                ser.write("z");
                my_sleep(10); //10ms
                cont=0;
            }
        }
        loop_rate.sleep();
    }
    ros::spin();
}

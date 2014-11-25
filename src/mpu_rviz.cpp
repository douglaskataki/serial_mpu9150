#include <ros/ros.h>
#include <std_msgs/String.h>
#include <visualization_msgs/Marker.h>
#include <tf/tf.h>
#include "serial_mpu9150.h"


float v[16];
float color[3];
float first = 10; //change here the initial value for the range of temperature
float end = 25;  //ending value for the range

//Put in here all the things that are needed to get values on time
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{ 
   //ROS_INFO("I heard:%s",msg->data.c_str());
   // GetData(msg->data.c_str(),&pr,&tmp,&x,&y,&z,&w);
   GetData(msg->data.c_str(),v);
   TmpToRGB(v[13],color,first,end);
   // Rot(x,y,z,w,&nx,&ny,&nz,&nw,ang);
   //ROS_INFO_STREAM("x,y,z,w="<<x<<","<<y<<","<<z<<","<<w);
}

int main(int argc, char **argv){
  
  ros::init(argc, argv, "serial_imu");
  
  ros::NodeHandle n;
  
  ros::Subscriber sub = n.subscribe("read", 1000, chatterCallback); 

  ros::Rate r(5);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 2);
  
  //Set our shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;
  uint32_t shape_2 = visualization_msgs::Marker::ARROW;

  while(ros::ok()){
      ros::spinOnce();
      visualization_msgs::Marker marker;
      visualization_msgs::Marker arrow;
    
      // Set the frame ID and timestamp.  See the TF tutorials for information on these.
      marker.header.frame_id = "/my_frame";
      marker.header.stamp = ros::Time::now();

      arrow.header.frame_id = "/my_frame";
      arrow.header.stamp = ros::Time::now();
    
      // Set the namespace and id for this marker.  This serves to create a unique ID
      // Any marker sent with the same namespace and id will overwrite the old one
      marker.ns = "basic_shapes";
      marker.id = 0;

      arrow.ns = "arrow";
      arrow.id = 1;
      
      //Cube and Arrow shapes
      marker.type = shape;
      arrow.type = shape_2;

      // Set the marker action.  Options are ADD and DELETE
      marker.action = visualization_msgs::Marker::ADD;
      arrow.action = visualization_msgs::Marker::ADD;

      // Set the pose of the marker.  This is a full 6DOF pose relative 
      //to the frame/time specified in the header
      marker.pose.position.x = 0;
      marker.pose.position.y = 0;
      marker.pose.position.z = 0;

      arrow.pose.position.x = 0;
      arrow.pose.position.y = 0;
      arrow.pose.position.z = 0;

      // Got the quaternions in serial bus
      marker.pose.orientation.x = v[0];
      marker.pose.orientation.y = v[1];
      marker.pose.orientation.z = v[2];
      marker.pose.orientation.w = v[3];

      //rotate the quaternions so the arrow is always perpendicular
      //to the base

      arrow.pose.orientation.x = marker.pose.orientation.x;
      arrow.pose.orientation.y = marker.pose.orientation.y;
      arrow.pose.orientation.z = marker.pose.orientation.z;
      arrow.pose.orientation.w = marker.pose.orientation.w;
      
      // Set the scale of the marker 
      marker.scale.x = 0.2;
      marker.scale.y = 0.4;
      marker.scale.z = 0.7;

      // marker.scale.x = 80.0/pr;
      // marker.scale.y = .10;
      // marker.scale.z = .10;

      arrow.scale.x = 80.0/v[14];
      arrow.scale.y = .10;
      arrow.scale.z = .10;

      // Set the color -- be sure to set alpha to something non-zero!

      //Cube color
      marker.color.r = color[0];
      marker.color.g = color[1];
      marker.color.b = color[2];
      marker.color.a = 1.0;

      //Arrow color
      arrow.color.r = 1.0f;
      arrow.color.g = 1.0f;
      arrow.color.b = 1.0f;
      arrow.color.a = 1.0;

      marker.lifetime = ros::Duration();
      arrow.lifetime = ros::Duration();

      // Publish the marker
      marker_pub.publish(marker);
      marker_pub.publish(arrow);

      r.sleep();
  }
  ros::spin();
  return 0;
}
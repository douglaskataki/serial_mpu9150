#include <ros/ros.h>
#include <std_msgs/String.h>
#include <visualization_msgs/Marker.h>
#include <tf/tf.h>
#include "serial_mpu9150.h"

float color[3],v[16]; //rgb, get data
float first = 15; //change here the initial value for the range of temperature
float end = 25;  //ending value for the range
float q[]={0,0,0,0}; //quaternion
float hq[]={0,0,0,0}; //another quaterion
float qq[]={0,0,0,0};
float factor=80.0f; //set a good factor here
float pos[]={0,0,0}; //Set here the position
float cube_color[]={0,0,1}; //blue color (r,g,b)
float arrow_color[]={0,1,1}; //ciano color 

//Put in here all the things that are needed to get values on time
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{ 
  GetData(msg->data.c_str(),v);
  BlueToRed(color,v[13],first,end);
  setToQuat(v[0],v[1],v[2],v[3],q);
}

//Function that get parameters to set a marker
void setMarker(visualization_msgs::Marker *marker, std::string frame_id, std::string ns,int id,uint32_t shape,
              float position[3], float quaternion[4], float color[3], float alpha){
  
  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker->header.frame_id = frame_id;
  marker->header.stamp = ros::Time::now();
  
  // Set the namespace and id for this marker.  This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker->ns = ns;
  marker->id = id;

  //Shape
  marker->type = shape;

  // Set the marker action.  Options are ADD and DELETE
  marker->action = visualization_msgs::Marker::ADD;

  //set the position
  marker->pose.position.x = position[0];
  marker->pose.position.y = position[1];
  marker->pose.position.z = position[2];

  // Got the quaternions in serial bus
  marker->pose.orientation.x = quaternion[0];
  marker->pose.orientation.y = quaternion[1];
  marker->pose.orientation.z = quaternion[2];
  marker->pose.orientation.w = quaternion[3];

  marker->color.r = color[0]; //red
  marker->color.g = color[1]; //green
  marker->color.b = color[2]; //blue
  marker->color.a = alpha;
}

int main(int argc, char **argv){
  
  ros::init(argc, argv, "serial_imu");
  
  ros::NodeHandle n;
  
  ros::Subscriber sub = n.subscribe("serial_topic", 1000, chatterCallback); 

  ros::Rate r(20);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
  
  //Set our shape type to be a cube
  uint32_t cube_shape = visualization_msgs::Marker::CUBE;
  uint32_t arrow_shape = visualization_msgs::Marker::ARROW;

  int i=0;

  while(ros::ok()){
      ros::spinOnce();
      visualization_msgs::Marker cube,arrow,acc;

      //some calculations to rotate the quaternion

      // if(i<100){
        quatConjugate(q,hq);
        // i++;
      // }
      quatProd(q,hq,qq);

      //marker, frame_id, ns, id, type, position, direction, color, alpha(non zero)
      setMarker(&cube,"my_frame","cube",0,cube_shape,pos,q,cube_color,1.0);
      setMarker(&arrow,"my_frame","arrow",1,arrow_shape,pos,q,color,1.0);
      setMarker(&acc,"my_frame","acc",2,arrow_shape,pos,qq,arrow_color,1.0);

      // Set the scale of the marker 
      cube.scale.x = 0.3;
      cube.scale.y = 0.7;
      cube.scale.z = 0.1;

      arrow.scale.x = factor/v[14]; //arrow length
      arrow.scale.y = .10; //arrow width
      arrow.scale.z = .10; //arrow height

      acc.scale.x = 0.5;
      acc.scale.y = 0.10;
      acc.scale.z = 0.10;

      cube.lifetime = ros::Duration();
      arrow.lifetime = ros::Duration();
      acc.lifetime = ros::Duration();

      // Publish the marker
      marker_pub.publish(cube);
      marker_pub.publish(arrow);
      marker_pub.publish(acc);
      r.sleep();
  }
  ros::spin();
  return 0;
}

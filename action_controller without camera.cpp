#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Range.h>

int id = 0;
ros::Publisher action_pub;
geometry_msgs::Twist set_vel;

float distFL = 0;
float distFR = 0;

void distFL_callback(const sensor_msgs::Range &range) 
{
   	distFL = range.range;
	if(distFL > 0.7 )
	{
		set_vel.linear.x = 0.5;
   		set_vel.angular.z = 0;
	}
	else if(distFL > 0.2 )
	{
		set_vel.linear.x = 0.3;
   		set_vel.angular.z = 0;
	}
        else 
	{
		set_vel.linear.x = 0.1;
   		set_vel.angular.z = 0;
	}

	action_pub.publish(set_vel);
}



int main(int argc, char **argv)
{

   ros::init(argc, argv, "action_controller");
   ros::NodeHandle n("~");
   ros::Rate loop_rate(50);

   ros::Subscriber distFL_sub = n.subscribe("/range/fl", 1, distFL_callback);
   //ros::Subscriber distFR_sub = n.subscribe("/range/fr", 1, distFR_callback);
 
   action_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
   set_vel.linear.x = 0;
   set_vel.linear.y = 0;
   set_vel.linear.z = 0;
   set_vel.angular.x = 0;
   set_vel.angular.y = 0;
   set_vel.angular.z = 0;

   while (ros::ok())
   {
   
      ros::spinOnce();
      loop_rate.sleep();
   }
}
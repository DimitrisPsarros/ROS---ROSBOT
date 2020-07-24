
#include "tf/tf.h"

#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"

#include "sensor_msgs/BatteryState.h"
#include "std_msgs/Bool.h"
#include "sensor_msgs/Range.h"
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/Vector3.h"





int id = 0;
ros::Publisher action_pub;
geometry_msgs::Twist set_vel;




int main(int argc, char **argv)
{

   ros::init(argc, argv, "action_controller");
   ros::NodeHandle n("~");
   ros::Rate loop_rate(50);

   action_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
   set_vel.linear.x = 0;
   set_vel.linear.y = 0;
   set_vel.linear.z = 0;
   set_vel.angular.x = 0;
   set_vel.angular.y = 0;
   set_vel.angular.z = 0;
   while (ros::ok())
   {
      set_vel.linear.x = 0;
      action_pub.publish(set_vel);
      ros::spinOnce();
      loop_rate.sleep();
   }
}
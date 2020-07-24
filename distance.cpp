#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/String.h>

ros::Publisher distance_pub;


bool firstIt = true;
double previousX, previousY;
double currentX, currentY;
double x, y;
double squaredVals, sqrtVal, distanceCalculated, finalDistance;
std_msgs::String msg;


void calculateDistance()
{
	x = previousX - currentX;
	y = previousY - currentY;
	squaredVals = pow(x, 2) + pow(y, 2);
	sqrtVal = sqrt(squaredVals);	
	
	finalDistance = finalDistance + sqrtVal;
	
	previousX = currentX;
	previousY = currentY;

	
    	std::stringstream ss;
    	ss << finalDistance ;
    	msg.data = ss.str();
	ROS_INFO("Sending...%s", msg.data.c_str());

	std::cout << " -------------\n" << "| distance: " << finalDistance << " |\n" << " -------------" << std::endl;
}

void distance_callback(const geometry_msgs::PoseStampedPtr &pose)
{	
	if(firstIt)
	{	
		previousX = (int)(pose->pose.position.x * 1000.0)/1000.0;
		previousY = (int)(pose->pose.position.y * 1000.0)/1000.0;
		firstIt = false;
	}
	
	currentX = (int)(pose->pose.position.x * 1000.0)/1000.0;
	currentY = (int)(pose->pose.position.y * 1000.0)/1000.0;
	
	calculateDistance();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "distance");
    ros::NodeHandle n("~");
    ros::Subscriber distancePose_sub = n.subscribe("/pose", 1, distance_callback);
    ros::Publisher distance_pub = n.advertise<std_msgs::String>("dist", 1000);
    ros::Rate loop_rate(500);
    while (ros::ok())
    {
	distance_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
}
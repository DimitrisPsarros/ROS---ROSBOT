#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/UInt8.h>
#include <std_srvs/Empty.h>
#include <sensor_msgs/LaserScan.h>
#include <ros/time.h>

int i = 0;
int id = 0;


int counter=0;
int count=0;
int f_count=10;
float difference =0;

double start_time=0;

ros::Publisher action_pub;
geometry_msgs::Twist set_vel;

float init_dir=0;

float distFL = 0;
float distFR = 0;
float distRL = 0;
float distRR = 0;
float direction =0;



int first_time=0;

float liddarrange = 0;

bool print_b;
ros::Publisher brightness_pub;
int frames_passed = 0;

tf::Transform transform;
tf::Quaternion q;
float deg2rad = 3.14159/180;

void imu_callback(const geometry_msgs::Vector3 &imu)
{
    static tf::TransformBroadcaster br;
    q = tf::createQuaternionFromRPY(imu.x * deg2rad, imu.y * deg2rad, imu.z * deg2rad);

    if(first_time==0){
        init_dir=imu.z;   
        first_time=1;
    } 

    direction= imu.z ;
    std::cout << imu.z << std::endl ;
   
 
    transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
    transform.setRotation(q);

    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", "orientation"));
}

void imageCallback(const sensor_msgs::ImageConstPtr &image)
{
   long long sum = 0;
   for (int value : image->data)
   {
      sum += value;
   }
   int avg = sum / image->data.size();
   if (print_b)
   {
      std::cout << "Brightness: " << avg << std::endl;
   }
   std_msgs::UInt8 brightness_value;
   brightness_value.data = avg;
   brightness_pub.publish(brightness_value);
   frames_passed++;
}


//void lidarrange_callback( const sensor_msgs::LaserScan &ranges){
//float lidar[360];
//lidar[360] = ranges.ranges;

//float count = ranges -> ranges[90];

//float array = ranges.ranges[1];

//std::cout <<  array; 
		
//	if ( array < 0.8) {
//	set_vel.linear.x = 0.1;
//	}
//	if ( array >= 0.8) {
//	set_vel.linear.x = 0.0;
//	}
//	action_pub.publish(set_vel);
//}


void distFL_callback(const sensor_msgs::Range &range) {
	
   	distFL = range.range;
	
	if(distFL >= 0.6 && distFR >= 0.6 ){   
                
                difference = direction-init_dir;

                if( direction > init_dir +10 ){
		    std::cout << " reorientating 1"<< std::endl;
			//while( difference > 3){
                        
			     set_vel.linear.x = 0.25;
			     set_vel.angular.z = -1.5;
			     action_pub.publish(set_vel);
			//}
                }else if(  direction < init_dir -10 ) {
		    std::cout << " reorientating 2"<< std::endl;
			//while( difference > -2){
                        
			     set_vel.linear.x  = 0.25;
			     set_vel.angular.z = 1.5;
			     action_pub.publish(set_vel);
			//}
		}else {
                         set_vel.linear.x = 0.35;
   		         set_vel.angular.z = 0;
                         std::cout << " q"<< std::endl;
		}

                std::cout << " RPY initial " << init_dir  << std::endl;
                std::cout << " RPY current " << direction << std::endl;
	        std::cout <<"straight " << std::endl;
         
	}



        
        if(distFL <= 0.2 || distFR <= 0.2   ){
               if(distFL <= 0.2 && distFR <= 0.2){
		    if(distRL <= 0.2 || distRR <= 0.2   ){
		        set_vel.linear.x  = 0;
   		        set_vel.angular.z = 0;
	                std::cout <<"stop going back" << std::endl;
			
                    }
		    else{    
                         std::cout <<"Go back" << std::endl;
		         set_vel.linear.x = -0.2;
		         set_vel.angular.z = 0;
                    }
                }
	        else if (distFL<distFR){
                    set_vel.linear.x = 0.05;
		    set_vel.angular.z = -1.5;
                }
                else if (distFL>distFR){
                    set_vel.linear.x = 0.05;
		    set_vel.angular.z = 1.5;
                }

	}
  
	if (distFL <= 0.6 && distFL > 0.2 && distFR <= 0.6 && distFR >0.2){
	    if( distFR > distFL && (distFR - distFL) >= 0.2  ){
              //while(count<f_count){
                for(int i=0; i<10000;i++){
		set_vel.linear.x = 0.3;
		set_vel.angular.z = -1.0;
                action_pub.publish(set_vel);
                }

		std::cout <<"big right " << std::endl;
              //}
	    }
	    else if( distFL > distFR && (distFL - distFR) >= 0.2) {
                for(int i=0; i<10000;i++){
	        set_vel.linear.x = 0.3;
	        set_vel.angular.z = 1.0;
		}
              
	        std::cout <<"big left " << std::endl;
	    }
	   else {
		set_vel.linear.x = 0.01;
		set_vel.angular.z = 1.5;

		std::cout <<"Skid left" <<std::endl;    // random
	   }

        }
       if (distFL <= 0.6 && distFL > 0.2 &&  distFR > 0.8   ){
                set_vel.linear.x = 0.3;
	        set_vel.angular.z = -1.0;
         
          
       }
       if (distFR <= 0.6 && distFR > 0.2 &&  distFL > 0.8    ){
                set_vel.linear.x  = 0.3;
	        set_vel.angular.z = 1.0;

         
       }
        action_pub.publish(set_vel);

}

void distFR_callback(const sensor_msgs::Range &range){

   	distFR = range.range;
}

void distRR_callback(const sensor_msgs::Range &range){

   	distRR = range.range;
}

void distRL_callback(const sensor_msgs::Range &range){

   	distRL = range.range;
}



int main(int argc, char **argv)
{
   ros::Time::init();

   ros::init(argc, argv, "action_controller");
   ros::NodeHandle n("~");
   ros::Rate loop_rate(100);
	

   //ros::Subscriber lidarrange_sub = n.subscribe("/scan", 1, lidarrange_callback);

   ros::Subscriber distFL_sub   = n.subscribe  ("/range/fl", 1, distFL_callback);
   ros::Subscriber distFR_sub   = n.subscribe  ("/range/fr", 1, distFR_callback);
   ros::Subscriber distRL_sub   = n.subscribe  ("/range/rl", 1, distRL_callback);
   ros::Subscriber distRR_sub   = n.subscribe  ("/range/rr", 1, distRR_callback);

   ros::Subscriber pose_sub = n.subscribe("/rpy", 1, imu_callback);

   ros::Subscriber sub = n.subscribe("/camera/rgb/image_raw", 10, imageCallback);
   n.param<bool>("print_brightness", print_b, false);
   brightness_pub = n.advertise<std_msgs::UInt8>("brightness", 1);
   ros::ServiceClient client = n.serviceClient<std_srvs::Empty>("/image_saver/save");
   std_srvs::Empty srv;
 
   action_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
   set_vel.linear.x = 0;
   set_vel.linear.y = 0;
   set_vel.linear.z = 0;
   set_vel.angular.x = 0;
   set_vel.angular.y = 0;
   set_vel.angular.z = 0;

   while (ros::ok())
   {
      
      if (frames_passed > 100)
      {
         frames_passed = 0;
         client.call(srv);
      }
   
      ros::spinOnce();
      loop_rate.sleep();
   }
}
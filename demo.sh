#!/bin/bash

/opt/husarion/tools/rpi-linux/ros-core2-client /dev/ttyCORE2 &
# rostopic pub -1 /reset_odom std_msgs/Bool true
roslaunch tutorial_pkg demo.launch

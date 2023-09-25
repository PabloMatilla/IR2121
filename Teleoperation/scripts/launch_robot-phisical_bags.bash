ssh ubuntu@169.254.XXX.YYY

source /opt/ros/foxy/setup.bash
export ROS_LOCALHOST_ONLY=1
export TURTLEBOT3_MODEL=burger

ros2 bag record /clock /odom /tf /tf_static

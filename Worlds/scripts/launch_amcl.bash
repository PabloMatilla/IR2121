#!/bin/bash
source /opt/ros/foxy/setup.bash
export ROS_LOCALHOST_ONLY=1
export TURTLEBOT3_MODEL=burger

ros2 launch amcl.launch.py \
  use_sim_time:=True \
  map:=/home/usuario/Documentos/GitHub/IR2121/Worlds/TD_n1_edited.yaml


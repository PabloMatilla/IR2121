#include <chrono>
#include <unistd.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

// Este codigo esta disenado para actuar en los aldrededores de la clase de Robotica.

using namespace std::chrono_literals;

double x, y;
double x_ang, y_ang, z_ang;

double pos_x_1, pos_x_2, pos_x_3, pos_x_4;
double pos_y_1, pos_y_2, pos_y_3, pos_y_4;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    // Obtener la velocidad x y y
    x = msg->twist.twist.linear.x;
    y = msg->twist.twist.linear.y;

    x_ang = msg->twist.twist.angular.x;
    y_ang = msg->twist.twist.angular.y;
    z_ang = msg->twist.twist.angular.z;

}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");

  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback);

  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);
  geometry_msgs::msg::PoseStamped goal_pose_message;
  rclcpp::WallRate loop_rate(500ms);

  bool flag1 = 1, flag2 = 0, flag3 = 0, flag4 = 0;

  std::cout << "Primer goal" << std::endl;
  goal_pose_message.pose.position.x = -7.95;
  goal_pose_message.pose.position.y = 3.12;
  goal_pose_message.pose.position.z = 0;
  goal_pose_message.pose.orientation.x = 1;
  goal_pose_message.pose.orientation.y = 1;
  goal_pose_message.pose.orientation.z = 0;
  goal_pose_message.pose.orientation.w = 1;
  publisher->publish(goal_pose_message);
  rclcpp::spin_some(node);
  loop_rate.sleep();
  sleep(1);

  while (rclcpp::ok() and flag1) {

    if ((x == 0.001 and y == 0.001)){
      std::cout << "Primer bucle" << std::endl;
      flag1 = 0;
      flag2 = 1;
    }
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }

  std::cout << "Segundo goal" << std::endl;
  goal_pose_message.pose.position.x = -15.69;
  goal_pose_message.pose.position.y = 32.3366;
  goal_pose_message.pose.position.z = 0;
  goal_pose_message.pose.orientation.x = 1;
  goal_pose_message.pose.orientation.y = 1;
  goal_pose_message.pose.orientation.z = 0;
  goal_pose_message.pose.orientation.w = 1;
  publisher->publish(goal_pose_message);
  rclcpp::spin_some(node);
  loop_rate.sleep();
  sleep(1);

  while (rclcpp::ok() and flag2) {

    if ((x == 0.001 and y == 0.001)){
      std::cout << "Segundo bucle" << std::endl;
      flag2 = 0;
      flag3 = 1;
    }
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }

  std::cout << "Tercer goal" << std::endl;
  goal_pose_message.pose.position.x = -8.74;
  goal_pose_message.pose.position.y = 41.89;
  goal_pose_message.pose.position.z = 0;
  goal_pose_message.pose.orientation.x = 1;
  goal_pose_message.pose.orientation.y = 1;
  goal_pose_message.pose.orientation.z = 0;
  goal_pose_message.pose.orientation.w = 1;
  publisher->publish(goal_pose_message);
  rclcpp::spin_some(node);
  loop_rate.sleep();
  sleep(1);

  while (rclcpp::ok() and flag3) {

    if ((x == 0.001 and y == 0.001)){
      std::cout << "Tercer bucle" << std::endl;
      flag3 = 0;
      flag4 = 1;
    }
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }

  std::cout << "Cuarto bucle" << std::endl;
  goal_pose_message.pose.position.x = -0.06;
  goal_pose_message.pose.position.y = 33.52;
  goal_pose_message.pose.position.z = 0;
  goal_pose_message.pose.orientation.x = 1;
  goal_pose_message.pose.orientation.y = 1;
  goal_pose_message.pose.orientation.z = 0;
  goal_pose_message.pose.orientation.w = 1;
  publisher->publish(goal_pose_message);
  rclcpp::spin_some(node);
  loop_rate.sleep();

  while (rclcpp::ok() and flag4) {

    if ((x == 0.001 and y == 0.001)){
      std::cout << "Cuarto bucle" << std::endl;
      flag4 = 0;
    }
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  

  rclcpp::shutdown();
  return 0;
}
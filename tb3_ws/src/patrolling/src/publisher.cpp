#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

using namespace std::chrono_literals;

double x_init, y_init, ang_init;
double x, y;
double ang;

void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    // Obtener la posición x y y
    x = msg->pose.pose.position.x;
    y = msg->pose.pose.position.y;

    // Obtener la orientación como cuaternión
    geometry_msgs::msg::Quaternion orientation = msg->pose.pose.orientation;

    // Convertir cuaternión a ángulos de Euler
    tf2::Quaternion quat;
    tf2::convert(orientation, quat);
    double roll, pitch, yaw;
    tf2::Matrix3x3(quat).getRPY(roll, pitch, yaw);

    // Obtener el ángulo en grados y mostrarlo
    ang = yaw * 180.0 / M_PI;

    std::cout << "\tx: " << x << std::endl;
    std::cout << "\ty: " << y << std::endl;
    std::cout << "\tAngulo: " << ang << std::endl;
}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");

  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback);
  /*
  node->declare_parameter("pose_goal", [1, 2, 3, 4]);
  double goal = node->get_parameter("goal_pose").get_parameter_value().get<list<int>>();
  */

  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);
  geometry_msgs::msg::PoseStamped goal_pose_message;
  rclcpp::WallRate loop_rate(500ms);

  while (rclcpp::ok()) {
    goal_pose_message.pose.position.x = 1;
    goal_pose_message.pose.position.y = 1;
    goal_pose_message.pose.position.z = 0;
    goal_pose_message.pose.orientation.x = 1;
    goal_pose_message.pose.orientation.y = 1;
    goal_pose_message.pose.orientation.z = 0;
    goal_pose_message.pose.orientation.w = 1;

    publisher->publish(goal_pose_message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}


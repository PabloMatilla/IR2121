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

double Vx, Vy;
double Vx_ang, Vy_ang, Vz_ang;

double x, y;
double ang;

void topic_callback1(const nav_msgs::msg::Odometry::SharedPtr msg) {
    // Obtener la velocidad x y y
    Vx = msg->twist.twist.linear.x;
    Vy = msg->twist.twist.linear.y;

    Vx_ang = msg->twist.twist.angular.x;
    Vy_ang = msg->twist.twist.angular.y;
    Vz_ang = msg->twist.twist.angular.z;

}

void topic_callback2(const nav_msgs::msg::Odometry::SharedPtr msg) {
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

  auto subscription1 = node->create_subscription<nav_msgs::msg::Odometry>("odom", 10, topic_callback1);
  auto subscription2 = node->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>("/amcl_pose", 10, topic_callback2);

  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);
  geometry_msgs::msg::PoseStamped goal_pose_message;
  rclcpp::WallRate loop_rate(500ms);

  bool flag1 = 1, flag2 = 0, flag3 = 0, flag4 = 0;

  std::cout << "Primer goal" << std::endl;

  while (rclcpp::ok() and flag1) {
      goal_pose_message.pose.position.x = 0.17; //-7.95;
      goal_pose_message.pose.position.y = 6.44; //3.12;
      goal_pose_message.pose.position.z = 0;
      goal_pose_message.pose.orientation.x = 0;
      goal_pose_message.pose.orientation.y = 0;
      goal_pose_message.pose.orientation.z = 0;
      goal_pose_message.pose.orientation.w = 1;
      publisher->publish(goal_pose_message);
      sleep(10);

    if ((Vx == 0.001 and Vy == 0.001)){
      std::cout << "Primer bucle" << std::endl;
      flag1 = 0;
      flag2 = 1;
    }
   
  }

   std::cout << "Segundo goal" << std::endl;

  while (rclcpp::ok() and flag2) {
  
  goal_pose_message.pose.position.x = -2.8;// -15.69;
  goal_pose_message.pose.position.y = 0.44; //32.3366;
  goal_pose_message.pose.position.z = 0;//0;
  goal_pose_message.pose.orientation.x = 0;
  goal_pose_message.pose.orientation.y = 0;
  goal_pose_message.pose.orientation.z = 0;
  goal_pose_message.pose.orientation.w = 1;
  publisher->publish(goal_pose_message);
  rclcpp::spin_some(node);
  loop_rate.sleep();
  sleep(10);

    if ((Vx == 0.001 and Vy == 0.001)){
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

    if ((Vx == 0.001 and Vy == 0.001)){
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

    if ((Vx == 0.001 and Vy == 0.001)){
      std::cout << "Cuarto bucle" << std::endl;
      flag4 = 0;
    }
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  

  rclcpp::shutdown();
  return 0;
}
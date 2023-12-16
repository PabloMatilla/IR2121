#include <chrono>
#include <unistd.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"


using namespace std::chrono_literals;

double Vx, Vy;

double x, y;
double ang;

double Vo = 4.000e-06;
double error = 0.7;

void topic_callback1(const nav_msgs::msg::Odometry::SharedPtr msg) {
    // Obtener la velocidad x y y
    Vx = msg->twist.twist.linear.x;
    Vy = msg->twist.twist.linear.y;
}

void topic_callback2(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg) {
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
}


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");

  auto subscription1 = node->create_subscription<nav_msgs::msg::Odometry>("/odom", 10, topic_callback1);
  auto subscription2 = node->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>("/amcl_pose", 10, topic_callback2);

  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("/goal_pose", 10);

  geometry_msgs::msg::PoseStamped goal_pose_message;
  rclcpp::WallRate loop_rate(500ms);


  bool flag1 = 1, flag2 = 0, flag3 = 0, flag4 = 0;

  double a, b;

  std::cout << "Primer goal" << std::endl;

  goal_pose_message.pose.position.x = a = -2.8;// -15.69;
  goal_pose_message.pose.position.y = b = 1.25; //32.3366;
  goal_pose_message.pose.position.z = 0;//0;
  goal_pose_message.pose.orientation.x = 0;
  goal_pose_message.pose.orientation.y = 0;
  goal_pose_message.pose.orientation.z = 0;
  goal_pose_message.pose.orientation.w = 1;
  publisher->publish(goal_pose_message);
  sleep(4); rclcpp::spin_some(node);
  
  while (rclcpp::ok() and flag1) {
    if (((Vx < Vo) and (Vy < Vo) and (Vx != 0)) and (((a - error) < x) and (x < (a + error))) and (((b - error) < y) and (y < (b + error)))){
      flag1 = 0;
      flag2 = 1;
    }
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }

   std::cout << "Segundo goal" << std::endl;

  while (rclcpp::ok() and flag2) {
  
    goal_pose_message.pose.position.x = a = 5.14; // -0.06;
    goal_pose_message.pose.position.y = b = 5.36; //33.52;
    goal_pose_message.pose.position.z = 0;
    goal_pose_message.pose.orientation.x = 0;
    goal_pose_message.pose.orientation.y = 0;
    goal_pose_message.pose.orientation.z = 0;
    goal_pose_message.pose.orientation.w = 1;
    
    publisher->publish(goal_pose_message);
    sleep(4);
    rclcpp::spin_some(node);
    loop_rate.sleep();
   

    if (((Vx < Vo) and (Vy < Vo) and (Vx != 0)) and (((a - error) < x) and (x < (a + error))) and (((b - error) < y) and (y < (b + error)))){
      flag2 = 0;
      flag3 = 1;
    }
  }

  std::cout << "Tercer goal" << std::endl;

  while (rclcpp::ok() and flag3) {

    goal_pose_message.pose.position.x = a = 10.09; //-8.74;
    goal_pose_message.pose.position.y = b = -1.91; //41.89;
    goal_pose_message.pose.position.z = 0;
    goal_pose_message.pose.orientation.x = 0;
    goal_pose_message.pose.orientation.y = 0;
    goal_pose_message.pose.orientation.z = 0;
    goal_pose_message.pose.orientation.w = 1;
    
    publisher->publish(goal_pose_message);
    sleep(4);
    rclcpp::spin_some(node);
    loop_rate.sleep();

    if (((Vx < Vo) and (Vy < Vo) and (Vx != 0)) and (((a - error) < x) and (x < (a + error))) and (((b - error) < y) and (y < (b + error)))){
      flag3 = 0;
      flag4 = 1;
    }
  }

  std::cout << "Cuarto goal" << std::endl;

  while (rclcpp::ok() and flag4) {

    goal_pose_message.pose.position.x = 1.62; // -0.06;
    goal_pose_message.pose.position.y = 6.08; //33.52;
    goal_pose_message.pose.position.z = 0;
    goal_pose_message.pose.orientation.x = 0;
    goal_pose_message.pose.orientation.y = 0;
    goal_pose_message.pose.orientation.z = 0;
    goal_pose_message.pose.orientation.w = 1;
    publisher->publish(goal_pose_message);
    flag4 = 0;
    sleep(4);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  

  rclcpp::shutdown();
  return 0;
}
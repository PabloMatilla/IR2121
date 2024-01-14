#include <chrono>
#include <vector>
#include "rclcpp/clock.hpp"
#include <unistd.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"



using namespace std::chrono_literals;

double Vx, Vy;

double x, y;

double Vo = 4.000e-06;
double error = 5; //  2;



void topic_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
    // Obtener la velocidad x y y
    Vx = msg->twist.twist.linear.x;
    Vy = msg->twist.twist.linear.y;
}



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");

  auto subscription = node->create_subscription<nav_msgs::msg::Odometry>("/odom", 10, topic_callback);

  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("/goal_pose", 10);

  geometry_msgs::msg::PoseStamped goal_pose_message;
  rclcpp::WallRate loop_rate(500ms);

  rclcpp::Clock::SharedPtr clock = std::make_shared<rclcpp::Clock>(RCL_ROS_TIME);
  tf2_ros::Buffer tfBuffer(clock);

    // TF2 Buffer and Listener for performing transformations
  tf2_ros::TransformListener tfListener(tfBuffer);
  geometry_msgs::msg::TransformStamped transformStamped;


  double a, b;
  bool flag1;

  std::vector<double> house_sim_x = {-2.8, 5.14, 10.09, 1.62};
  std::vector<double> house_sim_y = {1.25, 5.36, -1.91, 6.08};

  std::vector<double> laboratory_x = {-24.73, -28.50, -20.24, 24.73};
  std::vector<double> laboratory_y = {-8.78, -20.83, -22.23, -8.78};

  for (int i=0; i<4; i++){  
      flag1 = 1;

      while (rclcpp::ok() and flag1) {
                                        
        goal_pose_message.pose.position.x = a = house_sim_x[i];
        goal_pose_message.pose.position.y = b = house_sim_y[i];
        goal_pose_message.pose.position.z = 0;
        goal_pose_message.pose.orientation.x = 0;
        goal_pose_message.pose.orientation.y = 0;
        goal_pose_message.pose.orientation.z = 0;
        goal_pose_message.pose.orientation.w = 1;
        publisher->publish(goal_pose_message);
        sleep(4); rclcpp::spin_some(node);

      try {
          transformStamped = tfBuffer.lookupTransform("base_link", "map", rclcpp::Time(0));
          } catch (tf2::TransformException &ex) {
              RCLCPP_ERROR(node->get_logger(), "%s", ex.what());
              continue;
          }

      x = - transformStamped.transform.translation.x;
      y = - transformStamped.transform.translation.y;

      std::cout << "a: " << a << "     x: "    << x << std::endl;
      std::cout << "b: " << b << "     y: "    << y << std::endl;

        if (((Vx < Vo) and (Vy < Vo) and (Vx != 0)) and (((a - error) < x) and (x < (a + error))) and (((b - error) < y) and (y < (b + error)))){
          flag1 = 0;
        }
        rclcpp::spin_some(node);
        loop_rate.sleep();
      }

  }

  rclcpp::shutdown();
  return 0;
}
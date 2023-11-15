#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("publisher");
  auto publisher = node->create_publisher<geometry_msgs::msg::PoseStamped>("goal_pose", 10);
  geometry_msgs::msg::PoseStamped goal_pose_message;
  rclcpp::WallRate loop_rate(500ms);

  while (rclcpp::ok()) {
    publisher->publish(goal_pose_message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}


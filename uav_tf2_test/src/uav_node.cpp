#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

using namespace std::chrono_literals;


class uavPublisher : public rclcpp::Node
{
public:
  uavPublisher()
  : Node("Uav_publisher")
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("uav/pose", 10);
    auto timer_callback =
      [this]() -> void {
        auto message = geometry_msgs::msg::Pose();
        auto position = geometry_msgs::msg::Point();
        position.x = 3;
        position.y = 1;
        position.z = 5;
        auto orientation = geometry_msgs::msg::Quaternion();
        message.position = position;
        message.orientation = orientation;
        // RCLCPP_INFO(this->get_logger(), "Publishing uav pose...");
        this->publisher_->publish(message);
      };
    timer_ = this->create_wall_timer(50ms, timer_callback);
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<uavPublisher>());
  rclcpp::shutdown();
  return 0;
}
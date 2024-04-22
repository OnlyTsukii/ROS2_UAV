#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

using namespace std::chrono_literals;


class LidarPublisher : public rclcpp::Node
{
public:
  LidarPublisher()
  : Node("Lidar_publisher")
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("lidar/pose", 10);
    auto timer_callback =
      [this]() -> void {
        auto message = geometry_msgs::msg::Pose();
        auto position = geometry_msgs::msg::Point();
        position.x = 4;
        position.y = 7;
        position.z = 2;
        auto orientation = geometry_msgs::msg::Quaternion();
        message.position = position;
        message.orientation = orientation;
        // RCLCPP_INFO(this->get_logger(), "Publishing Lidar pose...");
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
  rclcpp::spin(std::make_shared<LidarPublisher>());
  rclcpp::shutdown();
  return 0;
}
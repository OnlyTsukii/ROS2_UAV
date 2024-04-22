#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/quaternion.hpp"

using namespace std::chrono_literals;


class CameraPublisher : public rclcpp::Node
{
public:
  CameraPublisher()
  : Node("Camera_publisher")
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("camera/pose", 10);
    auto timer_callback =
      [this]() -> void {
        auto message = geometry_msgs::msg::Pose();
        auto position = geometry_msgs::msg::Point();
        position.x = 2;
        position.y = 4;
        position.z = 1;
        auto orientation = geometry_msgs::msg::Quaternion();
        message.position = position;
        message.orientation = orientation;
        // RCLCPP_INFO(this->get_logger(), "Publishing camera pose...");
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
  rclcpp::spin(std::make_shared<CameraPublisher>());
  rclcpp::shutdown();
  return 0;
}
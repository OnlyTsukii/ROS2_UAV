#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

class FrameListener : public rclcpp::Node
{
public:
  FrameListener()
  : Node("tf2_listener")
  {
    source_frame_1 = this->declare_parameter<std::string>("source_frame1", "camera");
    source_frame_2 = this->declare_parameter<std::string>("source_frame2", "lidar");

    tf_buffer_ =
      std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ =
      std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    timer_ = this->create_wall_timer(
      1s, std::bind(&FrameListener::on_timer, this));
  }

private:
  void on_timer()
  {
    std::string fromFrameRel1 = source_frame_1.c_str();
    std::string fromFrameRel2 = source_frame_2.c_str();
    std::string toFrameRel = "uav";

    geometry_msgs::msg::TransformStamped t;

    rclcpp::Time now = this->get_clock()->now();

    try {
        geometry_msgs::msg::TransformStamped t1 = tf_buffer_->lookupTransform(
                toFrameRel, fromFrameRel1, now, 100ms);

        RCLCPP_INFO(
          this->get_logger(), "Transform %s to %s successfully, result: p_x %f p_y %f p_z %f r_x %f r_y %f r_z %f r_w %f",
          fromFrameRel1.c_str(), toFrameRel.c_str(), t1.transform.translation.x, 
          t1.transform.translation.y, t1.transform.translation.z, t1.transform.rotation.x, 
          t1.transform.rotation.y, t1.transform.rotation.z, t1.transform.rotation.w 
       );
    } catch (const tf2::TransformException & ex) {
        RCLCPP_INFO(
            this->get_logger(), "Could not transform %s to %s: %s",
            fromFrameRel1.c_str(), toFrameRel.c_str(), ex.what());
        return;
    }

    try {
        geometry_msgs::msg::TransformStamped t2 = tf_buffer_->lookupTransform(
                toFrameRel, fromFrameRel2, now, 100ms);

        RCLCPP_INFO(
          this->get_logger(), "Transform %s to %s successfully, result: p_x %f p_y %f p_z %f r_x %f r_y %f r_z %f r_w %f",
          fromFrameRel2.c_str(), toFrameRel.c_str(), t2.transform.translation.x, 
          t2.transform.translation.y, t2.transform.translation.z, t2.transform.rotation.x, 
          t2.transform.rotation.y, t2.transform.rotation.z, t2.transform.rotation.w 
        );
    } catch (const tf2::TransformException & ex) {
        RCLCPP_INFO(
            this->get_logger(), "Could not transform %s to %s: %s",
            fromFrameRel2.c_str(), toFrameRel.c_str(), ex.what());
        return;
    }

    try {
        geometry_msgs::msg::TransformStamped t3 = tf_buffer_->lookupTransform(
                source_frame_2.c_str(), source_frame_1.c_str(), now, 100ms);

        RCLCPP_INFO(
          this->get_logger(), "Transform %s to %s successfully, result: p_x %f p_y %f p_z %f r_x %f r_y %f r_z %f r_w %f",
          source_frame_1.c_str(), source_frame_2.c_str(), t3.transform.translation.x, 
          t3.transform.translation.y, t3.transform.translation.z, t3.transform.rotation.x, 
          t3.transform.rotation.y, t3.transform.rotation.z, t3.transform.rotation.w 
        );
    } catch (const tf2::TransformException & ex) {
        RCLCPP_INFO(
            this->get_logger(), "Could not transform %s to %s: %s",
            source_frame_1.c_str(), source_frame_2.c_str(), ex.what());
        return;
    }
  }

  rclcpp::TimerBase::SharedPtr timer_{nullptr};
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_{nullptr};
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::string source_frame_1;
  std::string source_frame_2;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FrameListener>());
  rclcpp::shutdown();
  return 0;
}

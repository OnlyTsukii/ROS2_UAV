#include <functional>
#include <memory>
#include <sstream>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"

class FramePublisher : public rclcpp::Node
{
public:
  FramePublisher()
  : Node("uav_tf2_broadcaster")
  {
    uavname_ = this->declare_parameter<std::string>("nodename", "uav");

    tf_broadcaster_ =
      std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    std::ostringstream stream;
    stream << "/" << uavname_.c_str() << "/pose";
    std::string topic_name = stream.str();

    subscription_ = this->create_subscription<geometry_msgs::msg::Pose>(
      topic_name, 10,
      std::bind(&FramePublisher::handle_uav_pose, this, std::placeholders::_1));
  }

private:
  void handle_uav_pose(const std::shared_ptr<geometry_msgs::msg::Pose> msg)
  {
    geometry_msgs::msg::TransformStamped t;

    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "world";
    t.child_frame_id = uavname_.c_str();

    t.transform.translation.x = msg->position.x;
    t.transform.translation.y = msg->position.y;
    t.transform.translation.z = msg->position.z;

    t.transform.rotation.x = msg->orientation.x;
    t.transform.rotation.y = msg->orientation.y;
    t.transform.rotation.z = msg->orientation.z;
    t.transform.rotation.w = msg->orientation.w;

    tf_broadcaster_->sendTransform(t);
  }

  rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  std::string uavname_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FramePublisher>());
  rclcpp::shutdown();
  return 0;
}

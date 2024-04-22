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
  : Node("lidar_tf2_broadcaster")
  {
    lidarname_ = this->declare_parameter<std::string>("nodename", "lidar");

    tf_broadcaster_ =
      std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    std::ostringstream stream;
    stream << "/" << lidarname_.c_str() << "/pose";
    std::string topic_name = stream.str();

    subscription_ = this->create_subscription<geometry_msgs::msg::Pose>(
      topic_name, 10,
      std::bind(&FramePublisher::handle_lidar_pose, this, std::placeholders::_1));
  }

private:
  void handle_lidar_pose(const std::shared_ptr<geometry_msgs::msg::Pose> msg)
  {
    geometry_msgs::msg::TransformStamped t;

    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "uav";
    t.child_frame_id = lidarname_.c_str();

    t.transform.translation.x = msg->position.x + 3;
    t.transform.translation.y = msg->position.y + 1;
    t.transform.translation.z = msg->position.z + 2;

    t.transform.rotation.x = msg->orientation.x;
    t.transform.rotation.y = msg->orientation.y;
    t.transform.rotation.z = msg->orientation.z;
    t.transform.rotation.w = msg->orientation.w;

    tf_broadcaster_->sendTransform(t);
  }

  rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  std::string lidarname_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FramePublisher>());
  rclcpp::shutdown();
  return 0;
}

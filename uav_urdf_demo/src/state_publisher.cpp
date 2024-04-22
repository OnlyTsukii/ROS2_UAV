#include <functional>
#include <string>
#include <math.h>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/quaternion.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "std_msgs/msg/int16.hpp"
#include "tf2_ros/transform_broadcaster.h"

class StatePublisher : public rclcpp::Node {
public:
    StatePublisher() : Node("state_publisher") {
        publisher = create_publisher<sensor_msgs::msg::JointState>(
            "joint_states", 10
        );

        subscriber = this->create_subscription<std_msgs::msg::Int16>(
            "cmd_direction", 10, 
            std::bind(&StatePublisher::cmd_callback, this, std::placeholders::_1)
        );

        tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

        degree = 3.1415926 / 180.0;
        step = 0.01;
        direction = 1;
        angle = 0;

        transform_stamped.header.frame_id = "odom";
        transform_stamped.child_frame_id = "body";

        // timer = this->create_wall_timer(std::chrono::milliseconds(30), std::bind(&StatePublisher::timer_callback, this));
    }
private:
    geometry_msgs::msg::Quaternion euler_to_quaternion(float roll, float pitch, float yaw){
        float qx = sin(roll/2) * cos(pitch/2) * cos(yaw/2) - cos(roll/2) * sin(pitch/2) * sin(yaw/2);
        float qy = cos(roll/2) * sin(pitch/2) * cos(yaw/2) + sin(roll/2) * cos(pitch/2) * sin(yaw/2);
        float qz = cos(roll/2) * cos(pitch/2) * sin(yaw/2) - sin(roll/2) * sin(pitch/2) * cos(yaw/2);
        float qw = cos(roll/2) * cos(pitch/2) * cos(yaw/2) + sin(roll/2) * sin(pitch/2) * sin(yaw/2);
        
        geometry_msgs::msg::Quaternion rotation;
        rotation.x = qx;
        rotation.y = qy;
        rotation.z = qz;
        rotation.w = qw;

        return rotation;
    }

    void cmd_callback(const std::shared_ptr<std_msgs::msg::Int16> msg){

        joint_state.header.stamp = this->get_clock()->now();
        joint_state.name = {"left_wing_swivel", "right_wing_swivel"};

        left_swivel_pos = sin(angle * 8);
        right_swivel_pos = -1 * sin(angle * 8);

        joint_state.position = {left_swivel_pos, right_swivel_pos};

        transform_stamped.header.stamp = this->get_clock()->now();

        // transform_stamped.transform.translation.x = sin(angle) * 3;
        // transform_stamped.transform.translation.y = cos(angle) * 3;
        // transform_stamped.transform.translation.z = 0;
        
        // transform_stamped.transform.translation.x = 0;
        // transform_stamped.transform.translation.y = 0;
        // transform_stamped.transform.translation.z = 0;

        switch (msg->data)
        {
        case 0:
            transform_stamped.transform.translation.x += step * cos(yaw);
            transform_stamped.transform.translation.y += step * sin(yaw);
            RCLCPP_INFO(this->get_logger(), "Got cmd 0");
            break;
        case 1:
            transform_stamped.transform.translation.x -= step * cos(yaw);
            transform_stamped.transform.translation.y -= step * sin(yaw);
            RCLCPP_INFO(this->get_logger(), "Got cmd 1");
            break;
        case 2:
            yaw -= 0.1;
            RCLCPP_INFO(this->get_logger(), "Got cmd 2");
            break;
        case 3:
            yaw += 0.1;
            RCLCPP_INFO(this->get_logger(), "Got cmd 3");
            break;
        case 4:
            step += 0.01;
            RCLCPP_INFO(this->get_logger(), "Speed up! Current speed: %f", step);
            break;
        case 5:
            step -= 0.01;
            RCLCPP_INFO(this->get_logger(), "Slow down! Current speed: %f", step);
            break;
        
        default:
            break;
        }

        // yaw = -1 * angle;

        transform_stamped.transform.rotation = euler_to_quaternion(roll, pitch, yaw);

        publisher->publish(joint_state);
        tf_broadcaster->sendTransform(transform_stamped);

        angle += degree;
    }

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher;
    rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr subscriber;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;
    float degree, angle;
    float left_swivel_pos, right_swivel_pos;
    float step;
    int direction;
    float roll, pitch, yaw;
    sensor_msgs::msg::JointState joint_state;
    geometry_msgs::msg::TransformStamped transform_stamped;
    rclcpp::TimerBase::SharedPtr timer;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StatePublisher>());
    rclcpp::shutdown();

    return 0;
}
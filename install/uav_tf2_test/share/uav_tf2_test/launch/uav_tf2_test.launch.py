from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='uav_tf2_test',
            executable='camera_node',
            name='Camera_publisher'
        ),
        Node(
            package='uav_tf2_test',
            executable='lidar_node',
            name='Lidar_publisher'
        ),
        Node(
            package='uav_tf2_test',
            executable='uav_node',
            name='Uav_publisher'
        ),
        Node(
            package='uav_tf2_test',
            executable='camera_tf2_broadcaster',
            name='camera_tf2_broadcaster'
        ),
        Node(
            package='uav_tf2_test',
            executable='lidar_tf2_broadcaster',
            name='lidar_tf2_broadcaster'
        ),
        Node(
            package='uav_tf2_test',
            executable='uav_tf2_broadcaster',
            name='uav_tf2_broadcaster'
        ),
        Node(
            package='uav_tf2_test',
            executable='tf2_listener',
            name='tf2_listener'
        ),
    ])

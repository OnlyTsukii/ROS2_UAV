#include <iostream>
#include <unistd.h>
#include <termios.h>
#include "std_msgs/msg/int16.hpp"
#include "rclcpp/rclcpp.hpp"

class CommandPublisher : public rclcpp::Node {
public:
    CommandPublisher() : Node("command_publisher") {
        publisher = create_publisher<std_msgs::msg::Int16>("cmd_direction", 10);

        struct termios old_tio, new_tio;
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

        std::cout << "Press arrow keys to continue (press Enter to quit)..." << std::endl;

        char ch;
        char buf[3]; 

        while (true) {
            read(STDIN_FILENO, &ch, 1);
            
            if (ch == '\033') {
                read(STDIN_FILENO, buf, 3);
                if (buf[0] == '[') {
                    switch (buf[1]) {
                        case 'A':
                            // FORWARD
                            cmd.data = 0;
                            publisher->publish(cmd);
                            break;
                        case 'B':
                            // BACKWARD
                            cmd.data = 1;
                            publisher->publish(cmd);
                            break;
                        case 'C':
                            // RIGHT ROLL
                            cmd.data = 2;
                            publisher->publish(cmd);
                            break;
                        case 'D':
                            // LEFT ROLL
                            cmd.data = 3;
                            publisher->publish(cmd);
                            break;
                    }
                }
            } else if (ch == 'w') {
                cmd.data = 4;
                publisher->publish(cmd);
            } else if (ch == 's') {
                cmd.data = 5;
                publisher->publish(cmd);
            }
            if (ch == '\n') break;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    }

private:
    rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher;
    std_msgs::msg::Int16 cmd;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CommandPublisher>());
    rclcpp::shutdown();

    return 0;
}

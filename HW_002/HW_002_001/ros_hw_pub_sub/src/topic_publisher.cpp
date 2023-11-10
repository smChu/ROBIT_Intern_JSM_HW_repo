#include "ros/ros.h"
// ROS 기본 헤더파일
#include "ros_hw_pub_sub/MsgTutorial.h"// MsgTutorial 메시지 파일 헤더(빌드 후 자동 생성됨)
using namespace std;
int main(int argc, char **argv)
{
    ros::init(argc, argv, "topic_publisher");
    ros::NodeHandle nh;
    // 퍼블리셔 선언, ros_hw_pub_sub 패키지의 MsgTutorial 메시지 파일을 이용한
    // 퍼블리셔 ros_hw_pub_sub 를 작성한다. 토픽명은 "ros_tutorial_msg" 이며,
    // 퍼블리셔 큐(queue) 사이즈를 100개로 설정한다는 것이다
    ros::Publisher ros_hw_pub_sub = nh.advertise<ros_hw_pub_sub::MsgTutorial>("ros_tutorial_msg", 100);
    // MsgTutorial 메시지 파일 형식으로 msg 라는 메시지를 선언
    ros_hw_pub_sub::MsgTutorial msg;
    // 메시지에 사용될 변수 선언
    int count1 = 0;
    int count2 = 0;
    string input_message;
    while (ros::ok())
    {
        cout<<"input data:";
        cin>> count1 >> count2;
        //문자열 입력 방식
        cin.ignore();
        getline(cin, input_message);

        msg.stamp = ros::Time::now();
        msg.data1 = count1;
        msg.data2 = count2;
        msg.message = input_message;

        // 현재 시간을 msg의 하위 stamp 메시지에 담는다
        // count라는 변수 값을 msg의 하위 data 메시지에 담는다
        ROS_INFO("send msg = %d", msg.stamp.sec);
        ROS_INFO("send msg = %d", msg.stamp.nsec);
        ROS_INFO("send data1 = %ld", msg.data1);
        ROS_INFO("send data2 = %ld", msg.data2);
        ROS_INFO("send intput_message = %s", msg.message.c_str());// stamp.sec 메시지를 표시한다
        // stamp.nsec 메시지를 표시한다
        // data 메시지를 표시한다
        ros_hw_pub_sub.publish(msg); // 메시지를 발행한다

    }
    return 0;
}

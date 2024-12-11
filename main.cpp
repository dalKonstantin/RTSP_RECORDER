#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main()
{
    std::vector<std::string> streams = {
        "rtsp:/192.168.",
        "",
        "",
        ""
        };
    std::vector<std::string> outputs = {
        "",
        "",
        "",
        ""
        };

    std::vector<cv::VideoCapture> video_captures(streams.size());
    std::vector<cv::VideoWriter> video_writers(streams.size());



}



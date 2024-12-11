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

    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    double fps = 30.0;

    for(size_t i = 0; i < streams.size(); i++) {
        video_captures[i].open(streams[i]);
        if(!video_captures[i].isOpened()) {
            std::cerr << "Error opening video stream : " << streams[i] << std::endl;
            return -1;
    }

    int frame_width = static_cast<int>(video_captures[i].get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(video_captures[i].get(cv::CAP_PROP_FRAME_HEIGHT));

    video_writers[i].open(outputs[i], codec, fps, cv::Size(frame_width, frame_height), true);
    if(!video_writers[i].isOpened()){
        std::cerr << "Error opening video writer: " << outputs[i] << std::endl;
        return -1;
    }


    while(true) // main loop
    {
        std::vector<cv::Mat> frames(streams.size());

        for(size_t i = 0; i < streams.size(); i++){
            video_captures[i] >> frames[i];
            if(frames[i].empty()) {
                std::cerr << "Error reading frame from stream: " << streams[i] << std::endl;
                break;
            }
          video_writers[i].write(frames[i]);
        }


        if(cv::waitKey(1) == 'q')
            break;
    }


    for(size_t i; i < streams.size(); i++){
        video_captures[i].release();
        video_writers[i].release();
    }

    }


}

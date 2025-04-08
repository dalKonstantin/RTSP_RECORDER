#pragma once
#include <string>
#include <opencv2/opencv.hpp>

class CameraRecorder {
public:
    CameraRecorder(const std::string& rtspUrl, const std::string& outputFile,
                   int width, int height, double fps, bool& stopFlag);
    void record();

private:
    std::string rtspUrl;
    std::string outputFile;
    int width, height;
    double fps;
    bool& stopFlag;
};

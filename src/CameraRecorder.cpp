#include "CameraRecorder.h"
#include <iostream>

CameraRecorder::CameraRecorder(const std::string& rtspUrl, const std::string& outputFile,
                               int width, int height, double fps, bool& stopFlag)
    : rtspUrl(rtspUrl), outputFile(outputFile), width(width), height(height), fps(fps), stopFlag(stopFlag) {}

void CameraRecorder::record() {
    cv::VideoCapture capture(rtspUrl);
    if (!capture.isOpened()) {
        std::cerr << "Ошибка подключения к камере: " << rtspUrl << std::endl;
        return;
    }

    cv::VideoWriter writer(outputFile, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(width, height));
    if (!writer.isOpened()) {
        std::cerr << "Ошибка создания файла: " << outputFile << std::endl;
        return;
    }

    std::cout << "Начало записи с камеры: " << rtspUrl << " в файл " << outputFile << std::endl;
    while (!stopFlag) {
        cv::Mat frame;
        capture >> frame;
        if (frame.empty()) break;
        writer << frame;
    }

    capture.release();
    writer.release();
    std::cout << "Запись завершена для камеры: " << rtspUrl << std::endl;
}

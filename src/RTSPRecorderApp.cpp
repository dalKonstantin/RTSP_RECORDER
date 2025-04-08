#include "RTSPRecorderApp.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <csignal>
#include <opencv2/opencv.hpp>
#include "utils.h"

namespace fs = std::filesystem;
static RTSPRecorderApp* appInstance = nullptr;

RTSPRecorderApp::RTSPRecorderApp(const std::string& ipFilePath, const std::string& outputBaseDir)
    : ipFilePath(ipFilePath), outputBaseDir(outputBaseDir), stopFlag(false) {
    appInstance = this;
    std::signal(SIGINT, signalHandler);
}

void RTSPRecorderApp::run() {
    readIpFile();
    if (rtspUrls.empty()) {
        std::cerr << "Файл ip.txt пуст или не найден!" << std::endl;
        return;
    }
    startRecording();
    waitForThreads();
    std::cout << "Все записи завершены." << std::endl;
}

void RTSPRecorderApp::readIpFile() {
    std::ifstream inputFile(ipFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка открытия файла: " << ipFilePath << std::endl;
        return;
    }
    std::string line;
    while (std::getline(inputFile, line)) {
        rtspUrls.push_back(line);
    }
    inputFile.close();
}

void RTSPRecorderApp::startRecording() {
    std::string timestamp = getCurrentTimestamp();
    std::string outputDir = outputBaseDir + "/" + timestamp;
    fs::create_directories(outputDir);

    cv::VideoCapture cap(rtspUrls[0]);
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = 30.0;
    cap.release();

    for (size_t i = 0; i < rtspUrls.size(); ++i) {
        std::string outputFile = outputDir + "/cam" + std::to_string(i + 1) + ".mp4";
        recorders.emplace_back(std::make_unique<CameraRecorder>(rtspUrls[i], outputFile, width, height, fps, stopFlag));
        threads.emplace_back(&CameraRecorder::record, recorders.back().get());
    }
}

void RTSPRecorderApp::waitForThreads() {
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

void RTSPRecorderApp::signalHandler(int signum) {
    std::cout << "\nПолучен сигнал (" << signum << "). Завершаем запись..." << std::endl;
    if (appInstance) appInstance->stopFlag = true;
}

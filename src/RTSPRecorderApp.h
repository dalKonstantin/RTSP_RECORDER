#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "CameraRecorder.h"

class RTSPRecorderApp {
public:
    RTSPRecorderApp(const std::string& ipFilePath, const std::string& outputBaseDir);
    void run();

private:
    std::string ipFilePath;
    std::string outputBaseDir;
    std::vector<std::string> rtspUrls;
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<CameraRecorder>> recorders;
    bool stopFlag;
    std::mutex stopMutex;

    void readIpFile();
    void startRecording();
    void waitForThreads();
    static void signalHandler(int signum);
};

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <csignal>

namespace fs = std::filesystem;

bool stopFlag = false; // Глобальный флаг завершения
bool startFlag = false; // Флаг начала записи
std::mutex startMutex;  // Мьютекс для синхронизации старта
std::condition_variable startCondition; // Условие старта

// Обработчик сигнала
void signalHandler(int signum) {
    std::cout << "\nПолучен сигнал (" << signum << "). Завершаем запись..." << std::endl;
    stopFlag = true;
}

std::string getCurrentTimestamp() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d_%m_%y___%H:%M:%S", ltm);
    return std::string(buffer);
}

void recordCamera(const std::string &rtspUrl, const std::string &outputFile, int frameWidth, int frameHeight, double fps, bool &stopFlag) {
    cv::VideoCapture capture(rtspUrl);
    if (!capture.isOpened()) {
        std::cerr << "Ошибка подключения к камере: " << rtspUrl << std::endl;
        return;
    }

    cv::VideoWriter writer(outputFile, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(frameWidth, frameHeight));
    if (!writer.isOpened()) {
        std::cerr << "Ошибка создания файла: " << outputFile << std::endl;
        return;
    }

    // Ждём, пока основной поток даст сигнал на старт
    {
        std::unique_lock<std::mutex> lock(startMutex);
        startCondition.wait(lock, [] { return startFlag; });
    }

    std::cout << "Начало записи с камеры: " << rtspUrl << " в файл " << outputFile << std::endl;

    while (!stopFlag) {
        cv::Mat frame;
        capture >> frame;

        if (frame.empty()) {
            std::cerr << "Ошибка чтения кадра с камеры: " << rtspUrl << std::endl;
            break;
        }

        writer << frame;
    }

    capture.release();
    writer.release();
    std::cout << "Запись завершена для камеры: " << rtspUrl << std::endl;
}

int main() {
    std::signal(SIGINT, signalHandler);

    // Чтение RTSP-URL'ов из файла
    const std::string ipFile = "ip.txt";
    std::ifstream inputFile(ipFile);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка открытия файла: " << ipFile << std::endl;
        return -1;
    }

    std::vector<std::string> rtspUrls;
    std::string line;
    while (std::getline(inputFile, line)) {
        rtspUrls.push_back(line);
    }
    inputFile.close();

    if (rtspUrls.empty()) {
        std::cerr << "Файл ip.txt пуст!" << std::endl;
        return -1;
    }

    std::string timestamp = getCurrentTimestamp();
    std::string outputDir = "Videos/" + timestamp;
    fs::create_directories(outputDir);

    cv::VideoCapture capture(rtspUrls[0]);

    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    int frameWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = 30.0;

    capture.release();

    // Запуск потоков для каждой камеры
    std::vector<std::thread> threads;
    for (size_t i = 0; i < rtspUrls.size(); ++i) {
        std::string outputFile = outputDir + "/cam" + std::to_string(i + 1) + ".mp4";
        threads.emplace_back(recordCamera, rtspUrls[i], outputFile, frameWidth, frameHeight, fps, std::ref(stopFlag));
    }

    // Сигнализируем потокам о старте записи
    {
        std::lock_guard<std::mutex> lock(startMutex);
        startFlag = true;
    }
    startCondition.notify_all();

    // Ожидание завершения всех потоков
    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    cv::destroyAllWindows();
    std::cout << "Все записи завершены." << std::endl;
    return 0;
}

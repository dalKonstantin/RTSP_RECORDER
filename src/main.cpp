#include "RTSPRecorderApp.h"

int main() {
    RTSPRecorderApp app("ip.txt", "Videos");
    app.run();
    return 0;
}

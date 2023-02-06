#include <iostream>
#include <memory>
#include <cstring>

#include "networkActivityMonitor.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    bool guiEnabled = false;
    if (argc > 1 && !std::strcmp(argv[1], "-g")) {
        guiEnabled = true;
    }
    spdlog::info("Application started with the GUI {0}", (guiEnabled ? "enabled" : "disabled"));
    auto m_nam = std::make_unique<NetworkActivityMonitor>();
}
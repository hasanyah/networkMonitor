#include "cmdExecutor.h"

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "spdlog/spdlog.h"

const std::string UnixCmdExecutor::runCmd(const char* cmd) {
    spdlog::debug("Executing Unix Command: {0}", cmd);
    
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    spdlog::debug("Executed Unix Command successfully");
    
    std::size_t pos = result.find(" ");
    if (pos != 0)
        result = result.substr(0, pos);

    return result;
}

const std::string DosCmdExecutor::runCmd(const char* cmd) {
    spdlog::debug("Received UNSUPPORTED DOS Command: {0}", cmd);
    return std::string{"Not yet available on windows"};
}

std::unique_ptr<CmdExecutor> CmdExecutorCreator::Create() {
    return std::make_unique<UnixCmdExecutor>(UnixCmdExecutor{});
}   

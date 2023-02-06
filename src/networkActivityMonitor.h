#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <string>

#include "cmdExecutor.h"
#include <tins/tins.h>

struct Transmission {
    int totalUL{0};
    int totalDL{0};
    int transmissionCount{0};
    std::time_t lastTransmissionTime{};
};
typedef std::map<std::string, Transmission> TransmissionLog; 

class NetworkActivityMonitor {
    private:
        const std::unique_ptr<CmdExecutorFactory> m_factory;
        const std::unique_ptr<CmdExecutor> m_cmdExecutor;
        const std::unique_ptr<Tins::Sniffer> m_sniffer;
        const std::function<void(const Tins::PDU &pdu)> m_onNetworkActivityReceived;

        void setIP(const std::string&);
        void updateIP();
        static void addToLog(const std::string& src, const std::string& dest, int size);

    public:
        NetworkActivityMonitor();
        ~NetworkActivityMonitor();
};
#include "networkActivityMonitor.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "cmdExecutor.h"
#include "spdlog/spdlog.h"

static TransmissionLog s_logs;
static std::string localIP;
bool onNetworkActivity(const Tins::PDU &pdu);

std::ostream& operator<<(std::ostream& os, const Transmission& log)
{
    os << "DL/UL: " << log.totalDL << '/' << log.totalUL << " - " 
    << log.transmissionCount << " transactions." 
    << " Last seen on: " << ctime(&log.lastTransmissionTime);
    return os;
}

NetworkActivityMonitor::NetworkActivityMonitor()
    : m_factory(std::make_unique<CmdExecutorCreator>(CmdExecutorCreator {})),
      m_cmdExecutor(m_factory->Create()),
      m_sniffer(std::make_unique<Tins::Sniffer>("eth0"))
{
    updateIP(); 
    m_sniffer->sniff_loop(onNetworkActivity);
}

NetworkActivityMonitor::~NetworkActivityMonitor() {}

void NetworkActivityMonitor::setIP(const std::string& ip) {
    localIP = ip;
}

// In case the IP changes
void NetworkActivityMonitor::updateIP() {
    auto ipAddress = m_cmdExecutor->runCmd("hostname -I");
    setIP(ipAddress);
}

void addLog(TransmissionLog& logs, const std::string& src, const std::string& dest, int size) {
    if (src.compare(localIP) == 0) {
        // UPLOAD
        spdlog::debug("Inserting new UL Log");
        if (logs.count(dest) == 0) {
            logs.insert({dest, Transmission{size, 0, 1, std::time(nullptr)}});
        } else {
            TransmissionLog::iterator it = logs.find(dest);
            if (it != logs.end()) {
                it->second.totalUL += size;
                it->second.transmissionCount++;
                it->second.lastTransmissionTime = std::time(nullptr);
            }
        }
    } else if (dest.compare(localIP) == 0) {
        // DOWNLOAD
        spdlog::debug("Inserting new DL Log");
        if (logs.count(src) == 0) {
            logs.insert({src, Transmission{0, size, 0, std::time(nullptr)}});
        } else {
            TransmissionLog::iterator it = logs.find(src);
            if (it != logs.end()) {
                it->second.totalDL += size;
                it->second.transmissionCount++;
                it->second.lastTransmissionTime = std::time(nullptr);
            }
        }
    } else {
        spdlog::warn("UL/DL info is unknown. Local IP address may have changed.");
    }

    // TODO: Pretty print
    // TODO: Print on a timed event e.g. every X seconds/minutes
    spdlog::info("Transmission Logs:");  
    TransmissionLog::iterator itr;
    for( itr = logs.begin(); itr != logs.end(); ++itr){
        std::cout << "Key => " << itr->first << ", Value => " << itr->second << std::endl;      
    }    
}

bool onNetworkActivity(const Tins::PDU &pdu) {
    // Find the IP layer
    const Tins::IP &ip = pdu.rfind_pdu<Tins::IP>(); 
    // Find the TCP layer
    const Tins::TCP &tcp = pdu.rfind_pdu<Tins::TCP>();
    std::ostringstream src{};
    src << ip.src_addr();
    std::ostringstream dest{};
    dest << ip.dst_addr();
    int size{ip.advertised_size()};
    
    addLog(s_logs, src.str(), dest.str(), size);
    
    return true;
}
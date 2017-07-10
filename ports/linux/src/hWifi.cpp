/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hWifi.h"
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <cstring>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

// Wifi support on Linux is handled by external husarion-wifi command.

namespace hFramework {
void cmd(std::vector<std::string> args) {
    int pid = fork();
    if (pid == 0) {
        std::vector<const char*> ss;
        for (std::string& s: args) ss.push_back(s.c_str());
        ss.push_back(nullptr);
        execvp("husarion-wifi", (char**)ss.data());
        _exit(1);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    if (status != 0) {
        std::cerr << "husarion-wifi call returned " << status << std::endl;
    }
}

void _hWifi::setupAP(const AccessPointConfig& config) {
    std::vector<std::string> args = {
        "husarion-wifi", "setup-ap",
        "--channel", std::to_string(config.channel), "--max-connections", std::to_string(config.maxConnections),
        "--ip-address", std::to_string(htonl(config.ipAddress.v4)), "--netmask", std::to_string(config.netmask),
        "--", config.ssid, config.password
    };

    cmd(args);
}

void _hWifi::connect(const NetworkConfig& config) {
    std::vector<std::string> args = {
        "husarion-wifi", "connect", "--", config.ssid, config.password
    };

    cmd(args);
}

void _hWifi::connect(const NetworkConfig* configs, int count) {
    for (int i=0; i < count; i ++)
        connect(configs[i]);
}

bool _hWifi::scan(std::vector<ScannedNetwork>& networks) {
    std::cerr << "scanning..." << std::endl;
    cmd({
            "husarion-wifi", "scan-write"
        });

    std::cerr << "scanning..." << std::endl;
    FILE* f = fopen("/run/husarion-wifi-networks", "rb");
    short count;
    if (fread(&count, sizeof(short), 1, f) != 1) {
        std::cerr << "scanning failed" << std::endl;
        return false;
    }
    std::cerr << "found " << count << " networks" << std::endl;

    networks.resize(count);
    if ((int)fread(networks.data(), sizeof(ScannedNetwork), count, f) != (int)count) return false;
    fclose(f);

    return true;
}

bool _hWifi::isComboModeSupported() {
    return system("husarion-wifi is-combo-mode-supported") == 0;
}

_hWifi Wifi;

bool _Network::isOnline() {
    return system("ip route get 8.8.8.8 >/dev/null 2>&1") == 0;
}

const char* _Network::getLocalIp() {
    // we rely on predictable interface names being disabled
    for (const char* name : {"eth0", "wlan0", "wlan1", "wlan2", "eth1"}) {
        int fd = socket(AF_INET, SOCK_DGRAM, 0);
        ifreq ifr;
        ifr.ifr_addr.sa_family = AF_INET;
        strcpy(ifr.ifr_name, name);

        if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
            close(fd);
            return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
        }
        close(fd);
    }
    return "";
}

}

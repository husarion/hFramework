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
#include <arpa/inet.h>

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
        "husarion-wifi", "setup-ap", config.ssid, config.password,
        "--channel", std::to_string(config.channel), "--max-connections", std::to_string(config.maxConnections),
        "--ip-address", std::to_string(htonl(config.ipAddress.v4)), "--netmask", std::to_string(config.netmask)
    };

    cmd(args);
}

void _hWifi::connect(const NetworkConfig& config) {
    std::vector<std::string> args = {
        "husarion-wifi", "connect", config.ssid, config.password
    };

    cmd(args);
}

void _hWifi::connect(const NetworkConfig* configs, int count) {
    for (int i=0; i < count; i ++)
        connect(configs[i]);
}

bool _hWifi::scan(std::vector<ScannedNetwork>& networks) {
    return true;
}

bool _hWifi::isComboModeSupported() {
    return system("husarion-wifi is-combo-mode-supported") == 0;
}

_hWifi Wifi;

bool _Network::isOnline() {
    return system("ip route get 8.8.8.8 >/dev/null 2>&1") == 0;
}

}

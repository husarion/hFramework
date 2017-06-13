/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <hFramework.h>
#include <cstring>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event_loop.h"

namespace hFramework {

hMutex mutex;
esp_err_t eventCallback(void *arg, system_event_t *event);

void init() {
    static bool initDone = false;
    if (initDone) return;
    initDone = true;

    esp_event_loop_init(&eventCallback, NULL);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

void _hWifi::setupAP(const AccessPointConfig& config) {
    hMutexGuard guard(mutex);
    init();

    wifi_config_t ap_config;
    memset(&ap_config, 0, sizeof(ap_config));

    if (strlen(config.ssid) >= sizeof(ap_config.ap.ssid)) {
        fprintf(stderr, "hWifi error: SSID too long\n");
        return;
    }

    if (strlen(config.password) >= sizeof(ap_config.ap.password)) {
        fprintf(stderr, "hWifi error: password too long\n");
        return;
    }

    ESP_ERROR_CHECK( esp_wifi_set_mode((wifi_mode_t)(WIFI_MODE_AP | WIFI_MODE_STA)) );

    ap_config.ap.channel = config.channel;
    strcpy((char*)ap_config.ap.ssid, config.ssid);
    strcpy((char*)ap_config.ap.password, config.password);

    ap_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    ap_config.ap.ssid_hidden = 0;
    ap_config.ap.max_connection = config.maxConnections;
    ap_config.ap.beacon_interval = 100;

    tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);

    tcpip_adapter_ip_info_t ip_info;
    ip_info.ip.addr = config.ipAddress.v4;
    ip_info.netmask.addr = htonl(0xFFFFFFFF - ((1 << (32 - config.netmask)) - 1));
    ip_info.gw.addr = 0;
    tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &ip_info);

    tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP);

    // TODO: dhcp options

    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &ap_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    fprintf(stderr, "hWifi: started AP (SSID: %s)\n", config.ssid);

#define PRINTFIP(x) ((x)>>0)&0xFF,((x)>>8)&0xFF,((x)>>16)&0xFF,((x>>24))&0xFF
    fprintf(stderr, "hWifi: IP address: %d.%d.%d.%d\n", PRINTFIP(config.ipAddress.v4));
}

void _hWifi::connect(const NetworkConfig& config) {
    connect(&config, 1);
}

void _hWifi::connect(const NetworkConfig* config, int count) {
    hMutexGuard guard(mutex);
    init();

    if (count != 1) {
        fprintf(stderr, "hWifi error: currently only one network is supported\n");
        return;
    }


    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );

    wifi_config_t sta_config;

    if (config->ssid == nullptr) {
        fprintf(stderr, "hWifi error: missing SSID\n");
        return;
    }

    if (strlen(config->ssid) >= sizeof(sta_config.sta.ssid)) {
        fprintf(stderr, "hWifi error: SSID too long\n");
        return;
    }

    if (config->password != nullptr && strlen(config->password) >= sizeof(sta_config.sta.password)) {
        fprintf(stderr, "hWifi error: password too long\n");
        return;
    }

    memset(&sta_config, 0, sizeof(sta_config));
    strcpy((char*)sta_config.sta.ssid, config->ssid);
    if (config->password != nullptr)
        strcpy((char*)sta_config.sta.password, config->password);
    sta_config.sta.bssid_set = false;

    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

}

bool _hWifi::scan(std::vector<ScannedNetwork>& networks) {
    hMutexGuard guard (mutex);
    init();

    ::printf("start scan RAM left: %d\n", esp_get_free_heap_size());
    wifi_scan_config_t cfg;
    cfg.ssid = nullptr;
    cfg.bssid = nullptr;
    cfg.channel = 0;
    cfg.show_hidden = false;
    cfg.scan_type = WIFI_SCAN_TYPE_ACTIVE;
    cfg.scan_time.active.min = 100;
    cfg.scan_time.active.max = 300;
    {
        ::printf("start scan RAM left: %d\n", esp_get_free_heap_size());
        esp_wifi_scan_start(&cfg, true);
        ::printf("finish scan RAM left: %d\n", esp_get_free_heap_size());
    }
    uint16_t num = -1;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&num));
    ::printf("found %d networks\n", num);
    num += 4;
    std::vector<wifi_ap_record_t> records (num);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&num, records.data()));
    networks.resize(num);
    for (int i=0; i < num; i ++) {
        networks[i].isEncrypted = records[i].authmode != WIFI_AUTH_OPEN;
        memcpy(networks[i].ssid, records[i].ssid, sizeof(networks[i].ssid));
        networks[i].quality = records[i].rssi;
    }

    return true;
}

_hWifi Wifi;

esp_err_t eventCallback(void *arg, system_event_t *event) {
    ::printf("wifi event (id=%d)\n", event->event_id);
    if(event->event_id == SYSTEM_EVENT_STA_DISCONNECTED) {
        uint8_t reason = event->event_info.disconnected.reason;
        ::printf("wifi disconnected (reason=%d)", (int)reason);
        if(reason == WIFI_REASON_NO_AP_FOUND ||
           reason == WIFI_REASON_BEACON_TIMEOUT || reason == WIFI_REASON_HANDSHAKE_TIMEOUT ||
           reason == WIFI_REASON_AUTH_EXPIRE) {
             // reattempt connection
             esp_wifi_connect();
        } else if(reason == WIFI_REASON_AUTH_FAIL || reason == WIFI_REASON_ASSOC_FAIL) {

        }
    } else if(event->event_id == SYSTEM_EVENT_STA_START) {

    } else if(event->event_id == SYSTEM_EVENT_STA_STOP) {

    } else if(event->event_id == SYSTEM_EVENT_STA_GOT_IP) {

    }

    return 0;
}
}

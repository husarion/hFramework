#!/bin/bash
set -e

src_dir="$1"
build_dir="$2"
sdk_dir="$3"

archives="
$src_dir/build/app_update/libapp_update.a
$src_dir/build/bootloader_support/libbootloader_support.a
$src_dir/build/bt/libbt.a
$src_dir/build/coap/libcoap.a
$src_dir/build/cxx/libcxx.a
$src_dir/build/driver/libdriver.a
$src_dir/build/esp32/libesp32.a
$sdk_dir/components/esp32/lib/libcore.a
$sdk_dir/components/esp32/lib/librtc.a
$sdk_dir/components/esp32/lib/librtc_clk.a
$sdk_dir/components/esp32/lib/libphy.a
$sdk_dir/components/esp32/lib/libcoexist.a
$sdk_dir/components/esp32/lib/libnet80211.a
$sdk_dir/components/esp32/lib/libpp.a
$sdk_dir/components/esp32/lib/libwpa.a
$sdk_dir/components/esp32/lib/libsmartconfig.a
$sdk_dir/components/esp32/lib/libcoexist.a
$sdk_dir/components/esp32/lib/libwps.a
$sdk_dir/components/esp32/lib/libwpa2.a
$src_dir/build/ethernet/libethernet.a
$src_dir/build/expat/libexpat.a
$src_dir/build/fatfs/libfatfs.a
$src_dir/build/freertos/libfreertos.a
$src_dir/build/json/libjson.a
$src_dir/build/log/liblog.a
$src_dir/build/lwip/liblwip.a
$src_dir/build/mbedtls/libmbedtls.a
$src_dir/build/mdns/libmdns.a
$src_dir/build/micro-ecc/libmicro-ecc.a
$src_dir/build/newlib/libnewlib.a
$src_dir/build/nghttp/libnghttp.a
$src_dir/build/nvs_flash/libnvs_flash.a
$src_dir/build/openssl/libopenssl.a
$src_dir/build/sdmmc/libsdmmc.a
$src_dir/build/spi_flash/libspi_flash.a
$src_dir/build/tcpip_adapter/libtcpip_adapter.a
$src_dir/build/ulp/libulp.a
$src_dir/build/vfs/libvfs.a
$src_dir/build/wpa_supplicant/libwpa_supplicant.a
$src_dir/build/xtensa-debug-module/libxtensa-debug-module.a
$sdk_dir/components/newlib/lib/libc.a
$sdk_dir/components/newlib/lib/libm.a
$sdk_dir/components/esp32/libhal.a"

for archive in $archives; do
    cp "$archive" "$build_dir"
done

cp "$sdk_dir"/components/esp32/ld/*.ld "$build_dir"
cp "$src_dir"/build/esp32/*.ld "$build_dir"
cp "$src_dir"/build/{partitions_ota,bootloader/bootloader}.bin "$build_dir"

#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_PICOSDK
#define MG_OTA MG_OTA_PICOSDK
#define MG_IO_SIZE 2048

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_PICO_W 1
#define MG_ENABLE_DRIVER_W 1
#define MG_ENABLE_PACKED_FS 1

// MAC address is read from OTP by the driver

// Set your Wi-Fi configuration using a custom function:
extern void wifi_setconfig(void *data);
#define MG_SET_WIFI_CONFIG(data) wifi_setconfig(data)

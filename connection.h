#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

struct ConnectionDataBuffer {
    uint8_t id;
    byte instructions;
    byte board_state[8][8];
};

extern ConnectionDataBuffer connection_data_buffer;
extern esp_err_t connection_status;
extern esp_now_peer_info_t peerInfo;
extern uint8_t last_connection_id;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void onDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len);

void startConnection(const uint8_t mac_addr[6]);

void sendData(const uint8_t *mac_addr, ConnectionDataBuffer data);
#include "connection.h"

ConnectionDataBuffer connection_data_buffer;
esp_err_t connection_status;
esp_now_peer_info_t peerInfo;
uint8_t last_connection_id;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    connection_status = status;
}

void onDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    memcpy(&connection_data_buffer, data, ESP_NOW_MAX_DATA_LEN);
}

void startConnection(const uint8_t mac_addr[6]){
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    connection_status = esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceive);

    memcpy(peerInfo.peer_addr, mac_addr, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
}

void sendData( const uint8_t *mac_addr, ConnectionDataBuffer data){
    esp_err_t status;
    while (status != ESP_OK){
        status = esp_now_send(mac_addr, (uint8_t *) &data, sizeof(data));
    }
}
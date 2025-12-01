#include "EspNowBus.h"

#include <esp_now.h>
#include <string.h>

// Stub implementation. Functions return success/failure placeholders until full logic is added.

bool EspNowBus::begin(const Config& cfg) {
    config_ = cfg;
    return false;
}

bool EspNowBus::begin(const char* groupName,
                      bool canAcceptRegistrations,
                      bool useEncryption,
                      uint16_t maxQueueLength) {
    Config cfg;
    cfg.groupName = groupName;
    cfg.canAcceptRegistrations = canAcceptRegistrations;
    cfg.useEncryption = useEncryption;
    cfg.maxQueueLength = maxQueueLength;
    return begin(cfg);
}

void EspNowBus::end() {
    // TODO: tear down tasks/queues/peers
}

bool EspNowBus::sendTo(const uint8_t mac[6], const void* data, size_t len, uint32_t timeoutMs) {
    (void)mac; (void)data; (void)len; (void)timeoutMs;
    return false;
}

bool EspNowBus::sendToAllPeers(const void* data, size_t len, uint32_t timeoutMs) {
    (void)data; (void)len; (void)timeoutMs;
    return false;
}

bool EspNowBus::broadcast(const void* data, size_t len, uint32_t timeoutMs) {
    (void)data; (void)len; (void)timeoutMs;
    return false;
}

void EspNowBus::onReceive(ReceiveCallback cb) {
    onReceive_ = cb;
}

void EspNowBus::onSendResult(SendResultCallback cb) {
    onSendResult_ = cb;
}

bool EspNowBus::addPeer(const uint8_t mac[6]) {
    (void)mac;
    return false;
}

bool EspNowBus::removePeer(const uint8_t mac[6]) {
    (void)mac;
    return false;
}

bool EspNowBus::hasPeer(const uint8_t mac[6]) const {
    (void)mac;
    return false;
}

void EspNowBus::setAcceptRegistration(bool enable) {
    config_.canAcceptRegistrations = enable;
}

bool EspNowBus::sendRegistrationRequest() {
    return false;
}

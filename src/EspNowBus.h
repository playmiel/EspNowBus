#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>

// ESP32 ESP-NOW message bus (design in SPEC.ja.md). Implementation is WIP.
// APIs are stubbed so the library can be included and built while the core logic is developed.

class EspNowBus {
public:
    struct Config {
        const char* groupName;               // Required

        bool useEncryption        = true;
        bool enablePeerAuth       = true;
        bool enableBroadcastAuth  = true;

        uint16_t maxQueueLength   = 16;
        uint16_t maxPayloadBytes  = 1470;
        uint32_t sendTimeoutMs    = 50;
        uint8_t  maxRetries       = 1;
        uint16_t retryDelayMs     = 0;
        uint32_t txTimeoutMs      = 120;

        bool canAcceptRegistrations = true;

        int8_t taskCore      = ARDUINO_RUNNING_CORE; // -1 = unpinned, 0/1 = pinned core
        UBaseType_t taskPriority = 3;
        uint16_t taskStackSize   = 4096;
    };

    // sendTimeout special values
    static constexpr uint32_t kUseDefault = portMAX_DELAY - 1;
    static constexpr uint16_t kMaxPayloadDefault = 1470;
    static constexpr uint16_t kMaxPayloadLegacy  = 250;

    enum SendStatus : uint8_t {
        Queued,
        SentOk,
        SendFailed,
        Timeout,
        DroppedFull,
        DroppedOldest,
        TooLarge,
        Retrying
    };

    using ReceiveCallback = void (*)(const uint8_t* mac, const uint8_t* data, size_t len, bool wasRetry);
    using SendResultCallback = void (*)(const uint8_t* mac, SendStatus status);

    bool begin(const Config& cfg);

    bool begin(const char* groupName,
               bool canAcceptRegistrations = true,
               bool useEncryption = true,
               uint16_t maxQueueLength = 16);

    void end();

    bool sendTo(const uint8_t mac[6], const void* data, size_t len, uint32_t timeoutMs = kUseDefault);
    bool sendToAllPeers(const void* data, size_t len, uint32_t timeoutMs = kUseDefault);
    bool broadcast(const void* data, size_t len, uint32_t timeoutMs = kUseDefault);

    void onReceive(ReceiveCallback cb);
    void onSendResult(SendResultCallback cb);

    bool addPeer(const uint8_t mac[6]);
    bool removePeer(const uint8_t mac[6]);
    bool hasPeer(const uint8_t mac[6]) const;

    void setAcceptRegistration(bool enable);

    bool sendRegistrationRequest();

private:
    Config config_{};
    ReceiveCallback onReceive_ = nullptr;
    SendResultCallback onSendResult_ = nullptr;
};

#include <EspNowBus.h>

// en: Demonstrate JOIN callbacks and send status handling (auto-purge removed).
// ja: JOIN のコールバックと送信ステータスを確認する例（自動パージ機能は廃止）。

EspNowBus bus;

void onReceive(const uint8_t *mac, const uint8_t *data, size_t len, bool wasRetry, bool isBroadcast)
{
  // en: Print sender and payload; app-ACK is auto-sent when enabled.
  // ja: 送信元とペイロードを表示。AppAck は有効時に自動返信。
  Serial.printf("RX from %02X:%02X:%02X:%02X:%02X:%02X data='%s' len=%u retry=%d\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], (const char *)data, (unsigned)len, wasRetry);
}

void onJoinEventCb(const uint8_t mac[6], bool accepted, bool isAck)
{
  // en: Report join events (accepted/denied, request vs ack)
  // ja: JOIN イベントを表示（受理/拒否、ReqかAckか）
  Serial.printf("JoinEvent mac=%02X:%02X:%02X:%02X:%02X:%02X accepted=%d isAck=%d\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], accepted, isAck);
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  EspNowBus::Config cfg;
  cfg.groupName = "espnow-demo_" __FILE__; // en: Group name for communication / ja: 同じグループ名同士で通信可能
  cfg.heartbeatIntervalMs = 5000;          // en: Heartbeat interval in milliseconds(10s->5s) / ja: ハートビート間隔ミリ秒(10秒->5秒)
  // en: ping cadence; 2x -> targeted join, 3x -> drop
  // ja: ping の間隔; 2x でターゲットJOINチャレンジ、3x でピア解消

  bus.onReceive(onReceive);
  bus.onJoinEvent(onJoinEventCb);

  if (!bus.begin(cfg))
  {
    Serial.println("begin failed");
  }
}

void loop()
{
  static uint32_t lastSend = 0;

  if (millis() - lastSend > 3000)
  {
    lastSend = millis();
    size_t peers = bus.peerCount();
    if (peers == 0)
    {
      Serial.println("no peers");
      return;
    }
    uint8_t target[6];
    if (bus.getPeer(0, target))
    {
      const char msg[] = "ping";
      bus.sendTo(target, msg, sizeof(msg));
    }
  }
}

#include <EspNowBus.h>

// en: Slave node — does NOT accept registrations, periodically finds masters and sends sensor data to all peers.
// ja: スレーブノード — 登録を受け付けず、マスター探索と全ピア宛て送信でセンサデータを送る。

EspNowBus bus;

void onReceive(const uint8_t *mac, const uint8_t *data, size_t len, bool wasRetry)
{
  // en: Slaves mainly send; still log any incoming data
  // ja: スレーブは送信主体だが受信もログ
  Serial.printf("RX len=%u retry=%d\n", (unsigned)len, wasRetry);
}

void onSendResult(const uint8_t *mac, EspNowBus::SendStatus status)
{
  // en: Report send status
  // ja: 送信ステータスを表示
  Serial.printf("Send status=%d\n", (int)status);
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  EspNowBus::Config cfg;
  cfg.groupName = "espnow-master";    // en: must match masters / ja: マスターと同じグループ名
  cfg.canAcceptRegistrations = false; // en/ja: スレーブは登録受け入れなし

  bus.onReceive(onReceive);
  bus.onSendResult(onSendResult);

  if (!bus.begin(cfg))
  {
    Serial.println("begin failed");
  }

  // en: initial registration request to find masters
  // ja: 起動直後にマスター探索
  bus.sendRegistrationRequest();
}

void loop()
{
  static uint32_t lastJoinFast = 0;
  static uint32_t lastJoinSlow = 0;
  static uint32_t lastSend = 0;

  size_t peers = bus.peerCount();

  // en: If no peers, search masters more frequently (fast JOIN)
  // ja: ピアがいない時は短周期でマスター探索
  if (peers == 0 && millis() - lastJoinFast > 3000)
  {
    lastJoinFast = millis();
    bus.sendRegistrationRequest();
  }

  // en: Even when peers exist, occasionally refresh JOIN for multi-master
  // ja: ピアがいてもマルチマスタ想定でたまに探索
  if (millis() - lastJoinSlow > 15000)
  {
    lastJoinSlow = millis();
    bus.sendRegistrationRequest();
  }

  // en: Send sensor-like payload to all known peers
  // ja: センサデータ風のペイロードを全ピアに送信
  if (peers > 0 && millis() - lastSend > 5000)
  {
    lastSend = millis();
    char msg[32];
    snprintf(msg, sizeof(msg), "value=%lu", (unsigned long)millis());
    bus.sendToAllPeers(msg, strlen(msg) + 1);
  }
}

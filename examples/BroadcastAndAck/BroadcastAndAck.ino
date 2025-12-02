#include <EspNowBus.h>

// en: Simple periodic broadcast with app-level ACK enabled (default)
// ja: アプリ層ACKを有効にしたシンプルな定期ブロードキャスト

EspNowBus bus;

void onReceive(const uint8_t *mac, const uint8_t *data, size_t len, bool wasRetry)
{
  // en: Print received payload
  // ja: 受信ペイロードを表示
  Serial.printf("RX len=%u retry=%d\n", (unsigned)len, wasRetry);
}

void onSendResult(const uint8_t *mac, EspNowBus::SendStatus status)
{
  // en: Report send status (app-ACK enabled by default)
  // ja: 送信ステータスを表示（デフォルトで論理ACK待ち）
  Serial.printf("Send status=%d\n", (int)status);
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  EspNowBus::Config cfg;
  cfg.groupName = "espnow-demo"; // en: Group name for communication / ja: 同じグループ名同士で通信可能

  bus.onReceive(onReceive);
  bus.onSendResult(onSendResult);

  if (!bus.begin(cfg))
  {
    Serial.println("begin failed");
  }
}

void loop()
{
  static uint32_t last = 0;
  if (millis() - last > 2000)
  {
    last = millis();
    const char msg[] = "hello broadcast";
    bus.broadcast(msg, sizeof(msg));
  }
}

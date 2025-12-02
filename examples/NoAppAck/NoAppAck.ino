#include <EspNowBus.h>

// en: Example with app-level ACK disabled (enableAppAck=false). onAppAck is set but will not be called.
// ja: 論理ACK無効の例（enableAppAck=false）。onAppAck を設定しても呼ばれないことを確認。

EspNowBus bus;

void onReceive(const uint8_t *mac, const uint8_t *data, size_t len, bool wasRetry)
{
  Serial.printf("RX len=%u retry=%d\n", (unsigned)len, wasRetry);
}

void onSendResult(const uint8_t *mac, EspNowBus::SendStatus status)
{
  // en: With app-ACK off, SentOk just means physical send success.
  // ja: 論理ACKオフでは SentOk は物理送信成功のみを意味する。
  Serial.printf("Send status=%d\n", (int)status);
}

void onAppAck(const uint8_t *mac, uint16_t msgId)
{
  // en: Should not be called because enableAppAck=false
  // ja: enableAppAck=false のため呼ばれないはず
  Serial.printf("AppAck (unexpected) msgId=%u\n", msgId);
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  EspNowBus::Config cfg;
  cfg.groupName = "espnow-noappack";
  cfg.enableAppAck = false; // en/ja: disable app-level ACK

  bus.onReceive(onReceive);
  bus.onSendResult(onSendResult);
  bus.onAppAck(onAppAck); // en/ja: set, but won't be called

  if (!bus.begin(cfg))
  {
    Serial.println("begin failed");
  }

  bus.sendRegistrationRequest();
}

void loop()
{
  static uint32_t lastJoin = 0;
  static uint32_t lastSend = 0;

  if (millis() - lastJoin > 5000)
  {
    lastJoin = millis();
    bus.sendRegistrationRequest();
  }

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
      const char msg[] = "no-app-ack";
      bus.sendTo(target, msg, sizeof(msg));
    }
  }
}

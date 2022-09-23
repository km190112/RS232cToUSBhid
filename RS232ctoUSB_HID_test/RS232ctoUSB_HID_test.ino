/*
  RS323C to TTLコンバータと接続し、受信したchar文字を、
  USB HID(UART1)で出力する。

  接続：Seeedino XINO
  ハードウェアシリアル 
  UART1（RX：GPIO7、TX：GPIO6）
  RX：GPIO7 -- TX
  TX：GPIO6 -- RX

  機器側設定
  ボーレート：9600
  ビット長：8ビット、パリティ：なし、ストップビット：なし(SERIAL_8N1)
  非同期シリアル通信(UART)で受信したデータをUSBでデータ送信。
  USB HIDデバイスとして動かすことが出来る。

  ライブラリ(adafruit/Adafruit_TinyUSB_Arduino)
  https://github.com/adafruit/Adafruit_TinyUSB_Arduino
*/

#include <Keyboard.h>

#define BPS0 9600       //シリアル送信側UART0(USB)
#define BPS1 9600       //シリアル受信側UART1


/***********************************************************************
  初期化
 **********************************************************************/
void setup() {
  Keyboard.begin();
  // Serial.begin(BPS0);　// 通信速度bps、UART0
  Serial1.begin(BPS1);  // 通信速度bps、UART1
  delay(50);
}

/***********************************************************************
  UART1から受信シリアル受信
  1バイトづつ読み込みUSB HID送信
   **********************************************************************/
void loop() {
  while (Serial1.available() > 0) {  // 受信したデータバッファが1バイト以上存在する場合

    char inChar = (char)Serial1.read();  // シリアル1からデータ読み込み

    if (inChar == '\n') {         // 改行(LF:0x0a)がある場合の処理
      Serial.println(inChar);
      Keyboard.println(inChar);
      
    } else if (inChar != '\r') {  // 復帰(CR:0x0d)の場合は結合しない
      Serial.print(inChar);
      Keyboard.print(inChar);
      
      } else {
        // 何もしない
      }
    }
  }
  delay(1);
}
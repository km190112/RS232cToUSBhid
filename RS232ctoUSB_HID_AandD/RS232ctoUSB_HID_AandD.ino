/*
  RS323C to TTLコンバータと接続し、設備から出力した信号を変数に格納し、
  改行コード(Cr:0x0D)PCにHID入力する。
  AandDFormatSend()関数は A&D標準フォーマットから値部分のデータを切り出してデータ送信
  RS232Cから受け取るデータ例：ST,+00012.78  g
  マイコンからHIDでPCに出力されるデータ例：12.78
  「ST,-00012.78  g」の場合は「-12.78」が出力される。

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
#define DATA_BUFFER 17  // 文字列の格納領域のバイト数

char resDATA[DATA_BUFFER];  //送信用データ
uint8_t pData = 0;


void AandDFormatSend() {
  // A&D標準フォーマットから値部分のデータを切り出してデータ送信
  // データ例：ST,+00012.78  gCrLf
  //CrLfはresDATA格納時に除いている。

  //HIDでデータをPCに送信マイナスの場合は'-'(0x2d)を送信
  if (resDATA[3] == '-') {
    // Serial.print('-');
    Keyboard.print('-');
  } else if (resDATA[3] != '+') {  // '+'(0x2b)フォーマットエラー
    // Serial.println("Data Format Error");
    return;
  }

  uint8_t pPass = 4;
  do {
    if (resDATA[pPass] != '0') { // 重量値の上位'0'(0x30)埋め値は送信しない
      break;  //数値がゼロ以外になったら抜ける
    }
    pPass++;
  } while (pPass < 8);

  for (uint8_t pSend = pPass; pSend <= 12; pSend++) {
    // Serial.print(resDATA[pSend]);
    Keyboard.print(resDATA[pSend]);
  }

  // Serial.println("");
  Keyboard.println("");

  return;
}


void bufferReset() {
  //char配列とシリアルバッファを消去する。
  for (pData = 0; pData < DATA_BUFFER; pData++) {
    resDATA[pData] = '\0';
  }
  pData = 0;
  while (Serial1.available() > 0) {
    Serial1.read();
  }
}

/***********************************************************************
  初期化
 **********************************************************************/
void setup() {
  Keyboard.begin();
  // Serial.begin(BPS0);　// 通信速度bps、UART0
  Serial1.begin(BPS1);  // 通信速度bps、UART1
  delay(50);

  bufferReset();
}

/***********************************************************************
  UART1から受信シリアル受信
  1バイトづつ読み込みstrDATAに結合しUSB HID送信
   **********************************************************************/
void loop() {
  while (Serial1.available() > 0) {  // 受信したデータバッファが1バイト以上存在する場合

    char inChar = (char)Serial1.read();  // シリアル1からデータ読み込み

    if (inChar == '\n') {  // 改行(LF:0x0a)がある場合の処理
      for (uint i = 0; i < pData; i++) {
        // Serial.print(resDATA[i]);
        // Keyboard.print(resDATA[i]);
      }
      // Serial.println("");
      // Keyboard.println("");

      AandDFormatSend();
      bufferReset();

    } else if (inChar != '\r') {  // 復帰(CR:0x0d)の場合は結合しない

      if (pData < DATA_BUFFER) {
        resDATA[pData] = inChar;
        pData++;

      } else {  //データバッファ溢れが発生する不正データは一旦リセット
        // Serial.println("Buffer over Error");
        bufferReset();
      }
    }
  }
  delay(1);
}
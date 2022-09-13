/*
  RS323C to TTLコンバータと接続し、設備から出力した信号をPCにHID入力する。
  AandDFormatSend()関数は A&D標準フォーマットから値部分のデータを切り出してデータ送信
  RS232Cから受け取るデータ例：ST,+00012.78  g
  マイコンからHIDでPCに出力されるデータ例：12.78
  「ST,-00012.78  g」の場合は「-12.78」が出力される。

  接続：Seeedino XINO
  ハードウェアシリアル 
  UART1（RX：GPIO7、TX：GPIO6）
  RX：GPIO7 -- TX
  TX：GPIO6 -- RX

  機器側通信設定
  ボーレート：9600
  ビット長：8bit、パリティ：なし、ストップビット：なし(SERIAL_8N1)

  ライブラリ(adafruit/Adafruit_TinyUSB_Arduino)
  https://github.com/adafruit/Adafruit_TinyUSB_Arduino
*/

#include <Keyboard.h>

#define BPS0 9600       //シリアル送信側UART0(USB)
#define BPS1 9600       //シリアル受信側UART1
#define DATA_BUFFER 17  // 文字列の格納領域のバイト数

char resDATA[DATA_BUFFER];  //送信用データ
uint pData = 0;


void AandDFormatSend() {
  // A&D標準フォーマットから値部分のデータを切り出してデータ送信
  // データ例：ST,+00012.78  g

  //HIDでデータをPCに送信マイナスの場合は'-'を送信
  if (resDATA[3] == '-') {
    // Serial.print('-');
    Keyboard.print('-');
  } else if (resDATA[3] != '+') {  //フォーマットエラー
    // Serial.println("Data Format Error");
    return;
  }

  uint pPass = 4;
  do {
    if (resDATA[pPass] != '0') {
      break;  //数値がゼロ以外になったら抜ける
    }
    pPass++;
  } while (pPass < 8);

  for (uint pSend = pPass; pSend <= 12; pSend++) {
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
  // Serial.begin(BPS0);   // 通信速度bps、UART0
  Serial1.begin(BPS1);  // 通信速度bps、UART1
  delay(50);

  bufferReset();
}

/***********************************************************************
   UART1から受信シリアル受信
   1バイトづつ読み込みstrDATAに結合してUSB HID送信
   **********************************************************************/
void loop() {
  while (Serial1.available() > 0) {  // 受信したデータバッファが1バイト以上存在する場合

    char inChar = (char)Serial1.read();  // シリアル1からデータ読み込み

    if (inChar == '\n') {  // 改行コードがある場合の処理
      for (uint i = 0; i < pData; i++) {
        // Serial.print(resDATA[i]);
        // Keyboard.println(resDATA[i]);
      }
      // Serial.println("");
      // Keyboard.println("");

      AandDFormatSend();
      bufferReset();

    } else if (inChar != '\r') {  // CRの場合は結合しない(改行コード)

      if (pData < DATA_BUFFER) {
        resDATA[pData] = inChar;
        pData++;

      } else {  //データバッファ溢れが発生する不正データは一旦リセット
        // Serial.println("Buffer over Error");
        delay(5);
        bufferReset();
      }
    }
  }
  delay(1);
}
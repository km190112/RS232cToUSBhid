/*
  規格：Seeedino XINO
  ハードウェアシリアル
  UART1（RX：GPIO7、TX：GPIO6）
  RX：GPIO7 -- TX
  TX：GPIO6 -- RX

  機器側通信設定
  ボーレート：9600
  ビット長：8bit、パリティ：なし、ストップビット：なし(SERIAL_8N1)
*/
#include <Keyboard.h>

// #define bps0 115200  //シリアル送信側UART0(USB)
#define bps1 9600    //シリアル受信側UART1

String strDATA = "";  //送信用データ

void AandDFormatSend() {
  // A&D標準フォーマットから値部分のデータを切り出してデータ送信
  // データ例：ST,+00012.78  g

  //HIDでデータをPCに送信マイナスの場合は'-'を送信
  if (strDATA.charAt(3) == '-') {
    // Serial.print('-');
    Keyboard.print('-');

  } else if (strDATA.charAt(3) != '+') {  //フォーマットエラー
    // Serial.println("Data Format Error");
    return;
  }

  int i = 4;
  do {
    if (strDATA.charAt(i) != '0') {
      break;  //数値がゼロ以外になったら抜ける
    }
    i++;
  } while (i < 8);
  // Serial.println(strDATA.substring(i, 12));
  Keyboard.println(strDATA.substring(i, 12));  // 数値データ送信
  return;
}

/***********************************************************************
   初期化
 **********************************************************************/
void setup() {
  Keyboard.begin();
  // Serial.begin(bps0);   // 通信速度bps、UART0
  Serial1.begin(bps1);  // 通信速度bps、UART1
  delay(50);

  strDATA.reserve(32);  // 文字列の格納領域を指定バイト数確保
}

/***********************************************************************
   UART1から受信シリアル受信
   1バイトづつ読み込みstrDATAに結合してUSB HID送信
   **********************************************************************/
void loop() {
  while (Serial1.available() > 0) {  // 受信したデータバッファが1バイト以上存在する場合

    char inChar = (char)Serial1.read();  // シリアル1からデータ読み込み

    if (inChar == '\n') {  // 改行コードがある場合の処理
      // Keyboard.println(strDATA);
      // Serial.println(strDATA);

      AandDFormatSend();

      strDATA = "";

      // CRの場合は結合しない(改行コード)
    } else if (inChar != '\r') {
      if (strDATA.length() < 16) {
        strDATA += inChar;

      } else {
        strDATA = "";
        while (Serial1.available() > 0) {
          Serial1.read();
        }
      }
    }
  }
  delay(1);
}
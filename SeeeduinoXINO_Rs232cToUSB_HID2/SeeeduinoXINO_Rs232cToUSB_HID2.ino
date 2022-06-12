/*
  Seeeduino XINO USB HID
  ハードウェアシリアル
  UART0 USB
  UART1（RX：GPIO7、TX：GPIO6）機器側TXのみ使用

  ライブラリ(adafruit/Adafruit_TinyUSB_Arduino)
  https://github.com/adafruit/Adafruit_TinyUSB_Arduino

  USB機能を利用するスケッチを書き込むとSeeeduino XIAOポートが消える
  この問題はリセットの操作で解決できます。
  ジャンパー線でこのパッド部に素早く２回ショート(ON/OFF)させて行う。
  ツール→USB Stack→TinyUSB

  機器側設定
  ボーレート：9600
  ビット長：8ビット、パリティ：なし、ストップビット：なし(SERIAL_8N1)

  非同期シリアル通信(UART)で受信したデータをUSBでデータ送信。
  USB HIDデバイスとして動かすことが出来る。

*/
#include <Keyboard.h>

#define BPS0 9600                 // bps UART0(USB)
#define BPS1 9600                 // bps UART1
#define BAFFSIZE 150              // 連続で受信する場合の文字列のバッファ数
#define DATASIZE 80               // 一度に送る文字列のバイト数

char baffArr[BAFFSIZE][DATASIZE]; // バッファ
int ReadX = 0;
int ReadY = 0;
int WriteX = 0;
int WriteY = 0;

#define INTERVAL 900   // 最小送信間隔ms(アプリの入力速度に合わせて変更する)
int count = 0;
String strDATA = "";   // 送信する文字列

/***************************************************************
   初期化
 **************************************************************/
void setup() {
  strDATA.reserve(DATASIZE); // strDATAの文字列の格納領域をbyte数確保
  Keyboard.begin();          // USB HID接続開始
  Serial.begin(BPS0);        // UART0
  Serial1.begin(BPS1);       // UART1
  delay(500);
  //Keyboard.println("USB HID Start!!");
}

/***************************************************************
  1バイトづつSerialで読み込み、baffArrに一旦格納する。strDATAに結合してUSB HIDで送信
   *************************************************************/
void loop() {
  //読み込み
  while (Serial1.available() > 0) {     // 受信したデータバッファが1バイト以上存在する場合
    char inChar = (char)Serial1.read(); // Serial1からデータ読み込み
    if (inChar == '\n') {               // 改行コード(LF)がある場合の処理
      baffArr[ReadX][ReadY] = '\0';
      ReadX += 1;
      ReadX = ReadX % BAFFSIZE;
      ReadY = 0;
    } else if (inChar != '\r') {        // CRの改行コードの場合は結合しない
      baffArr[ReadX][ReadY] = inChar;  // 読み込んだデータを結合
      ReadY += 1;
    }
  }

  //書き出し
  if (INTERVAL <= count) {
    if (ReadX != WriteX) {             //データが保存されている場合に実行
      for (WriteY = 0; WriteY < DATASIZE; WriteY++) {
        if (baffArr[WriteX][WriteY] != '\0') {
          strDATA += baffArr[WriteX][WriteY];
          baffArr[WriteX][WriteY] = '\0';
        } else {
          break;
        }
      }
      if (strDATA.length() > 1) {
        Keyboard.println(strDATA);     // データ送信
      }
      strDATA = "";                    // データ初期化
      WriteX += 1;
      WriteX = WriteX % BAFFSIZE;
    }
  }

  count = count % INTERVAL;
  count += 1;
  delay(1);
}

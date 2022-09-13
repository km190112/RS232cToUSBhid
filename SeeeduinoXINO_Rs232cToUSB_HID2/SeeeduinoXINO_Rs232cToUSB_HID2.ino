/*
  RS323C to TTLコンバータと接続し、設備から出力した信号をPCにHID入力する。
  連続で改行コードを出力するような機器との接続する場合HIDの出力速度より入力速度を上回る。
  ハードウェアシリアル64byteを超える可能性があため、一旦char型の２次元配列に格納し、
  のバッファ溢れを防止する。

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

#define BPS0 9600                 // bps UART0(USB)test用
#define BPS1 9600                 // bps UART1
#define BAFFSIZE 150              // 連続で受信する場合の文字列のバッファ数
#define DATASIZE 80               // 一度に送る文字列のバイト数のヴバッファ数

char baffArr[BAFFSIZE][DATASIZE]; // バッファ
int ReadX = 0;
int ReadY = 0;
int WriteX = 0;
int WriteY = 0;

#define INTERVAL 800   // 最小送信間隔ms(アプリの入力速度に合わせて変更する)
int count = 0;

/***************************************************************
   初期化
 **************************************************************/
void setup() {
  strDATA.reserve(DATASIZE); // strDATAの文字列の格納領域をbyte数確保
  Keyboard.begin();          // USB HID接続開始
  // Serial.begin(BPS0);        // UART0
  Serial1.begin(BPS1);       // UART1
  delay(50);
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
          Keyboard.println(baffArr[WriteX][WriteY]);     // データ送信
          baffArr[WriteX][WriteY] = '\0';
        } else {
          break;
        }
      }
      
      WriteX += 1;
      WriteX = WriteX % BAFFSIZE;
    }
  }

  count = count % INTERVAL;
  count += 1;
  delay(1);
}

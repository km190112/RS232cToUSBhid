# RS232c to USBhid

# **1.概要**

RS232をTTL信号に変換しUARTで受信し、USB HIDでAscii文字を出力する。

活用例

RS232c出力機能がある重量計などの機器とPCの間に接続することで、PCのカーソルを合わせたところに直接キーボード入力できる。

ＵＳＢシリアルドライバーなどPC側の設定は不要。

![IMG_1](https://github.com/km190112/UARTtoUSBhid/assets/46617422/dbb15d3f-9597-4968-860b-1a1ef1c38b7b)
![IMG_3](https://github.com/km190112/UARTtoUSBhid/assets/46617422/1b630abb-3ba1-4212-b958-5a10000cf67e)
![IMG_2](https://github.com/km190112/UARTtoUSBhid/assets/46617422/4cc832f7-c2e2-431c-9232-52013ad98a5b)


# **2.つなぎ方**
装置 - RS232cケーブル - RS232ctoHidモジュール - USBケーブル - PC


# **3.H/W構成

RS232ctoTTLレベル変換モジュール - Seeeduino XINO


# **3.使い方* 
(1)データを送りたい装置の出力設定を以下にする。
  機器側設定
  
  ボーレート：9600bps
  
  ビット長：8ビット
  
  パリティ：None
  
  ストップビット：1bit
  
  (SERIAL_8N1)
  
(2)RS232cとUSBケーブルを変換機に接続

(3)装置からシリアル送信すると、PCのカーソルを合わせたところにキーボード入力するアプリ


# **4.H/Wの作り方**
  
・ハードウェアシリアル  UART1（RX：GPIO 7、TX：GPIO 6）機器側TXのみ使用する。

・写真のようにはんだ付けする。

・RS232cのケーブルのストレート、クロス結線は機器側の仕様を確認して選定する。(Tx-Rxが接続されるようにする)

# **5.seeeduino xinoに書き込むプログラム**

  使用ライブラリ
  (adafruit/Adafruit_TinyUSB_Arduino)
  https://github.com/adafruit/Adafruit_TinyUSB_Arduino

  非同期シリアル通信(UART)で受信したデータをUSBでデータ送信。
  USB HIDデバイスとして動かすことが出来る。

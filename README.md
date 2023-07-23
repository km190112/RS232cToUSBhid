# RS232c to USBhid

# **1.概要**

RS232をTTL信号に変換したものを受信し、USB HIDでAscii文字を出力する。

活用例

・重量計などをキーボード入力などに活用。

# **2.H/W構成**

装置 - RS232cケーブル - RS232ctoTTLレベル変換モジュール - Seeeduino XINO - USBケーブル - PC


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
  
  ハードウェアシリアル
  UART1（RX：GPIO 7、TX：GPIO 6）機器側TXのみ使用


# **5.seeeduino xinoに書き込むプログラム**

  使用ライブラリ
  (adafruit/Adafruit_TinyUSB_Arduino)
  https://github.com/adafruit/Adafruit_TinyUSB_Arduino



  非同期シリアル通信(UART)で受信したデータをUSBでデータ送信。
  USB HIDデバイスとして動かすことが出来る。

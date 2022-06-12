# UARTtoUSBhid
RS232をTTL信号に変換したものをUSB　HID出力する。
活用例
・重量計などをキーボード入力などに活用。

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

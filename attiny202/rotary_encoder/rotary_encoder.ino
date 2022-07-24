// ロータリーエンコーダーの入力をI2Cに流す

#include <Wire.h>
#include <avr/pgmspace.h> 

#define READ_BUF_LEN   8

uint8_t read_buf[READ_BUF_LEN];
uint8_t key_input = 0x00;
uint8_t old_input = 0x00;
uint8_t send_input = 0x00;

// I2Cコマンドを受け取った時の処理
void receiveEvent(int howMany);
void requestEvent();

void setup() {
  pinMode(A7, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);

  // I2C スレーブ初期化
  Wire.begin(0x40);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

}

// I2Cデータを受け取った時の処理
void receiveEvent(int howMany) {
    // コマンドを受け取る
    int i = 0;
    while (Wire.available()){
        if (i < READ_BUF_LEN) {
            read_buf[i] = Wire.read();
            i++;
        } else {
            Wire.read(); // バッファ容量を超えたデータは捨てる
        }
    }
}

// I2Cデータ要求を受け取った時の処理
void requestEvent() {
    // 処理実行
    if (read_buf[0] == 0x10) {
        // 入力状態を返す
        Wire.write(send_input);
        send_input = 0x00;
        
    } else {
        // 不明なコマンドの場合はFFを返す
        Wire.write(0xff);
    }
}

void loop() {
  // key read
  old_input = key_input;
  key_input = 0x00;
  if (digitalRead(A6)) key_input |= 0x01;
  if (digitalRead(A7)) key_input |= 0x02;
  if (old_input == 0x03) {
      if (key_input == 0x01) {
          send_input = 0x01;
      } else if (key_input == 0x02) {
          send_input = 0x02;
      }
  }
  delay(2);

}

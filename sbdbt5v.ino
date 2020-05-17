#include <SoftwareSerial.h>


#define MYRX 2 //デジタル12番ピンはソフトウェアシリアルRX
#define MYTX 3 //デジタル11番ピンはソフトウェアシリアルTX
SoftwareSerial mySerial(MYRX, MYTX);

unsigned char c[8];
unsigned long chksum;


void setup() {
  mySerial.begin(2400);//SBDBTとArduinoは2400bps
  Serial.begin(19200);//シリアルモニター表示
  c[0] = 0x80; //SBDBTからのシリアル信号の１個目は固定。
}

void loop() {
  //まずは無線からシリアルを読み込む。c[1]とc[2]にキー入力が格納される。
  int i;
  if (mySerial.available() >= 8) { //8byte以上あるかチェック
    if (mySerial.read() == 0x80) { //１byte読み込んで0x80のスタートビットかチェック
      Serial.print(c[0], HEX); //１６進数で数値を表示。
      Serial.print(",");//コンマで区切る。
      for (chksum = c[0], i = 1; i < 8; i++) { //スタートビットは読み込み済みなので、次の７個のデータを読み込む。
        c[i] = mySerial.read();
        if (i < 7) chksum += c[i];
        Serial.print(c[i], HEX); //１６進数で数値を表示。
        Serial.print(",");//}//コンマで区切る。
      }
      if (c[7] == (chksum & 0x7F)) { //ボタン部分のみのチェックサムを簡易計算してみる。
        Serial.println("check sum OK !");//チェックサムOKを表示。
      }
      else {
        Serial.println("check sum * * ERROR * *");//ダメならエラーを表示。
      }
      
      //アナログハットの部分
      String leftstickx =  String(c[3], DEC); //左のアナログスティックライトの左右の値を16→10進数へ
      String leftsticky =  String(c[4], DEC); //左のアナログスティックライトの上下の値を16→10進数へ
      String rightstickx =  String(c[5], DEC); //右のアナログスティックライトの左右の値を16→10進数へ
      String rightsticky =  String(c[6], DEC); //右のアナログスティックライトの上下の値を16→10進数へ
      Serial.print(leftstickx);
      Serial.print(leftsticky);
      Serial.print(rightstickx);
      Serial.println(rightsticky);
       Serial.print("\t");
      
      //ここから、キー入力に応じて、メッセージを出す。
      if (c[1] == 0x00 ) { //何も押されていなければ静止
        if (c[2] == 0x00 ) { //何も押されていなければ静止

          Serial.println("* STOP *");
        }
      }
      
      if ((c[2] & 0x01) == 0x01 && (c[2] & 0x02) == 0x02) {
        //if ((c[2] & 0x03 ) == 0x03 ) { //Start(上下同時押しはないと言う前提で書いてるので、注意！）
        Serial.println("Start");

      } else  if ((c[2] & 0x04) == 0x04 && (c[2] & 0x08) == 0x08) {//左右同時押しはないと言う前提で書いてるので、注意！）
        // if ((c[2] & 0x0C ) == 0x0C ) { //Select
        Serial.println("Select");

      } else {


        if ((c[2] & 0x01) == 0x01 ) { //上
          Serial.println("↑Up");    
        }
        if ((c[2] & 0x02) == 0x02 ) { //下
          Serial.println("↓Down");
        }
        if ((c[2] & 0x03 ) == 0x03 ) { //Start
          Serial.println("Start");
        }
        if ((c[2] & 0x04 ) == 0x04 ) { //右
          Serial.println("→Right");
        }
        if ((c[2] & 0x08 ) == 0x08 ) { //左
          Serial.println("←Left");
        }
        if ((c[2] & 0x10 ) == 0x10 ) { //三角
          Serial.println("Triangle△");
        }
        if ((c[2] & 0x20 ) == 0x20 ) { //バツ
          Serial.println("×Cross");
        }
        if ((c[2] & 0x40 ) == 0x40 ) { //マル
          Serial.println("○Circle");
        }
        if ((c[1] & 0x01 ) == 0x01 ) { //四角
          Serial.println("□Square");
        }
        if ((c[1] & 0x02 ) == 0x02 ) { //L1
          Serial.println("L1");
        }
        if ((c[1] & 0x04 ) == 0x04 ) { //L2
          Serial.println("L2");
        }
        if ((c[1] & 0x08 ) == 0x08 ) { //R1
          Serial.println("R1");
        }
        if ((c[1] & 0x10 ) == 0x10 ) { //R2
          Serial.println("R2");
        }
      }
    }
  }
}

//1st byte:0x80
//2nd-3rd byte:button
//4-7th byte:analogstick
//8th byte:checksum(2nd~7th byteまでの加算した結果の下位7bit)
//↑：80 00 01 40 40 40 40 01
//↓：80 00 02 40 40 40 40 02
//→：80 00 04 40 40 40 40 04
//←：80 00 08 40 40 40 40 08
//△:80 00 10 40 40 40 40 10
//×:80 00 20 40 40 40 40 20
//○:80 00 40 40 40 40 40 40
//□:80 01 00 40 40 40 40 01
//L1:80 02 00 40 40 40 40 02
//L2:80 04 00 40 40 40 40 04
//R1:80 08 00 40 40 40 40 08
//R2:80 10 00 40 40 40 40 10
//(Start):80 00 03 40 40 40 40 03
//(Select):80 00 0C 40 40 40 40 0C
//左アナログ左右:80 00 00 XX 40 40 40 XX
//左アナログ上下:80 00 00 00 XX 40 40 XX
//右アナログ左右:80 00 00 00 40 XX 40 XX
//右アナログ上下:80 00 00 00 40 40 XX XX
//詳しくは、http://runele.jp/ps3/PS3-um.pdf

#include <SoftwareSerial.h>
#define MYRX 12 //デジタル12番ピンはソフトウェアシリアルRX
#define MYTX 11 //デジタル11番ピンはソフトウェアシリアルTX
SoftwareSerial mySerial(MYRX, MYTX);
unsigned char c[8];
unsigned long chksum;
int direction(int x,int y){
  double deg,rad;
  int pattern = 0;
  x -= 64; //中心(0,0)の座標平面に変換
  y = abs(y -128) - 64;
  rad = atan2((double)y,(double)x);
  deg = 180 + (rad * 180.0 /(atan(1.0) * 4.0)); //ラジアンを度に変換し、0<=deg<=360として方向を判別
  if(abs(x) > 16 || abs(y)> 16){ // x,yが16以下の時は0を返す
    //pattern = 12;
    for(int i = 0;i < 12;i++){
      if(deg >= 0.0 + (i * 30.0) && deg < 30.0 + (i * 30.0))
        pattern = i + 1;
    }
    Serial.print(deg);
    Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(pattern);
  }
  return pattern;
}
void setup() {
  // put your setup code here, to run once:
   mySerial.begin(2400);//SBDBTとArduinoは2400bps
    Serial.begin(19200);//シリアルモニター表示
    c[0] = 0x80; //SBDBTからのシリアル信号の１個目は固定。
}
void mortarrun(int pin1,int pin2,int power){
  if ( 0<=power){
    analogWrite(pin1,power);
    analogWrite(pin2,0);
  }
  else{
    analogWrite(pin1,0);
    analogWrite(pin2,-power);
  }
}
void Stop(){
}
  void Up(int x, int y){
  }
  void rightup(int power){
 }
  void Right(int x, int y){
 }
 void rightdown(int power){
 }
  void Down(int x, int y){
 }
 void leftdown(int power){
 }
 void Left(int x, int y){
 }
 void leftup(int power){
 }
void loop() {
  int i;
      String leftstickx =  String(c[3], DEC); //左のアナログスティックライトの左右の値を16→10進数へ
      String leftsticky =  String(c[4], DEC); //左のアナログスティックライトの上下の値を16→10進数へ
      String rightstickx =  String(c[5], DEC); //右のアナログスティックライトの左右の値を16→10進数へ
      String rightsticky =  String(c[6], DEC); //右のアナログスティックライトの上下の値を16→10進数へ
      int Lx = leftstickx.toInt();
      int Ly = leftsticky.toInt();
      Serial.print(Ly);
      int powerx = map(abs(Lx-64),0,64,0,200);
      int powery = map(abs(-64-Ly),0,64,0,200);
      int power  = sqrt(pow(powerx,2) + pow(powery,2) );
      //Serial.print(powerx); //１６進数で数値を表示。
      //Serial.print(",");//}//コンマで区切る。
      //Serial.print(powery); //１６進数で数値を表示。
      //Serial.println(",");//}//コンマで区切る。
  // put your main code here, to run repeatedly:
  if (mySerial.available() >= 8) { //8byte以上あるかチェック
      if (mySerial.read() == 0x80) { //１byte読み込んで0x80のスタートビットかチェック
         // Serial.print(c[0], HEX); //１６進数で数値を表示。
          //Serial.print(",");//コンマで区切る。
          for (chksum = c[0], i = 1; i < 8; i++) { //スタートビットは読み込み済みなので、次の７個のデータを読み込む。
            c[i] = mySerial.read();
            if (i < 7) chksum += c[i];
            //Serial.print(c[i], HEX); //１６進数で数値を表示。
            //Serial.print(",");//}//コンマで区切る。
        }
        if(Lx <32 || 98 < Lx || Ly <32 || 98 < Ly)
          switch(direction(Lx,Ly)){
            case 0:
              Stop();
              break;
            case 1:
             // Serial.print(F("\r\nLeft"));
              Right(powerx,powery);
              break;
            case 12:
              //Serial.print(F("\r\nLeft"));
              Right(powerx,powery);
              break;
            case 2:
              //Serial.print(F("\r\nDown+Left"));
              //Down_Left();
              break;
            case 3:
              //Serial.print(F("\r\nUp"));
              Up(powerx,powery);
              break;
            case 4:
             // Serial.print(F("\r\nDown"));
              Up(powerx,powery);
              break;
            case 5:
              //Serial.print(F("\r\nDown+Right"));
              //Down_Right();
              break;
            case 6:
             // Serial.print(F("\r\nRight"));
              Left(powerx,powery);
              break;
            case 7:
             // Serial.print(F("\r\nRight"));
              Left(powerx,powery);
              break;
            case 8:
             // Serial.print(F("\r\nUp+Right"));
              //Up_Right();
              break;
            case 9:
            case 10:
             //Serial.print(F("\r\nDown"));
              Down(powerx,powery);
              break;
            case 11:
             // Serial.print(F("\r\nDown"));
              Down(powerx,powery);
              break;
            default:
              Stop();
              break;
            }
            delay(40);
      if (c[7] == (chksum & 0x7F)) { //ボタン部分のみのチェックサムを簡易計算してみる。
         // Serial.println("check sum OK !");//チェックサムOKを表示。
      }
      else {
        Serial.println("check sum * * ERROR * *");//ダメならエラーを表示。
      }
      //ここから、キー入力に応じて、メッセージを出す。
      if (c[1] == 0x00 ) { //何も押されていなければ静止
        if (c[2] == 0x00 ) { //何も押されていなければ静止
          Serial.println("* STOP *");
          Stop();
        }
      }
      if ((c[2] & 0x01) == 0x01 && (c[2] & 0x02) == 0x02) {
        //if ((c[2] & 0x03 ) == 0x03 ) { //Start(上下同時押しはないと言う前提で書いてるので、注意！）
        Serial.println("Start");
      } else  if ((c[2] & 0x04) == 0x04 && (c[2] & 0x08) == 0x08) {//左右同時押しはないと言う前提で書いてるので、注意！）
        if ((c[2] & 0x0C ) == 0x0C ) { //Select
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

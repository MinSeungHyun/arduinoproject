#include <DynamixelWorkbench.h>

#define DXL_BUS_SERIAL1 "1"            //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 "2"            //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 "3"            //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#define DXL_BUS_SERIAL4 "/dev/ttyUSB0" //Dynamixel on Serial3(USART3)  <-OpenCR

#define BAUDRATE  1000000
#define DXL_UD 1
#define DXL_UD2 2
#define DXL_LR 11
#define DXL_FB 6
#define DXL_CUT 5
#define DXL_REC 7
#define PICTURE_CENTER_X 960
#define PICTURE_CENTER_Y 540


DynamixelWorkbench dxl_wb;


int pin_X = A0;
int pin_Y = A1;
int pin_SW = A2;
int Direction;
int num;
String coordinates;
int pointx;
int pointy;
int serialCheck = 0;
int tomatoCheck = 0;
int left = 0;
int right = 0;
int up = 0;
int down = 0;
int sleft = 0;
int sright = 0;
int sup = 0;
int sdown = 0;

void setup() {
  Serial.begin(9600);
  /*
    Serial.begin(57600);
    while (!Serial); // Open a Serial Monitor
  */

  dxl_wb.begin(DXL_BUS_SERIAL1, BAUDRATE);

  dxl_wb.ping(DXL_UD);
  dxl_wb.ping(DXL_UD2);
  dxl_wb.ping(DXL_LR);
  dxl_wb.ping(DXL_FB);
  dxl_wb.ping(DXL_CUT);
  dxl_wb.ping(DXL_REC);

  dxl_wb.wheelMode(DXL_UD);
  dxl_wb.wheelMode(DXL_UD2);
  dxl_wb.wheelMode(DXL_LR);
  dxl_wb.wheelMode(DXL_FB);
  dxl_wb.jointMode(DXL_CUT);
  dxl_wb.jointMode(DXL_REC);

  pinMode(pin_SW, INPUT_PULLUP);
  delay(10000);
}

void loop() {

  scanTomato();
  //Serial.print("coordinates : ");
  //Serial.println(coordinates);
  handling();
}

void scanTomato() {                 //토마토를 발견할 떄까지 탐색하는 함수
  while (1) {
    if (Serial.available()) {
      while (Serial.available()) {
        char c = Serial.read();
        coordinates += c;
      }
      checkxy();
      findTomato();
    }

    for (int i = 0; i < 100; i++)
    {
      int z = digitalRead(pin_SW);
      if (!z) {
        Serial.println("hanlingMode");
        handling();
      }
      if (Serial.available())
      {
        serialCheck = 2;
        while (Serial.available()) {
          char c = Serial.read();
          coordinates += c;
        }
        checkxy();
        findTomato();
      }

      else {
        dxl_wb.goalSpeed(DXL_LR, -900);
        delay(100);
        right++;
      }
      //left
    }

    dxl_wb.goalSpeed(DXL_UD, -700);
    dxl_wb.goalSpeed(DXL_UD2, -700);
    delay(2000);
    dxl_wb.goalSpeed(DXL_UD, 0);
    dxl_wb.goalSpeed(DXL_UD2, 0);
    delay(500);
    down++;
    //up

    for (int i = 0; i < 100; i++)
    {
      int z = digitalRead(pin_SW);
      if (!z) {
        Serial.println("hanlingMode");
        handling();
      }
      if (Serial.available())
      {
        serialCheck  = 2;
        while (Serial.available()) {
          char c = Serial.read();
          coordinates += c;
        }
        checkxy();
        findTomato();
      }

      else {
        dxl_wb.goalSpeed(DXL_LR, 900);
        delay(100);
        left++;
      }
      //right
    }

    dxl_wb.goalSpeed(DXL_UD, -700);
    dxl_wb.goalSpeed(DXL_UD2, -700);
    delay(2000);
    dxl_wb.goalSpeed(DXL_UD, 0);
    dxl_wb.goalSpeed(DXL_UD2, 0);
    delay(500);
    down++;
    //up
  }
}

void checkxy() {
  if (serialCheck != 2) {
    Serial.print("serialCheck: ");
    Serial.println(serialCheck);
    if (Serial.available()) {
      while (Serial.available()) {
        char c = Serial.read();
        coordinates += c;
      }
    }
  }
  String x = coordinates.substring(0, coordinates.indexOf(','));
  String y = coordinates.substring(coordinates.indexOf(',') + 1);
  pointx = x.toInt();
  pointy = y.toInt();
  serialCheck = 1;
}

void findTomato() {                                                //좌표값을 받은 후 토마토까지 로봇을 움직이는 함

  while (serialCheck != 10) {
    checkxy();
    Serial.print("current x axis: ");
    Serial.println(pointx);
    if (pointx == 1) { //현재 토마토 위치가 사진의 중심보다 좌측에 있을 경우
      dxl_wb.goalSpeed(DXL_LR, -1023);
      delay(10);
      sright++;
    }
    if (pointx == 3) { //우측에 있을 경우
      Serial.println("chekcheck");
      dxl_wb.goalSpeed(DXL_LR, 1023);
      delay(10);
      sleft++;
    }
    if (pointx == 2) {
      Serial.println("success");
      serialCheck = 10;
      tomatoCheck++;
      dxl_wb.goalSpeed(DXL_LR, 0);
      delay(1000);
    }                                                 //x위치 맞춤
    //dxl_wb.goalSpeed(DXL_LR, 0);
    coordinates = "";
  }
  while (serialCheck != 11) {
    checkxy();
    Serial.print("current y axis: ");
    Serial.println(pointy);
    if (pointy == 1) { //현재 토마토 위치가 사진의 중심보다 위에 있을 경우
      dxl_wb.goalSpeed(DXL_UD, -1000);
      dxl_wb.goalSpeed(DXL_UD2, -1000);
      delay(10);
      sdown++;
    }
    if (pointy == 3) { //아래ㄴㄴㄴ 있을 경우
      Serial.println("chekcheck");
      dxl_wb.goalSpeed(DXL_UD, 1000);
      dxl_wb.goalSpeed(DXL_UD2, 1000);
      delay(10);
      sup++;
    }
    if (pointy == 2) {
      Serial.println("success");
      serialCheck = 11;
      tomatoCheck++;
      dxl_wb.goalSpeed(DXL_UD, 0);
      dxl_wb.goalSpeed(DXL_UD2, 0);
      delay(1000);
    }
    coordinates = "";
  }

  if (tomatoCheck == 2) {                               //x y 모두 찾음
    serialCheck = 1;
    tomatoCheck = 0;
    Serial.println("ready complete!!!!");
    catchTomato();
    goBackHome();
    delay(2000);
    scanTomato();
  }
}

void catchTomato() {         //토마토 잡는 함수
  dxl_wb.goalSpeed(DXL_UD, -800);
  dxl_wb.goalSpeed(DXL_UD2, -800);
  delay(2000);
  dxl_wb.goalSpeed(DXL_UD, 0);
  dxl_wb.goalSpeed(DXL_UD2, 0);
  delay(500);
  dxl_wb.goalPosition(DXL_REC, 100);
  delay(1000);
  dxl_wb.goalSpeed(DXL_FB, -1023);
  delay(6000);
  dxl_wb.goalSpeed(DXL_FB, 0);
  delay(100);
  dxl_wb.goalPosition(DXL_REC, 600);
  delay(1000);
  for (int i = 0; i < 2; i++) {
    dxl_wb.goalPosition(DXL_CUT, 545);
    delay(800);
    dxl_wb.goalPosition(DXL_CUT, 200);
    delay(800);
  }
  dxl_wb.goalSpeed(DXL_FB, 1023);
  delay(6000);
  dxl_wb.goalSpeed(DXL_FB, 0);
  delay(100);
  dxl_wb.goalPosition(DXL_REC, 100);
  delay(1000);
  dxl_wb.goalPosition(DXL_REC, 600);
  delay(1000);
  //handling();
  //return 0                                                         //프로그램 종료
}


void handling() {          //직접 조이스틱을 이용해 수동으로 토마토를 수확하는 함수
  delay(1000);
  while (1) {
    Serial.println(num);
    int x = analogRead(pin_X);
    int y = analogRead(pin_Y);
    int z = digitalRead(pin_SW);


    SangHaZwaWoo(x, y, 700, 800, 700, 800, 0);  //가운데
    SangHaZwaWoo(x, y, 1000, 1023, 700, 800, 1);  //위
    SangHaZwaWoo(x, y, 0, 60, 600, 800, 2); //아래
    SangHaZwaWoo(x, y, 700, 800, 0, 10, 3); //왼쪽
    SangHaZwaWoo(x, y, 700, 800, 1000, 1023, 4);  //오른쪽

    if (num == 0) {
      dxl_wb.goalSpeed(DXL_UD, 0);
      dxl_wb.goalSpeed(DXL_UD2, 0);
      dxl_wb.goalSpeed(DXL_LR, 0);
      dxl_wb.goalSpeed(DXL_FB, 0);
    }
    if (num == 1) {
      dxl_wb.goalSpeed(DXL_UD, -700);
      dxl_wb.goalSpeed(DXL_UD2, -700);
      delay(50);
      num = 0;
    }
    if (num == 2) {
      dxl_wb.goalSpeed(DXL_UD, 700);
      dxl_wb.goalSpeed(DXL_UD2, 700);
      delay(50);
      num = 0;
    }
    if (num == 3) {
      dxl_wb.goalSpeed(DXL_LR, -900);
      dxl_wb.goalSpeed(DXL_LR, -900);
      delay(50);
      num = 0;
    }
    if (num == 4) {
      dxl_wb.goalSpeed(DXL_LR, 900);
      dxl_wb.goalSpeed(DXL_LR, 900);
      delay(50);
      num = 0;
    }
    if (!z) {
      serialCheck = 100;
      scanTomato();
    }
  }
}


void goBackHome() {

  if (sleft < sright)
  {
    for (int i = sright - sleft; i > 0; i--)
    {
      dxl_wb.goalSpeed(DXL_LR, 1023);
      delay(10);
    }
    dxl_wb.goalSpeed(DXL_LR, 0);
    delay(100);
  }

  else if (sright <= sleft)
  {
    for (int i = sleft - sright; i > 0; i--)
    {
      dxl_wb.goalSpeed(DXL_LR, -1023);
      delay(10);
    }
    dxl_wb.goalSpeed(DXL_LR, 0);
    delay(100);
  }

  if (sup < sdown)                                       //아래로 내려가야 하는 경우
  {
    for (int i = sdown - sup; i > 0; i--)
    {
      dxl_wb.goalSpeed(DXL_UD, 1000);
      dxl_wb.goalSpeed(DXL_UD2, 1000);
      delay(10);
    }
    dxl_wb.goalSpeed(DXL_UD, 0);
    dxl_wb.goalSpeed(DXL_UD2, 0);
    delay(100);
  }

  else if (sdown < sup)                                  //위로 올라가야 하는 경우
  {
    for (int i = sup - sdown; i > 0; i--)
    {
      dxl_wb.goalSpeed(DXL_UD, -1000);
      dxl_wb.goalSpeed(DXL_UD2, -1000);
      delay(10);
    }
    dxl_wb.goalSpeed(DXL_UD, 0);
    dxl_wb.goalSpeed(DXL_UD2, 0);
    delay(100);
  }

  else if (sup == sdown)
  {
    dxl_wb.goalSpeed(DXL_UD, 0);
    dxl_wb.goalSpeed(DXL_UD2, 0);
    delay(100);
  }



  if (left <= right)
  {
    for (int i = right - left; i > 0; i--)
    {
      dxl_wb.goalSpeed(DXL_LR, 900);
      delay(100);
    }
    dxl_wb.goalSpeed(DXL_LR, 0);
    delay(100);
  }

  for (int i = down; i > 0; i--)
  {
    dxl_wb.goalSpeed(DXL_UD, 700);
    dxl_wb.goalSpeed(DXL_UD2, 700);
    delay(2000);
  }
  dxl_wb.goalSpeed(DXL_UD, 0);
  dxl_wb.goalSpeed(DXL_UD2, 0);
  delay(100);
  left, right, up, down, sright, sup, sdown = 0;
}


void SangHaZwaWoo(int x, int y, int minx, int maxx, int miny, int maxy, int Dir)
{
  if ((minx <= x && x <= maxx) && (y <= maxy && y >= miny))
  {
    num = Dir;
  }

}

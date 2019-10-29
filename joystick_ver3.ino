#include <DynamixelWorkbench.h>

#define DXL_BUS_SERIAL1 "1"            //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 "2"            //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 "3"            //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#define DXL_BUS_SERIAL4 "/dev/ttyUSB0" //Dynamixel on Serial3(USART3)  <-OpenCR

#define BAUDRATE  1000000
#define DXL_UD 1
#define DXL_UD2 2
#define DXL_UD3 3

DynamixelWorkbench dxl_wb;


int pin_X = A0;
int pin_Y = A1;
int pin_SW = A2;
int Direction;
int num;

void setup() {
  Serial.begin(57600);
  while (!Serial); // Open a Serial Monitor

  dxl_wb.begin(DXL_BUS_SERIAL1, BAUDRATE);
  
  dxl_wb.ping(DXL_UD);
  dxl_wb.ping(DXL_UD2);
  dxl_wb.ping(DXL_UD3);
  
  dxl_wb.wheelMode(DXL_UD);
  dxl_wb.wheelMode(DXL_UD2);
  dxl_wb.wheelMode(DXL_UD3);
  
  pinMode(pin_SW, INPUT_PULLUP);
}

void loop() {

  int x = analogRead(pin_X);
  int y = analogRead(pin_Y);

  Serial.print("X : ");
  Serial.print(x);
  Serial.print("Y : ");
  Serial.println(y);

  sanghazwawoo(x, y, 700, 800, 700, 800, 0);  //가운데
  sanghazwawoo(x, y, 1000, 1023, 700, 800, 1);  //위
  sanghazwawoo(x, y, 0, 60, 600, 800, 2); //아래
  sanghazwawoo(x, y, 700, 800, 0, 10, 3); //왼쪽
  sanghazwawoo(x, y, 700, 800, 1000, 1023, 4);  //오른쪽

  Serial.print("Coordinate value: ");
  Serial.println(num);
  
  if(num == 0) {
    dxl_wb.goalSpeed(DXL_UD, 0);
    dxl_wb.goalSpeed(DXL_UD2, 0);
  }
  if (num == 1) {
    Serial.println("go up");
    dxl_wb.goalSpeed(DXL_UD2, -900);
    dxl_wb.goalSpeed(DXL_UD3, -900);
    delay(50);
    num = 0;
  }
  if (num == 2) {
    Serial.println("go down");
    dxl_wb.goalSpeed(DXL_UD2, 900);
    dxl_wb.goalSpeed(DXL_UD3, 900);
    delay(50);
    num = 0;
  }
  if (num == 3) {
    Serial.println("go left");
    dxl_wb.goalSpeed(DXL_UD, -900);
    dxl_wb.goalSpeed(DXL_UD2, -900);
    delay(50);
    num = 0;
  }
  if (num == 4) {
    Serial.println("go right");
    dxl_wb.goalSpeed(DXL_UD, 900);
    dxl_wb.goalSpeed(DXL_UD2, 900);
    delay(50);
    num = 0;
  }

  delay(10);

}




void sanghazwawoo(int x, int y, int minx, int maxx, int miny, int maxy, int Dir)
{
  if ((minx <= x && x <= maxx) && (y <= maxy && y >= miny))
  {
    num = Dir;
  }

}

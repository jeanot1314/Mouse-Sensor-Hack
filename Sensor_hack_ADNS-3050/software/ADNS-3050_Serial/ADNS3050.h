#include <SPI.h>

// SPI and misc pins for the ADNS
#define PIN_SCLK   SCK
#define PIN_MISO   MISO
#define PIN_MOSI   MOSI
#define PIN_NCS    3
#define PIN_MOTION 5
#define NBR_PIXEL 361

// Define all the registers
#define PROD_ID                          0x00
#define REV_ID                           0x01
#define MOTION_ST                        0x02
#define DELTA_X                          0x03
#define DELTA_Y                          0x04
#define SQUAL                            0x05
#define SHUT_HI                          0x06
#define SHUT_LO                          0x07
#define PIX_MAX                          0x08
#define PIX_ACCUM                        0x09
#define PIX_MIN                          0x0a
#define PIX_GRAB                         0x0b
#define MOUSE_CTRL                       0x0d
#define RUN_DOWNSHIFT                    0x0e
#define REST1_PERIOD                     0x0f
#define REST1_DOWNSHIFT                  0x10
#define REST2_PERIOD                     0x11
#define REST2_DOWNSHIFT                  0x12
#define REST3_PERIOD                     0x13
#define PREFLASH_RUN                     0x14
#define PREFLASH_RUN_DARK                0x18
#define MOTION_EXT                       0x1b
#define SHUT_THR                         0x1c
#define SQUAL_THRESHOLD                  0x1d
#define NAV_CTRL2                        0x22
#define MISC_SETTINGS                    0x25
#define RESOLUTION                       0x33
#define LED_PRECHARGE                    0x34
#define FRAME_IDLE                       0x35
#define RESET                            0x3a
#define INV_REV_ID                       0x3f
#define LED_CTRL                         0x40
#define MOTION_CTRL                      0x41
#define AUTO_LED_CONTROL                 0x43
#define REST_MODE_CONFIG                 0x45

char x1[NBR_PIXEL + 1];
int cpt_error=0;

void com_start() {
  digitalWrite(PIN_NCS, HIGH);
  delay(20);
  digitalWrite(PIN_NCS, LOW);
}



byte Read(byte reg_addr) {
  digitalWrite(PIN_NCS, LOW);//begin communication
  // send address of the register, with MSBit = 0 to say it's reading
  SPI.transfer(reg_addr & 0x7f );
  delayMicroseconds(100);
  // read data
  byte data = SPI.transfer(0);
  delayMicroseconds(30);
  //Serial.println(data);
  digitalWrite(PIN_NCS, HIGH);//end communication
  delayMicroseconds(30);

  return data;
}




void Write(byte reg_addr, byte data) {
  digitalWrite(PIN_NCS, LOW);
  //send address of the register, with MSBit = 1 to say it's writing
  SPI.transfer(reg_addr | 0x80 );
  //send data
  SPI.transfer(data);
  delayMicroseconds(30);
  digitalWrite(PIN_NCS, HIGH);//end communication
  delayMicroseconds(30);
}

void startup() {
  //--------Setup SPI Communication---------
  Serial.begin(115200);
  //byte out = 0;
  //byte read = 0;
  //byte bit = 0;
  pinMode(PIN_MISO, INPUT);
  pinMode(PIN_NCS, OUTPUT);
  SPI.begin();
  // set the details of the communication
  //SPI.setBitOrder(MSBFIRST); // transimission order of bits
  //SPI.setDataMode(SPI_MODE3); // sampling on rising edge
  //SPI.setClockDivider(SPI_CLOCK_DIV16); // 16MHz/16 = 1MHz
  SPI.beginTransaction(SPISettings(16000, MSBFIRST, SPI_MODE3));
  delay(10);

  //----------------- Power Up and config ---------------
  com_start();
  Write(RESET, 0x5a); // force reset
  delay(100); // wait for it to reboot
  Write(MOUSE_CTRL, 0x20);//Setup Mouse Control
  Write(MOTION_CTRL, 0x00);//Clear Motion Control register
  delay(100);
  digitalWrite(PIN_NCS, LOW);
}



int convTwosComp(int b) { //Convert from 2's complement
  if (b & 0x80) {
    b = -1 * ((b ^ 0xff) + 1);
  }
  return b;
}

void getXY() { //Prints out X and Y values to the serial terminal, use getX and getY sequentially for most operations
  byte x = 0;
  byte y = 0;
  x = Read(0x03);
  y = Read(0x04);
  if (x != 0 && y != 0) {
    Serial.print("x - ");
    Serial.print(convTwosComp(x));
    Serial.print("  y - ");
    Serial.println(convTwosComp(y));
  }
}

byte Read_Camera(byte reg_addr) {
  int i;
  digitalWrite(PIN_NCS, LOW);//begin communication
    //send address of the register, with MSBit = 1 to say it's writing
  //SPI.transfer(0x22 | 0x80 );
  //send data
  //SPI.transfer(0x60);
  // send address of the register, with MSBit = 0 to say it's reading

  // read data
  SPI.transfer(reg_addr & 0x80 );
  SPI.transfer(0xFF);
  delayMicroseconds(100);
  cpt_error=0;
  for (i = 0; i < NBR_PIXEL + 1; i++) {
    SPI.transfer(reg_addr & 0x7f );
    //delayMicroseconds(20);
    delay(1);
    x1[i] = SPI.transfer(0);
    if(x1[i] < 0x80){
      x1[i] = 0x00;
      cpt_error++;
    }
    else{
      x1[i] -= 0x80;
    }
    //delayMicroseconds(30);
    delay(1);
  }
  //Serial.println(data);
  digitalWrite(PIN_NCS, HIGH);//end communication
  delayMicroseconds(30);

  return 0;
}

void getImage() {
  int i;
  char tmp;
  
  Read_Camera(0x0b);

  
  Serial.print("#");
  for (i = 0; i < NBR_PIXEL; i++) {
    Serial.print(x1[i + 1]);
  }
  Serial.println();
}

void getImageSerial() {
  char x[NBR_PIXEL + 1];
  int i, j;
  char tmp;

  for (i = 0; i < NBR_PIXEL + 1; i++) {
    x[i] = Read(0x0b);
    //tmp = Read(0x0b);
    //if(tmp >= 0x80){x[i] = 0x00;}
    //else{x[i] = tmp;}
  }
  Serial.println();
  Serial.println();

  for (i = 0; i < 19; i++) {
    for (j = 0; j < 19; j++) {
      Serial.print(int(x[(i * 19) + j + 1]));
      Serial.print("   ");
      //Serial.print(x[(i*19)+j], BIN);
      //Serial.print("        ");
    }
    Serial.println();
    //for (i = 0; i < 100; i++) {

    //delayMicroseconds(10);
    //delay(1);
  }
  Serial.println();
}

int getX() { //returns the X acceleration value
  byte x = 0;
  x = Read(0x03);
  return (convTwosComp(x));
}

int getY() { //returns the Y acceleration value
  byte y = 0;
  y = Read(0x04);
  return (convTwosComp(y));
}




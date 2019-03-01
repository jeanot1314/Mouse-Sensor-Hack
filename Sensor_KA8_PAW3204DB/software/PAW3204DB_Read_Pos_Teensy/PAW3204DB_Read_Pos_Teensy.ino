
/*
    Author : Jean Perardel

    Communication code between Teensy board and PAW3204DB sensor.
    The code should work with any Arduino board. Just set the appropriate Pin

    The function write_register isn't working yet. But I am on it !

*/

#define CLK_Pin 13
#define SDIO_Pin 14

#define DELAY 10
#define reg_Product_ID_1 0x00
#define reg_Product_ID_2 0x01
#define reg_Motion_Status 0x02
#define reg_Delta_X 0x03
#define reg_Delta_Y 0x04
#define reg_Operation_Mode 0x05
#define reg_Configuration 0x06
#define reg_Write_Protect 0x09

uint8_t read_reg(uint8_t reg);

void setup() {
  pinMode(CLK_Pin, OUTPUT);
  digitalWrite(CLK_Pin, HIGH);
  pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, HIGH);

  Serial.begin(9600);
  Serial.println();
  Serial.println("--------------- START PROGRAM ---------------");
  Serial.println("------------- KA8 Sensor Reader -------------");
  Serial.println();

  uint16_t product_id = read_reg(reg_Product_ID_1) << 4;
  delay(20);
  product_id += read_reg(reg_Product_ID_2) >> 4;
  delay(20);

  if (product_id)
  {
    Serial.print("Communication with KA8 sensor : ");
    Serial.print("Product number : 0x");
    Serial.println(product_id, HEX);
  }
  else
  {
    Serial.println("Bad communication with the device");
  }
  Serial.println();

  // Remove sleep mode but keep LED alive (default 0xB8, no sleep = 0xA0)
  //write_reg(reg_Operation_Mode, 0b10100000)
  // Change the resolution Value (default 0x04, max 0x07, min 0x00)
  Serial.println("Reading several register configuration");
  uint8_t tmp = read_reg(reg_Write_Protect);
  Serial.print("Write protect register value : ");
  Serial.println(tmp, BIN);
  delay(20);

  tmp = read_reg(reg_Configuration);
  Serial.print("Configuration register value : ");
  Serial.println(tmp, BIN);
  delay(20);

  Serial.print("Changing the configuration value : ");
  write_reg(reg_Configuration, value);
  delay(20);
  tmp = read_reg(reg_Configuration);
  delay(20);
  Serial.println(tmp, BIN);

  Serial.println("Start reading position data : ");
}


void loop() {

  uint8_t rms = read_reg(reg_Motion_Status);

  if (rms == 0x84) {
    Serial.print(rms, BIN);
    Serial.print("  ---  X : ");

    uint8_t position_X = read_reg(reg_Delta_X);
    Serial.print(convTwosComp(position_X));

    Serial.print("  --- Y : ");
    uint8_t position_Y = read_reg(reg_Delta_Y);
    Serial.println(convTwosComp(position_Y));

    //write_reg(reg_Configuration, 0x07);

  }
  else if (rms > 0xA0) {
    //Serial.print("Error reserved Pin : ");
    //Serial.println(tmp, BIN);
  }
  else if (rms > 0x08) {
    Serial.print("Error : ");
    Serial.println(rms, BIN);
  }

  delay(20);
}

int convTwosComp(int b) { //Convert from 2's complement
  if (b & 0x80) {
    b = -1 * ((b ^ 0xff) + 1);
  }
  return b;
}

void latch(uint8_t tim)
{
  digitalWrite(CLK_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(CLK_Pin, HIGH);
  delay(tim);
}

uint8_t read_reg(uint8_t reg)
{

  uint8_t data = 0;
  pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, HIGH);
  delayMicroseconds(1);

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(CLK_Pin, LOW);

    if (reg & (0x01 << (7 - i) ) )
    {
      digitalWrite(SDIO_Pin, HIGH);
    }
    else
    {
      digitalWrite(SDIO_Pin, LOW);
    }
    digitalWrite(CLK_Pin, HIGH);
  }

  delayMicroseconds(2);
  pinMode(SDIO_Pin, INPUT);
  delayMicroseconds(1);

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(CLK_Pin, LOW);

    if (digitalRead(SDIO_Pin))
    {
      data |= (0x01 << (7 - i));
    }
    digitalWrite(CLK_Pin, HIGH);
  }
  pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, LOW);
  //delayMicroseconds(10);

  // Latch the data for synchronization
  digitalWrite(CLK_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(CLK_Pin, HIGH);
  delay(2);

  return data;
}

void write_reg(uint8_t reg, uint8_t data)
{
  reg |= (0x01 << 7);
  Serial.print(reg, BIN);
  Serial.print("   ");
  Serial.println(data, BIN);
  //uint8_t data = 0;
  pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, HIGH);
  delayMicroseconds(1);

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(CLK_Pin, LOW);

    if (reg & (0x01 << (7 - i) ) )
    {
      digitalWrite(SDIO_Pin, HIGH);
    }
    else
    {
      digitalWrite(SDIO_Pin, LOW);
    }
    digitalWrite(CLK_Pin, HIGH);
  }

  delayMicroseconds(2);
  //digitalWrite(SDIO_Pin, HIGH);
  //pinMode(SDIO_Pin, INPUT);
  delayMicroseconds(1);

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(CLK_Pin, LOW);

    if (data & (0x01 << (7 - i) ) )
    {
      digitalWrite(SDIO_Pin, HIGH);
    }
    else
    {
      digitalWrite(SDIO_Pin, LOW);
    }
    digitalWrite(CLK_Pin, HIGH);
  }

  //pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, LOW);
  //delayMicroseconds(10);

  // Latch the data for synchronization
  digitalWrite(CLK_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(CLK_Pin, HIGH);
  delay(2);
}







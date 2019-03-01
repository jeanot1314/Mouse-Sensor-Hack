
/*
    Author : Jean Perardel

    Communication code between Teensy board and PAW3204DB sensor.
    The code should work with any Arduino board. Just set the appropriate Pin

    You can set the resolution and sleep mode at the beginning. 
    The noise filter is here to avoid small values printing. 

*/

#define CLK_Pin 13
#define SDIO_Pin 14
#define RESOLUTION 0x06
#define SLEEP_MODE 0xB8 //(default 0xB8) no sleep 0xA0
#define NOISE_FILTER 0x02

#define DELAY 500


#define reg_Product_ID_1 0x00
#define reg_Product_ID_2 0x01
#define reg_Motion_Status 0x02
#define reg_Delta_X 0x03
#define reg_Delta_Y 0x04
#define reg_Operation_Mode 0x05
#define reg_Configuration 0x06
#define reg_Image_Quality 0x07
#define reg_Operation_State 0x08
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

  // This function read and print the configuration registers and set the resolution and sleep mode defined above.
  init_reg();

  Serial.println();
  Serial.println("Start reading position data : ");
}


void loop() {

  uint8_t rms = read_reg(reg_Motion_Status);

  if (0x80 < rms && rms < 0x88) {
    uint8_t position_X = read_reg(reg_Delta_X);
    uint8_t position_Y = read_reg(reg_Delta_Y);

    if (convTwosComp(position_X) < -NOISE_FILTER || NOISE_FILTER < convTwosComp(position_X) || convTwosComp(position_Y) < -NOISE_FILTER || NOISE_FILTER < convTwosComp(position_Y)) {
      if (convTwosComp(position_X) == -88 &&  convTwosComp(position_Y) == 80) {
        Serial.println("Strange value X=-88 & Y=80");
      }
      else {
        //Serial.print(rms, BIN);
        Serial.print("  ---  X : ");
        Serial.print(convTwosComp(position_X));
        Serial.print("  --- Y : ");
        Serial.println(convTwosComp(position_Y));
      }
    }
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

void init_reg()
{
  uint8_t resolution = RESOLUTION;
  uint8_t sleep_mode = SLEEP_MODE;
  uint8_t tmp = 0;

  uint16_t product_id = 0;
  uint8_t product_id_tmp = read_reg(reg_Product_ID_1);
  Serial.print("Product ID 1 register value (default 0x30) : 0x");
  Serial.println(product_id_tmp, HEX);
  product_id = product_id_tmp << 4;
  delay(DELAY);

  product_id_tmp = read_reg(reg_Product_ID_2);
  Serial.print("Product ID 2 register value (default 0x5X) : 0x");
  Serial.println(product_id_tmp, HEX);
  product_id += product_id_tmp >> 4;
  delay(DELAY);

  if (product_id)
  {
    Serial.println(">> Communication with KA8 sensor OK");
    Serial.print("Product number : 0x");
    Serial.println(product_id, HEX);
  }
  else
  {
    Serial.println("Bad communication with the device");
  }
  delay(DELAY);
  Serial.println();


  tmp = read_reg(reg_Operation_Mode);
  Serial.print("Operation Mode register value (default 0xB8) : 0x");
  Serial.println(tmp, HEX);
  delay(DELAY);

  while (tmp != sleep_mode) {
    Serial.println();
    // Change the resolution Value (default 0x04, max 0x07, min 0x00)
    Serial.println("Changing the operation mode to desactivate sleep");
    write_reg(reg_Operation_Mode, sleep_mode);
    delay(DELAY);
    tmp = read_reg(reg_Operation_Mode);
    Serial.print("New operation mode value : ");
    Serial.println(tmp, HEX);
    delay(DELAY);
  }

  tmp = read_reg(reg_Configuration);
  Serial.print("Configuration register value (default 0x04) : 0x");
  Serial.println(tmp, HEX);
  delay(DELAY);

  while (tmp != resolution) {
    Serial.println();
    // Change the resolution Value (default 0x04, max 0x07, min 0x00)
    Serial.println("Changing the configuration value for resolution");
    write_reg(reg_Configuration, resolution);
    delay(DELAY);
    tmp = read_reg(reg_Configuration);
    Serial.print("New configuration value : ");
    Serial.println(tmp, HEX);
    delay(DELAY);
  }

  tmp = read_reg(reg_Image_Quality);
  Serial.print("Image quality register value : 0x");
  Serial.println(tmp, HEX);
  delay(DELAY);

  tmp = read_reg(reg_Operation_State);
  Serial.print("Operation state register value : 0x");
  Serial.println(tmp, HEX);
  delay(DELAY);

  tmp = read_reg(reg_Write_Protect);
  Serial.print("Write protect register value (default 0x00) : 0x");
  Serial.println(tmp, HEX);
  delay(DELAY);
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

  digitalWrite(SDIO_Pin, LOW);

  // Latch the data for synchronization
  digitalWrite(CLK_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(CLK_Pin, HIGH);
  delay(2);
}







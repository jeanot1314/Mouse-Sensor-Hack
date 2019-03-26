
/*
 * This code read an ADNS-5020 sensor data using a Arduino UNO board
 * 
 * inside the loop function you can select : 
 * read_movement(); if you want to output serial Y & X movements
 * read_pixel(); if you want to output the 225 pixels of the image
 * 
 * Use the Qt GUI on the repo to draw the pixels
 * 
 */

#define reg_Product_ID 0x00
#define reg_Register_ID 0x01
#define reg_Motion_Status 0x02
#define reg_Delta_X 0x03
#define reg_Delta_Y 0x04
#define reg_Pixel_Grab 0x0b
#define reg_Mouse_Control 0x0d
#define reg_Chip_Reset 0x3a
#define value_Chip_Reset 0x5a

#define CLK_Pin 5
#define SDIO_Pin 6
#define NCS_Pin 7

#define NUMBER_PIXEL 225
#define RESOLUTION 500
#define DELAY 500

void setup() {
  pinMode(CLK_Pin, OUTPUT);
  digitalWrite(CLK_Pin, HIGH);
  pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, HIGH);
  pinMode(NCS_Pin, OUTPUT);
  digitalWrite(NCS_Pin, HIGH);

  Serial.begin(115200);
  Serial.println();
  Serial.println("--------------- START PROGRAM ---------------");
  Serial.println("---------- ADNS-5020 Sensor Reader -------------");
  Serial.println();
  
  init_reg();

  Serial.println();
  Serial.println("Start reading position data : ");
}

void loop() {

  read_movement();
  //read_pixel();
  delay(20);
}

void read_pixel()
{
  char pix[NUMBER_PIXEL];

  for (int i = 0; i < NUMBER_PIXEL; i++) {
    pix[i] = read_reg(reg_Pixel_Grab);
    
    if (pix[i] & 0x80) {
      pix[i] &= 0x7F;
    }
    else {
      pix[i] = 0x00;
    }
    Serial.print(pix[i], HEX);
    Serial.print(",");
    delay(2);
  }
  Serial.println();
}

void read_movement()
{
  int rms = read_reg(reg_Motion_Status);

  if (rms)
  {
    int position_X = read_reg(reg_Delta_X);
    int position_Y = read_reg(reg_Delta_Y);
    Serial.print(">>> X =  ");
    Serial.print(convTwosComp(position_X));
    Serial.print("    Y =  ");
    Serial.print(convTwosComp(position_Y));
    Serial.println();
  }
}

uint8_t read_reg(uint8_t reg)
{
  uint8_t data = 0;
  digitalWrite(NCS_Pin, LOW);
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
  digitalWrite(NCS_Pin, HIGH);
  delay(2);

  return data;
}

void write_reg(uint8_t reg, uint8_t data)
{
  reg |= (0x01 << 7);

  digitalWrite(NCS_Pin, LOW);
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
  digitalWrite(NCS_Pin, HIGH);
  delay(2);
}


void init_reg()
{

  // Restart the device by writing 0x5a in register 0x3a
  write_reg(reg_Chip_Reset, value_Chip_Reset);
  delay(10);

  uint8_t product_id = read_reg(reg_Product_ID);
  Serial.print("Product ID register value (default 0x12) : 0x");
  Serial.println(product_id, HEX);
  delay(DELAY);

  uint8_t register_id = read_reg(reg_Register_ID);
  Serial.print("Register ID register value : 0x");
  Serial.println(register_id, HEX);
  Serial.println();
  delay(DELAY);

  if (product_id == 0x12)
  {
    Serial.println();
    Serial.println(">> Communication with ADNS-5020 sensor OK");
  }
  else
  {
    Serial.println("Bad communication with the device");
  }

  write_reg(reg_Mouse_Control, RESOLUTION == 500 ? 0x00 : 0x01);
  Serial.print("Setting mouse control to : ");
  Serial.print(RESOLUTION);
  Serial.print("cpi");
  Serial.println();
  delay(DELAY);
  
  uint8_t res = read_reg(reg_Mouse_Control);
  Serial.print("Mouse control is now : ");
  if(res == 0){Serial.println("500cpi");}
  else{Serial.println("1000cpi");}
  Serial.println();
  delay(DELAY);

}

int convTwosComp(int b) { //Convert from 2's complement
  if (b & 0x80) {
    b = -1 * ((b ^ 0xff) + 1);
  }
  return b;
}



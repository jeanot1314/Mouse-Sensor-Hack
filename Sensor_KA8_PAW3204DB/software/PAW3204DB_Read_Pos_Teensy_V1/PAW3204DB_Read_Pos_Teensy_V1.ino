
/*
   Communication code between Teensy board and PAW3204DB sensor

*/

#define CLK_Pin 13
#define SDIO_Pin 14
#define DELAY 1
#define reg_Motion_Status 0x02
#define reg_Delta_X 0x03
#define reg_Delta_Y 0x04

uint8_t read_reg(uint8_t reg);

void setup() {
  pinMode(CLK_Pin, OUTPUT);
  digitalWrite(CLK_Pin, HIGH);
  pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, HIGH);

  Serial.begin(9600);
}

void loop() {
  uint8_t tmp = 0x70;
  read_reg(tmp);
  //if (read_reg(reg_Motion_Status)) {
  /*uint8_t position_X = read_reg(reg_Delta_X);
    uint8_t position_Y = read_reg(reg_Delta_Y);

    Serial.print("Position X : ");
    Serial.print(position_X);
    Serial.print("  --  Position Y : ");
    Serial.println(position_Y);*/

  //}
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
    delayMicroseconds(1);

    if (reg & (0x01 << (7 - i) ) )
    {
      digitalWrite(SDIO_Pin, HIGH);
    }
    else
    {
      digitalWrite(SDIO_Pin, LOW);
    }
    delayMicroseconds(1);

    digitalWrite(CLK_Pin, HIGH);
    delayMicroseconds(1);

  }
  pinMode(SDIO_Pin, INPUT);
  //digitalWrite(SDIO_Pin, HIGH);
  delayMicroseconds(10);

/*
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(CLK_Pin, LOW);
    delayMicroseconds(1);

    if (digitalRead(SDIO_Pin))
    {
      //PORTD |= 0x08;
      //PORTD &= ~(0x01<<i);
      data |= (0x01 << i);
    }
    delayMicroseconds(1);

    digitalWrite(CLK_Pin, HIGH);
    delayMicroseconds(1);
  }
  */
  delayMicroseconds(1);
  pinMode(SDIO_Pin, OUTPUT);
  digitalWrite(SDIO_Pin, HIGH);
  delayMicroseconds(10);
  return data;
}

void write_reg(uint8_t reg)
{

}

uint8_t read_data()
{
  uint8_t data = 0;
  return data;
}

void write_data(uint8_t data)
{

}





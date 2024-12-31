#include <Adafruit_LiquidCrystal.h>
#include <MCP23017.h>

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(5);

#define WEBL_HI 2
#define WEBL_LO 3
#define OEBL_HI 4
#define OEBL_LO 5
#define CEBL_HI 6
#define CEBL_LO 7
#define PBRSTB 8
#define MBINT 9

#define I2C_ADDR_MCP23017_A 0x27
#define I2C_ADDR_MCP23017_D 0x26
#define INPUT16 0xffff
#define OUTPUT16 0
#define PULLUP16 0xffff

#define MBOXBASE 0x40
// Signature
#define CG_SIG  0xA5
#define CO_SIG	0
// Handshake
#define CO_HSK	1
// Command
#define CO_CMD	2
// Status
#define CO_STS	3
// Data[0]-[11]
#define CO_DAT	4
//
// Handshake
#define CH_REQ	0xCC
#define CH_ACK	0x33
//
// Command
// Init
#define CC_INI	0x00
// Console Output
#define CC_COT	0x01 
// Console Input
#define CC_CIN	0x02 
// Console Status
#define CC_CST	0x03
// 
//Status
// OK
#define CS_OK	0x00
// Unknown Command
#define CS_UK	0xFF 

MCP23017 mcp_a(I2C_ADDR_MCP23017_A);
MCP23017 mcp_d(I2C_ADDR_MCP23017_D);

uint16_t Old_val, Val;

void write_shmem(uint16_t addr, uint16_t data);
uint16_t read_shmem(uint16_t addr);
void handle_mbox();

void setup() {
  bool b;

  Serial.begin(9600);
  //randomSeed(analogRead(A0));

  Wire.begin();
  b = mcp_a.begin();
  //Serial.println(b ? "\n\nA:true" : "A:false");
  b = mcp_d.begin();
  //Serial.println(b ? "D:true" : "D:false");

  mcp_a.pinMode16(OUTPUT16);
  mcp_d.pinMode16(INPUT16);
  mcp_d.setPullup16(PULLUP16);
  pinMode(CEBL_HI, OUTPUT);
  pinMode(CEBL_LO, OUTPUT);
  pinMode(WEBL_HI, OUTPUT);
  pinMode(WEBL_LO, OUTPUT);
  pinMode(OEBL_HI, OUTPUT);
  pinMode(OEBL_LO, OUTPUT);
  digitalWrite(CEBL_HI, HIGH);
  digitalWrite(CEBL_LO, HIGH);
  digitalWrite(WEBL_HI, HIGH);
  digitalWrite(WEBL_LO, HIGH);
  digitalWrite(OEBL_HI, HIGH);
  digitalWrite(OEBL_LO, HIGH);

  // set up the LCD's number of rows and columns:
  if (!lcd.begin(20, 4)) {
    Serial.println("Could not init backpack. Check wiring.");
    while(1);
  }
  // Serial.println("Backpack init'd.");
  //lcd.println("Backpack init'd.");
  lcd.setBacklight(HIGH);
  //lcd.setCursor(0, 0);
  //lcd.print("conout");
  //lcd.setCursor(0,1);

  pinMode(PBRSTB, OUTPUT);
  digitalWrite(PBRSTB, HIGH);
//  pinMode(MBINT, INPUT);
//  attachInterrupt(digitalPinToInterrupt(MBINT), handle_mbox, RISING);
}

void loop() {
  //if (read_shmem(MBOXBASE + CO_SIG) == CG_SIG && read_shmem(MBOXBASE + CO_HSK) == CH_REQ) {
  if (read_shmem(MBOXBASE + CO_HSK) == CH_REQ) {
    switch (read_shmem(MBOXBASE + CO_CMD)) {
      case CC_INI:
        write_shmem(MBOXBASE + CO_STS, CS_OK);
        lcd.setCursor(0,0);
        lcd.print("INIT   ");
        break;
      case CC_CIN:
        if (Serial.available() > 0) {
          write_shmem(MBOXBASE + CO_DAT, Serial1.read());
        }
        write_shmem(MBOXBASE + CO_STS, CS_OK);
        lcd.setCursor(0,0);
        lcd.print("CONIN  ");
        break;
      case CC_COT:
        Serial.write(read_shmem(MBOXBASE + CO_DAT));
        lcd.setCursor(0,0);
        lcd.print("CONOUT ");
        break;
      case CC_CST:
        if (Serial.available() > 0) {
          write_shmem(MBOXBASE + CO_DAT, 1);
        } else {
          write_shmem(MBOXBASE + CO_DAT, 0);
        }
        write_shmem(MBOXBASE + CO_STS, CS_OK);
        lcd.setCursor(0,0);
        lcd.print("CONST  ");
        break;
      default:
        write_shmem(MBOXBASE + CO_STS, CS_UK);
        lcd.setCursor(0,0);
        lcd.print("INVALID");
        break;  
    }
    write_shmem(MBOXBASE + CO_HSK, CH_ACK);
  }
}

void write_shmem(uint16_t addr, uint16_t data) {
//  noInterrupts();
  // address output; GPIOA for higher 8 bits and GPIOB for lower 8 bits
  digitalWrite(OEBL_HI, HIGH);  // for safety
  digitalWrite(OEBL_LO, HIGH);
  digitalWrite(CEBL_HI, HIGH);  // WE or CS must be HIGH during all address transitions
  digitalWrite(CEBL_LO, HIGH);
  mcp_a.write16(addr);          // address output
  mcp_d.pinMode16(OUTPUT16);
  mcp_d.write16(data);          // data output

  digitalWrite(CEBL_HI, LOW);   // write higher 8 bits
  digitalWrite(WEBL_HI, LOW);
  delayMicroseconds(1);
  digitalWrite(WEBL_HI, HIGH);
  digitalWrite(CEBL_HI, HIGH);
  _NOP();
  _NOP();

  digitalWrite(CEBL_LO, LOW);   // write lower 8 bits
  digitalWrite(WEBL_LO, LOW);
  delayMicroseconds(1);
  digitalWrite(WEBL_LO, HIGH);
  digitalWrite(CEBL_LO, HIGH);
  _NOP();
  _NOP();

  mcp_d.pinMode16(INPUT16);
//  interrupts();
} // write_shmem

uint16_t read_shmem(uint16_t addr) {
  uint16_t data;
//  noInterrupts();
  // address output; GPIOA for higher 8 bits and GPIOB for lower 8 bits
  digitalWrite(WEBL_HI, HIGH);  // for safety
  digitalWrite(WEBL_LO, HIGH);
  digitalWrite(CEBL_HI, HIGH);  // WE or CS must be HIGH during all address transitions
  digitalWrite(CEBL_LO, HIGH);
  mcp_a.write16(addr);          // address output
  mcp_d.pinMode16(INPUT16);

  digitalWrite(CEBL_HI, LOW);   // read higher 8 bits
  digitalWrite(OEBL_HI, LOW);
  delayMicroseconds(1);
  data = mcp_d.read8(0);
  digitalWrite(OEBL_HI, HIGH);
  digitalWrite(CEBL_HI, HIGH);
  _NOP();
  _NOP();
  data <<= 8;
  digitalWrite(CEBL_LO, LOW);   // read lower 8 bits
  digitalWrite(OEBL_LO, LOW);
  delayMicroseconds(1);
  data += mcp_d.read8(1);
  digitalWrite(OEBL_LO, HIGH);
  digitalWrite(CEBL_LO, HIGH);
  _NOP();
  _NOP();

//  interrupts();
  return (data);
} // read_shmem

void handle_mbox() {
  Serial1.write(read_shmem(MBOXBASE + CO_CMD));
  write_shmem(MBOXBASE + CO_STS, CS_OK);
  write_shmem(MBOXBASE + CO_HSK, CH_ACK);
}
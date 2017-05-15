/*Release v0.2 dd 17 august 2016 of software for sensorboard_HH12 with one
  or two 12 bits encoders (HH-12 or MAB25) for reading azimuth and/or elvation values
  HH12_AZ_and_EL_V0.2 dd 1 july 2016. Arduino software for reading two HH-12 or MAB25
  12 bits azimuth and elevation data are read from 18 bits encoder interface. 
  output from this program is a frame of 6 bytes: del azhi azlo elhi ello 13(hex);
  del is:
  -- D0 for azimuth data only
  -- D1 for elevation data only
  -- D2 for azimuth and elevation data
  
  Azimuth and elevation data is split in into 2 bytes for tansport (high byte and low byte)

  Configure which data to measure at: what_to_measure (at the start of the program)
  
  comments are welcome at: ON4CDU (at) UBA.be
*/

// MAKE YOUR CHOICE
int what_to_measure = 2; // 0 for az only; 1 for el only; 2 for az & el
int refresh_rate = 2 ; // data update rate. Figures between 1(fast) and 5 (slow)are useful 

int clockPin = 7; //output for clock
int CSPin_az = 6; //output to Chip Select for az data
int CSPin_el = 5; //output to Chip Select for el data
int inputPin = 4; //input encoderdata (AZ & EL)

int inputdata;
int ChipSelect; // used for selecting the encoder
int rxtx_control = 3;
int statusdata = 0; // statusdata from encoder
int angledata = 0;  // angle data from encoder
int mask_lo_byte = 0xFF; // 000011111111; mask for getting least significant byte
int shortdelay = 1; // clock delay
//frame
unsigned char del = 0;      // delimiter frame; 
unsigned char AZ_hi_byte = 0; // azimuth high byte value
unsigned char AZ_lo_byte = 0; // azimuth low byte value
unsigned char EL_hi_byte = 0; // elevation high byte value
unsigned char EL_lo_byte = 0; // elevation low byte value
unsigned char endframe = 0x13;  // end of frame byte

void setup()
{
  Serial.begin(9600);
  pinMode(clockPin, OUTPUT); // clock
  pinMode(CSPin_az, OUTPUT); // Chip Select az encoder
  pinMode(CSPin_el, OUTPUT); // Chip Select el encoder
  pinMode(inputPin, INPUT);   // input encoderdata (AZ & EL)
  pinMode(rxtx_control, OUTPUT); // direction control rs485
  pinMode(13, OUTPUT);           // LED indication on transmit
}

void encoder_data()
{
  int inputstream = 0;
  angledata = 0;
  statusdata = 0;
  digitalWrite(ChipSelect, HIGH); // CS high for disabling encoder
  digitalWrite(clockPin, HIGH);   // CLK high
  delay(shortdelay);              // delaytime between readings
  digitalWrite(ChipSelect, LOW);  // CS low for enabling encoder, start transfer
  delayMicroseconds(shortdelay);  // delay for chip initialization
  digitalWrite(clockPin, LOW);    // start clocking
  delayMicroseconds(shortdelay);  // hold low

  for (int i = 0; i < 12; i++) // clock signal, 12 transitions for reading angle data
  {
    digitalWrite(clockPin, HIGH);        // clock goes high
    delayMicroseconds(shortdelay);       // delaytime between readings
    inputstream = digitalRead(inputPin); // read one bit of data from pin
    angledata = ((angledata << 1) + inputstream); // left shift and add pin value
    digitalWrite(clockPin, LOW);
    delayMicroseconds(shortdelay);      // end of one clock cycle
  }
  for (int i = 0; i < 8; i++) // clock signal, 8 transitions for reading status data
  {
    digitalWrite(clockPin, HIGH);  // clock goes high
    delayMicroseconds(shortdelay); //
    inputstream = digitalRead(inputPin); // read one bit of data from pin
    statusdata = ((statusdata << 1) + inputstream);// left shift and add pin value
    digitalWrite(clockPin, LOW);
    delayMicroseconds(shortdelay); // end of one clock cycle
  }
  digitalWrite(ChipSelect, HIGH); // CS high
}

void decode_az_data()
{
  AZ_hi_byte = (angledata) >> 8; // shift 8 bits to form az high byte
  AZ_lo_byte = angledata & mask_lo_byte;
}

void decode_el_data()
{
  EL_hi_byte = (angledata) >> 8; // shift 8 bits to form az high byte
  EL_lo_byte = angledata & mask_lo_byte;
}

void sendframe()
{
  int i;
  char buf [6] = {del, EL_hi_byte, EL_lo_byte, AZ_hi_byte, AZ_lo_byte, endframe};

  for (i = 0; i < 6; i++)
  {
    digitalWrite (13, HIGH); // led on
    digitalWrite (rxtx_control, HIGH);
    Serial.write(buf[i]);
    //Serial.println (buf[i]);
    delay(2);
    digitalWrite (rxtx_control, LOW);
    digitalWrite (13, LOW); // led off
  }
}

void loop()
{
  if ((what_to_measure == 0) ||  (what_to_measure == 2)){
    del = 0xD0;
    ChipSelect = CSPin_az;  // select az encoder
    digitalWrite (CSPin_el,HIGH); //
    encoder_data();         // read encoder angle
    decode_az_data();       // make bytes for transport
  }
  if ((what_to_measure == 1) ||  (what_to_measure == 2)){
    del = 0xD1;
    ChipSelect = CSPin_el;  // select el encoder
    digitalWrite (CSPin_az,HIGH);
    encoder_data();
    decode_el_data();
  }
  if (what_to_measure == 2) {
    del = 0xD2;   
  }
  sendframe();            // send frame
  delay(50 * refresh_rate);
}


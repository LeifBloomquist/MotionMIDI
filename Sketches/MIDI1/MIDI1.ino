// MotionMIDI Arduino Code for the ADXL335

#define CENTER 341        // Assume same for x, y, and z
#define RANGE  100
#define MAX    (CENTER+RANGE)
#define MIN    (CENTER-RANGE)

#define LED 13

void setup()
{
//Serial.begin(31250);     // Raw MIDI
  Serial.begin(38400);     // Direct connect or XBee
  pinMode(LED, OUTPUT);
}

void loop()
{
  digitalWrite(LED, HIGH);      // set the LED on
   
  int x = analogRead(0);        // read analog input pin 0
  int y = analogRead(1);        // read analog input pin 1
  int z = analogRead(2);        // read analog input pin 2
  float m = Magnitude(x,y,z);   // get the overall magnitude
  
  byte xcc = AccelToCC(x);
  byte ycc = AccelToCC(y);
  byte zcc = AccelToCC(z);
  byte mcc = m*127;
  
  sendMIDI(0xB0,20,xcc);   // X = Channel 1, CC#20
  sendMIDI(0xB0,21,ycc);   // Y = Channel 1, CC#21
  sendMIDI(0xB0,22,zcc);   // Z = Channel 1, CC#22
  sendMIDI(0xB0,23,mcc);   // M = Channel 1, CC#23
  
  digitalWrite(LED, LOW);  // set the LED off
  
  delay(100);              // wait 100ms for next reading
}

void sendMIDI(byte cmd, byte data1, byte data2)
{
   Serial.write(cmd);
   Serial.write(data1);
   Serial.write(data2);
}

// Map an input between MIN and MAX to 0 to 127
byte AccelToCC(int in)
{   
  float temp = Scale(in);
  return temp*127;
}

// Scale one of the inputs to between 0.0 and 1.0
float Scale(int in)
{
  // Bound
  if (in > MAX) in=MAX;
  if (in < MIN) in=MIN;
  
  // Change zero-offset
  in = in-MIN;
  
  // Scale between 0.0 and 1.0
  float temp = (float)in/((float)MAX-(float)MIN);
  return temp;   
}

// Get the magnitude of the overall acceleration, with gravity removed.
float Magnitude(int x, int y, int z)
{
  // We want them signed and rescaled betweeen +/- 1.0
  float xf=2.0*(Scale(x) - 0.5);
  float yf=2.0*(Scale(y) - 0.5);
  float zf=2.0*(Scale(z) - 0.5);
  
  float mf =sqrt(sq(xf) + sq(yf) + sq(zf));
   
  // Subtract out gravity.  Our scaling isn't perfect so this is an empirical value.
  mf = abs(mf - 0.7);
  
  // Bound
  if (mf > 1.0) mf = 1.0;

  //Serial.println(mf, DEC); 
  return mf;
}

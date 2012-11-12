// ADXL335 Test Program

// Old Values
// Coincidentally, the difference between these is 127, perfect for MIDI
//#define MIN 270
//#define MAX 397

#define CENTER 371  // Same for x, y, and z
#define RANGE  100
#define MAX    (CENTER+RANGE)
#define MIN    (CENTER-RANGE)

#define LED 13

void setup()
{
//Serial.begin(31250);     // Raw MIDI
  Serial.begin(38400);     // Direct connect
  pinMode(LED, OUTPUT);
}

void loop()
{ 
  int x, y, z;
  byte x1, y1, z1;
  
  digitalWrite(LED, HIGH);   // set the LED on
 
  x = analogRead(0);       // read analog input pin 0
  y = analogRead(1);       // read analog input pin 1
  z = analogRead(2);       // read analog input pin 2

// Serial.println(z, DEC);  // print the acceleration in the X axis for debugging

  x1 = AccelToCC(x);
  y1 = AccelToCC(y);
  z1 = AccelToCC(z);

  sendMIDI(0xB0,20,x1);   // Channel 1, CC#20
  sendMIDI(0xB0,21,y1);   // Channel 1, CC#21
  sendMIDI(0xB0,22,z1);   // Channel 1, CC#22

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
    // Bound
    if (in > MAX) in=MAX;
    if (in < MIN) in=MIN;
    
    // Change zero-offset
    in = in-MIN;
    
    // Scale between 0.0 and 1.0
    float temp = (float)in/((float)MAX-(float)MIN);
    
    //Serial.println(temp, DEC);
    
    return temp*127;      
}

  

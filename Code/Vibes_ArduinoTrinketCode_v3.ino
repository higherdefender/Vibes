// Connect LED positive to pins 9, 10, 11 with common ground.
 
#define MIC_PIN   2       // connected to the out pin on the mic  #4 for Trinket
#defube VIB_PIN   1      // connected to one terminal of the vibration motor #1 for trinket
#define DC_OFFSET 3.3     // mic is plugged into 3.3v 
 
 
const int HI_CUT = 1024;  // upper brightness limit
const int LO_CUT = 0;   // lower iimit turns LEDs off
const int VIB_DELAY = 250; // time for vibration 
 
// sample window width in mS (50 mS = 20Hz)
const int sampleWindow = 50;
unsigned int sample;

void setup() {                
  // Serial.begin(9600);  // remove comment for debugging
 
    pinMode(PWMPin, OUTPUT);
    pinMode(2, OUTPUT);    //connected to the second terminal of vibration motor #2 for trinket
    digitalWrite(2, LOW);
 
}
 
void loop() {
  unsigned long startMillis= millis();  // start of sample window
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  double lvl = 0;
 
  // collect data for 50 mS within a reasonable range
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(MIC_PIN);
 
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;
      } 
      else if (sample < signalMin) {
        signalMin = sample;
      }
    }
  }
  lvl = signalMax - signalMin;            // max - min = peak-peak amplitude
  lvl = ( lvl * DC_OFFSET );                // reduce operational range
  lvl = ( lvl < LO_CUT ? 0 : lvl );         // low cut filter for noise
  lvl = ( lvl > HI_CUT ? HI_CUT : lvl );    // high cut filter for clipping
  
  lvl = map(lvl, 0, 1024, 0, 255);
 
  analogWrite(VIB_PIN, lvl);
  delay(VIB_DELAY)  ;
  analogWrite(VIB_PIN, 0);
  
 
  // Serial.println(lvl);  // remove comment for debugging
}

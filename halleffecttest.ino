/*
 * @note Based on script by Rob van den Tillaart and Ralph Martin
 * @see http://playground.arduino.cc/Code/HallEffect
 *
 * PURPOSE: use an A1301 or A1302 as magnetometer
 *
 * Pin Layout LH Package
 * =====================
 *  1     VCC 5V
 *  2     signal    connected to Analog 0
 *  3     GND
 *
 * Pin Layout UA Package
 * =====================
 *  1     VCC 5V
 *  2     GND
 *  3     signal    connected to Analog 0
 *
 */

#define NOFIELD 505L    // Analog output with no applied field, calibrate this

// Uncomment one of the lines below according to device in use A1301 or A1302
// This is used to convert the analog voltage reading to milliGauss
#define TOMILLIGAUSS 1953L  // For A1301: 2.5mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 1953mG
// #define TOMILLIGAUSS 3756L  // For A1302: 1.3mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 3756mG

#define THRESHOLD_HIGH 100

long prevReading = 0;
int prevDirection = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  long reading = getMeasurement();
  if (abs(reading - prevReading) > 5) {    
    int direction = getDirection(prevReading, reading);
    if (direction != prevDirection && direction < 0 && reading > THRESHOLD_HIGH) {
      tick();
    }
    
    prevReading = reading;
    prevDirection = direction == 0 ? prevDirection : direction;
  }
    
  delay(2);
}

long getMeasurement() {
  int raw = analogRead(0);   // Range : 0..1024

  long compensated = raw - NOFIELD;                 // adjust relative to no applied field
  long gauss = compensated * TOMILLIGAUSS / 1000;   // adjust scale to Gauss

  return gauss;
}

int getDirection(long prev, long curr) {
  return curr == prev ? 0 : abs(curr - prev) / (curr - prev);
}

void tick() {
  Serial.print(',');
}

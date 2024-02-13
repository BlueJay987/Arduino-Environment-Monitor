#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include <dht_nonblocking.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

// Calculate Runtime
String runtime = String(millis()/1000);

// Set Up BMP280
#define BMP280_ADDRESS 0x76
Adafruit_BMP280 bmp;                              

// Set Up LCD
const int LCD_WIDTH = 20;
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Set Up DHT11
#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);


// Check for DHT11 Output
static bool measure_environment(float *temperature, float *humidity) {
  static unsigned long measurement_timestamp = millis();

  /* Measure once every four seconds. */
  if (millis() - measurement_timestamp > 300ul) {
    if (dht_sensor.measure(temperature, humidity) == true) {
      measurement_timestamp = millis();
      return (true);
    }
  }

  return (false);
}

void setup() {
  lcd.begin(20, 4);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 1);

  if (!bmp.begin(BMP280_ADDRESS)) {
    lcd.setCursor(0,2);
    lcd.print("uhhh");
    while (1);
  }

  // Don't Know What This Is But It's Required
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

}

void loop() {
  float temperature;
  float humidity;

  if (measure_environment(&temperature, &humidity) == true) {
    lcd.setCursor(LCD_WIDTH / 2 - 3, 0);
    lcd.print((temperature * 9 / 5) + 32, 1);  // Converts C to F
    lcd.print("F");
    lcd.setCursor(LCD_WIDTH / 2 - 2, 1);
    lcd.print(humidity, 0);
    lcd.print("%");
  }
  lcd.setCursor(LCD_WIDTH/2 - 5, 2);
  lcd.print(bmp.readPressure()/100, 1); // Converts Pascals to Millibars
  lcd.print(" mb");
  lcd.setCursor(LCD_WIDTH/2 - runtime.length()*2, 3);
  lcd.print(millis()/1000);
  lcd.print(" sec.");
}

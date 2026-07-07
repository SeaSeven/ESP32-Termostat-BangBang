#include <OneWire.h>
#include <DallasTemperature.h>
#include <Tasker.h>

const int PIN_ONEWIRE = 16;
const int PIN_PWM = 17;

const float SETPOINT = 50.0;
const float HISTERESI = 0.3;
const float TEMP_MAX = 60.0;

const int PWM_RESOLUCIO = 10;
const int PWM_FREQUENCIA = 5000;
const int PWM_MAX = 1023;

OneWire oneWire(PIN_ONEWIRE);
DallasTemperature sensors(&oneWire);
Tasker tasker;

float temperatura = 0.0;
int potPWM = 0;
bool esperantConversio = false;

void llegirTemperatura() {
  if (!esperantConversio) {
    sensors.requestTemperatures();
    esperantConversio = true;
  }

  if (sensors.isConversionComplete()) {
    temperatura = sensors.getTempCByIndex(0);

    if (temperatura == DEVICE_DISCONNECTED_C) {
      temperatura = -127.0;
    }

    esperantConversio = false;
  }
}

void controlTermostat() {
  if (temperatura > TEMP_MAX) {
    potPWM = 0;
  } else if (temperatura < SETPOINT - HISTERESI) {
    potPWM = PWM_MAX;
  } else if (temperatura > SETPOINT + HISTERESI) {
    potPWM = 0;
  }

  analogWrite(PIN_PWM, potPWM);
}

void enviarSerial() {
  Serial.print(temperatura, 2);
  Serial.print(",");
  Serial.println(SETPOINT);
}

void setup() {
  Serial.begin(115200);

  analogWriteResolution(PIN_PWM, PWM_RESOLUCIO);
  analogWriteFrequency(PIN_PWM, PWM_FREQUENCIA);

  sensors.begin();
  sensors.setResolution(11);
  sensors.setWaitForConversion(false);

  tasker.setInterval(llegirTemperatura, 300);
  tasker.setInterval(controlTermostat, 100);
  tasker.setInterval(enviarSerial, 100);
}

void loop() {
  tasker.loop();
}

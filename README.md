# Termostat Bang-Bang per ESP32-S3

Control d'una resistència calefactora mitjançant un termostat de tipus Bang-Bang amb histèresi, implementat en un microcontrolador ESP32-S3 amb Arduino.

## Objectius d'aprenentatge

- Comprendre el funcionament d'un controlador Bang-Bang amb histèresi
- Programar tasques no bloquejants amb la llibreria Tasker
- Llegir sensors de temperatura DS18B20 amb OneWire
- Generar senyals PWM per controlar actuadors
- Visualitzar dades en temps real amb el Serial Plotter d'Arduino

## Requisits de hardware

| Component | Quantitat | Notes |
|-----------|-----------|-------|
| ESP32-S3 | 1 | Qualsevol variant compatible |
| Sensor DS18B20 | 1 | Temperatura, encapsulat TO-92 o sonda |
| Resistència de 4.7 kΩ | 1 | Pull-up per al bus OneWire |
| Resistència calefactora | 1 | Controlada per PWM |
| Font d'alimentació | 1 | Adequada per a la càrrega |
| Cablejat i protoboard | - | Connexions diverses |

## Connexions

| Senyal | Pin ESP32-S3 | Dispositiu |
|--------|-------------|------------|
| OneWire | GPIO 16 | DS18B20 (data) |
| PWM | GPIO 17 | Resistència calefactora (mitjançant transistor/MOSFET) |
| Alimentació | 3.3V / 5V | DS18B20 (VDD) |
| GND | GND | Comú a tots els dispositius |

> **Important**: La resistència calefactora requereix un transistor o MOSFET per ser controlada des del pin GPIO. NO connecteu el pin directament a una càrrega de potència.

Vegeu [WIRING.md](WIRING.md) per a un esquema detallat de connexions.

## Requisits de software

- [Arduino CLI](https://arduino.github.io/arduino-cli/) o Arduino IDE
- Core ESP32 per Arduino (`arduino-cli core install esp32:esp32`)
- Llibreries:
  - `OneWire` (v2.3.8 o superior)
  - `DallasTemperature` (v4.0.6 o superior)
  - `Tasker` (v3.0.0 o superior)

### Instal·lació de llibreries (Arduino CLI)

```bash
arduino-cli lib install "OneWire"
arduino-cli lib install "DallasTemperature"
arduino-cli lib install "Tasker"
```

## Compilació i càrrega

### Compilar

```bash
arduino-cli compile --fqbn esp32:esp32:esp32s3 termostat_bangbang/
```

### Carregar a la placa

```bash
arduino-cli upload --fqbn esp32:esp32:esp32s3 --port COM3 termostat_bangbang/
```

> Substituïu `COM3` pel port sèrie corresponent al vostre sistema.

## Funcionament

### Control Bang-Bang

El termostat implementa un control de tipus Bang-Bang amb histèresi:

```
Temperatura < 49.7°C  →  PWM = 100% (escalfant)
Temperatura > 50.3°C  →  PWM = 0%   (aturat)
49.7°C ≤ T ≤ 50.3°C   →  Manté estat anterior
```

L'histèresi de ±0.3°C evita les commutacions ràpides al voltant del punt de consigna.

### Seguretat

Si la temperatura supera els **60°C**, el sistema apaga immediatament la resistència independentment de l'estat del controlador.

### Sortida serial

El programa envia dades en format compatible amb el **Serial Plotter** d'Arduino IDE:

```
Treal,Tconsigna
```

Exemple de sortida:
```
49.82,50.00
49.95,50.00
50.08,50.00
```

## Estructura del codi

### Tasques (Tasker)

| Funció | Període | Descripció |
|--------|---------|------------|
| `llegirTemperatura()` | 300 ms | Llegeix el sensor DS18B20 (no bloquejant) |
| `controlTermostat()` | 100 ms | Calcula l'estat del termostat i actualitza PWM |
| `enviarSerial()` | 100 ms | Envia dades al port sèrie |

### Explicació de les funcions

- **`llegirTemperatura()`**: Inicia la conversió del sensor DS18B20 sense esperar (`setWaitForConversion(false)`). Quan la conversió es completa, llegeix la temperatura i l'emmagatzema.

- **`controlTermostat()`**: Implementa la lògica Bang-Bang comparant la temperatura actual amb el setpoint i l'histèresi. Primer verifica la condició de seguretat (>60°C). Aplica el valor PWM calculat al pin de sortida.

- **`enviarSerial()`**: Envia la temperatura real (amb 2 decimals) i la consigna separades per coma, format compatible amb el Serial Plotter.

## Especificacions

El document [Especificacions.txt](Especificacions.txt) conté la descripció detallada dels requisits del projecte.

## Exercicis proposats (per a estudiants)

1. **Modificar el setpoint**: Canvieu la constant `SETPOINT` a 45°C i observeu el comportament.
2. **Ampliar l'histèresi**: Augmenteu `HISTERESI` a 1.0°C i compareu l'estabilitat.
3. **Afegir LED indicador**: Feu parpellejar un LED a diferentes freqüències segons l'estat del termostat (escalfant/aturat).
4. **Implementar un PID**: Modifiqueu el control per utilitzar un PID en lloc de Bang-Bang (vegeu [control_pid](../control_pid/)).
5. **Logging a SD**: Emmagueu les temperatures en una targeta SD per a anàlisi posterior.

## Referències

- [Documentació DS18B20](https://www.analog.com/en/products/ds18b20.html)
- [Llibreria OneWire](https://www.pjrc.com/teensy/td_libs_OneWire.html)
- [Llibreria DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [Llibreria Tasker](https://github.com/arkhipenko/Tasker)
- [Arduino-ESP32 analogWrite](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/analogwrite.html)

## Llicència

Aquest projecte està sota llicència MIT. Vegeu el fitxer [LICENSE](LICENSE).

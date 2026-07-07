# Esquema de connexions

## Termostat Bang-Bang per ESP32-S3

### Components necessaris

| Component | Quantitat | 
|-----------|-----------|
| ESP32-S3 | 1 |
| Sensor DS18B20 | 1 |
| Resistència 4.7 kΩ (pull-up) | 1 |
| Resistència 10 kΩ (pull-down opcional)| 1 |
| Transistor NPN (2N2222 o similar) o MOSFET (IRLZ44N) | 1 |
| Díode 1N4007 (protecció) | 1 |
| Resistència calefactora | 1 |
| Font d'alimentació externa (segons càrrega) | 1 |

### Diagrama de connexions

```
        ┌─────────────────────────────┐
        │         ESP32-S3            │
        │                             │
        │  GPIO 16 ────┬──────────────┤── DS18B20 (Data)
        │              │4.7kΩ         │
        │             └── 3.3V        │
        │                             │
        │  GPIO 17 ────┬──────────────┤── Base/Puerta (Transistor/MOSFET)
        │              │10kΩ          │
        │             └── GND         │
        │                             │
        │  3.3V    ───────────────────┤── DS18B20 (VDD)
        │                             │
        │  GND     ───────────────────┤── DS18B20 (GND)
        │                             │
        │  GND     ───────────────────┤── Font alimentació (GND)
        └─────────────────────────────┘
```

### Connexions detallades

#### DS18B20 (sensor de temperatura)

| Pin DS18B20 | Connectat a |
|-------------|-------------|
| VDD (vermell) | 3.3V (ESP32-S3) |
| GND (negre) | GND (ESP32-S3) |
| Data (groc/blanc) | GPIO 16 + pull-up 4.7 kΩ a 3.3V |

#### Control de la resistència calefactora (mitjançant transistor)

```
           +12V / 24V (externa)
              │
         ┌────┴────┐
         │ Càrrega │
         │ (Resist.│
         │ calefac)│
         └────┬────┘
              │
              ├── Col·lector (NPN) / Drain (MOSFET)
              │
         ┌────┴────┐
         │Transistor│
         │ /MOSFET  │
         └────┬────┘
              │
              ├── GPIO 17 (via R 10kΩ) → Base (NPN) / Gate (MOSFET)
              │
              ├── GND → Emisor (NPN) / Source (MOSFET)

    Díode 1N4007 en paral·lel amb la càrrega (càtode a +V, ànode a col·lector)
```

### Advertències importants

1. **No connecteu la resistència calefactora directament al GPIO 17**. El pin d'un ESP32 pot proporcionar només ~40 mA. Utilitzeu sempre un transistor o MOSFET com a interruptor.

2. **Alimentació externa**: La resistència calefactora requereix una font d'alimentació externa (12V, 24V, etc.). No alimenteu la càrrega des del pin de 3.3V de l'ESP32.

3. **Protecció**: Utilitzeu un díode de roda lliure (1N4007) en paral·lel amb la càrrega inductiva per protegir el transistor.

4. **Pull-up OneWire**: El bus OneWire requereix una resistència de pull-up de 4.7 kΩ entre la línia de dades i 3.3V.

### Verificació

Abans d'alimentar el sistema:

1. Verifiqueu que totes les connexions de GND són comunes
2. Assegureu-vos que la resistència de pull-up del DS18B20 està connectada
3. Comproveu que el transistor/MOSFET està correctament orientat
4. Mesureu la continuïtat amb un multímetre si és possible

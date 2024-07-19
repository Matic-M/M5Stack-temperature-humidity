# M5Stack-temperature-humidity

This project uses an M5Stack to take temperature and humidity readings, upload them to ThingSpeak, and read the data via API to display on the M5Stack's screen and a webpage. Additionally, it uses a relay to manage window opening, which can be controlled both automatically and manually via the webpage.

## Features

- Measure temperature and humidity using a DHT sensor.
- Upload data to ThingSpeak.
- Retrieve data from ThingSpeak API.
- Display current temperature and humidity on the M5Stack display.
- Display current temperature and humidity on a webpage.
- Control window opening using a relay, either automatically or manually via the webpage.

## Components

- M5Stack Core
- DHT11 or DHT22 Sensor
- Relay Module
- USB Cable
- Wi-Fi connection

## Prerequisites

- Arduino IDE installed
- M5Stack library installed
- DHT sensor library installed
- HTTPClient library installed
- Wi-Fi network credentials
- ThingSpeak account and channel setup

## Setup

### Hardware Connections

1. **Connect the DHT sensor to the M5Stack:**
   - VCC to 3.3V
   - GND to GND
   - DATA to GPIO 22

2. **Connect the Relay Module to the M5Stack:**
   - VCC to 5V
   - GND to GND
   - IN to GPIO 26

### ThingSpeak Setup

1. Sign up for a [ThingSpeak](https://thingspeak.com/) account.
2. Create a new channel.
3. Note the **Channel ID** and **Write API Key**.

### Software Setup

1. Install the required libraries in Arduino IDE:
   - M5Stack library
   - DHT sensor library
   - HTTPClient library

2. Clone this repository or copy the code into your Arduino IDE.

3. Replace in the code your Wi-Fi credentials and ThingSpeak API reading/writing keys.

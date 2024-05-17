# Weather Data Scraper

This project is an Arduino-based weather data scraper that connects to a Wi-Fi network, fetches XML data from a specified URL, parses the data, and outputs the weather information such as temperature, humidity, pressure, wind direction, wind velocity, and rainfall.

## Table of Contents
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Code Overview](#code-overview)
- [Contributing](#contributing)
- [License](#license)

## Hardware Requirements
- An Arduino board with Wi-Fi capabilities (e.g., ESP32)
- USB cable for programming and power supply

## Software Requirements
- Arduino IDE
- Wi-Fi network with internet access

## Installation
1. **Clone the repository:**
    ```sh
    git clone https://github.com/lukbieli/weather-data-scraper-armaag.git
    ```
2. **Open the project in Arduino IDE:**
    - Open `weather_scrape.ino` in the Arduino IDE.

3. **Install required libraries:**
    - Ensure the following libraries are installed in your Arduino IDE:
        - `WiFi.h`
        - `HTTPClient.h`

## Usage
1. **Configure Wi-Fi settings:**
    - In `weather_scrape.ino`, update the `ssid` and `password` constants with your Wi-Fi network credentials.
    ```cpp
    const char *ssid = "your_wifi_ssid";
    const char *password = "your_wifi_password";
    ```

2. **Upload the code:**
    - Connect your Arduino board to your computer via USB.
    - Select the appropriate board and port in the Arduino IDE.
    - Upload the `weather_scrape.ino` sketch to the Arduino board.

3. **Monitor the output:**
    - Open the Serial Monitor in the Arduino IDE (set the baud rate to 115200).
    - You should see the connection status, HTTP request results, and parsed weather data output.

## Code Overview
### weather_scrape.ino
This file contains the main Arduino sketch that:
- Connects to a specified Wi-Fi network.
- Fetches XML data from a specified URL.
- Parses the fetched data to extract weather information.
- Outputs the extracted weather information to the Serial Monitor.

### xml_parser.cpp
This file contains functions for:
- Initializing the XML parser.
- Parsing XML data.
- Extracting specific weather data values from the parsed XML.

### xml_parser.h
This file contains the declarations for the functions and types used in `xml_parser.cpp`.

### Function Descriptions
#### XmlParser_init()
Initializes the XML parser by resetting flags and state.

#### XmlParser_parse(String str)
Parses the provided XML string to extract weather data.

#### XmlParser_GetValue(ValueType_e vType)
Returns the last extracted value for the specified weather data type.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
# weather-data-scraper-armaag

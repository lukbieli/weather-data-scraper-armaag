#include <WiFi.h>
#include <HTTPClient.h>
#include "xml_parser.h"

// WiFi settings
const char *ssid = "your_wifi_ssid";
const char *password = "your_wifi_password";

// Website settings
const char *url = "http://armaag.gda.pl/data/xml/weather_wszystko2.xml";

/**
 * @brief Setup function for initializing serial communication and Wi-Fi connection.
 * 
 * This function initializes the serial communication at a baud rate of 115200,
 * connects to the specified Wi-Fi network, and performs an HTTP GET request
 * to fetch weather data in XML format. The XML data is then parsed and the
 * relevant weather values are printed to the serial monitor.
 * 
 * @param None
 * @return None
 */
void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");

    // Wait for Wi-Fi connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Connected to Wi-Fi");

    // Check if Wi-Fi is connected
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Specify the URL
        http.begin(url);

        // Send the request
        int httpResponseCode = http.GET();

        // Check the response code
        if (httpResponseCode > 0)
        {
            // Initialize XML parser
            XmlParser_init();

            // Get the response payload
            String payload = http.getString();

            // Parse the XML payload
            XmlParser_parse(payload);
        }
        else
        {
            // Print error message
            Serial.print("Error on HTTP request: ");
            Serial.println(httpResponseCode);
        }

        // Free resources
        http.end();

        // Print summary of values
        Serial.println("Values:");
        Serial.printf("\t Temperature: %s C\n", XmlParser_GetValue(XV_TEMP));
        Serial.printf("\t Humidity: %s %%\n", XmlParser_GetValue(XV_HUMID));
        Serial.printf("\t Pressure: %s hPa\n", XmlParser_GetValue(XV_PRESS));
        Serial.printf("\t Wind direction: %s \n", XmlParser_GetValue(XV_WIND_D));
        Serial.printf("\t Wind velocity: %s m/s\n", XmlParser_GetValue(XV_WIND_V));
        Serial.printf("\t Rain: %s mm\n", XmlParser_GetValue(XV_RAIN));
    }
    else
    {
        // Print error message if not connected to Wi-Fi
        Serial.println("Wi-Fi not connected");
    }
}

/**
 * @brief Main loop function.
 * 
 * This function runs repeatedly after the setup() function. Currently, it does nothing.
 * 
 * @param None
 * @return None
 */
void loop()
{
    // The loop function is intentionally left empty
}

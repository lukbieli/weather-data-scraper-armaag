#ifndef D78DF1FD_3BAF_4CC0_AB53_3A0A6F85A5C8
#define D78DF1FD_3BAF_4CC0_AB53_3A0A6F85A5C8

#include <Arduino.h>  

/**
 * @brief Enumeration of value types for weather data.
 * 
 * This enum defines the various types of weather data that can be parsed and extracted.
 */
typedef enum {
    XV_TEMP = 0,  ///< Temperature
    XV_HUMID = 1, ///< Humidity
    XV_PRESS = 2, ///< Pressure
    XV_WIND_V = 3,///< Wind velocity
    XV_WIND_D = 4,///< Wind direction
    XV_RAIN = 5   ///< Rainfall
} ValueType_e;

/**
 * @brief Initialize the XML parser.
 * 
 * This function resets the XML parsing flags and state, preparing the parser for new data.
 * 
 * @param None
 * @return None
 */
void XmlParser_init();

/**
 * @brief Parse the XML data from a string.
 * 
 * This function takes an XML string, splits it into lines, and analyzes each line
 * to extract relevant weather data.
 * 
 * @param str String containing the XML data.
 * @return None
 */
void XmlParser_parse(String str);

/**
 * @brief Get the last extracted value for a specific weather data type.
 * 
 * This function returns the last extracted value for the specified weather data type.
 * 
 * @param vType Enumeration of the weather data type.
 * @return char* Pointer to the last extracted value string.
 */
char* XmlParser_GetValue(ValueType_e vType);

#endif /* D78DF1FD_3BAF_4CC0_AB53_3A0A6F85A5C8 */

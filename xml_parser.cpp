#include "xml_parser.h"
#include <Arduino.h>  
#include <string.h>
#include <stdlib.h>

#define NUM_OF_W_DT (6u)  // Number of weather data types

// XML parsing state flags
static int xmlStartFlag = 0;
static char previousChar = ' ';

// Analyze state enumeration for parsing state
typedef enum{
    AS_IDLE,     // Idle state
    AS_STATION   // Parsing station state
} analyze_state_e;

// Initial parsing state
static analyze_state_e analyze_state = AS_IDLE;

// Patterns to identify XML sections
static const char* stationPattern = "<station name=\"AM5\"";  // Pattern to parse specific station
static const char* substancePattern = "<substance type=";
static const char* stationEndPattern = "</station>";

// Struct to hold weather data type information
typedef struct {
    const char typeStr[4];  // Type string identifier
    int typeStrLen;         // Length of type string
    void (*valueExtractPtr)(const char* str, char* outStr);  // Pointer to value extraction function
    char lastVal[6];        // Last extracted value
    ValueType_e valType;    // Value type enumeration
} WeatherDataType_t;

// Function declarations
static void extractValeFromStrList(const char* str, char* outStr);
static void analyze_line(const char* buf, int size);
static char* getSubstanceType(const char* str);

// Array to hold different weather data types and their properties
static WeatherDataType_t WeatherDataTypes[NUM_OF_W_DT] = {
    { .typeStr = {'T', 'E', 'M', 'P'}, .typeStrLen = 4, .valueExtractPtr = &extractValeFromStrList, .lastVal = {0}, .valType = XV_TEMP },
    { .typeStr = {'W', 'I', 'L', 'G'}, .typeStrLen = 4, .valueExtractPtr = &extractValeFromStrList, .lastVal = {0}, .valType = XV_HUMID },
    { .typeStr = {'C', 'I', 'S', 'N'}, .typeStrLen = 4, .valueExtractPtr = &extractValeFromStrList, .lastVal = {0}, .valType = XV_PRESS },
    { .typeStr = {'W', 'V', ' ', ' '}, .typeStrLen = 2, .valueExtractPtr = &extractValeFromStrList, .lastVal = {0}, .valType = XV_WIND_V },
    { .typeStr = {'W', 'D', ' ', ' '}, .typeStrLen = 2, .valueExtractPtr = &extractValeFromStrList, .lastVal = {0}, .valType = XV_WIND_D },
    { .typeStr = {'R', 'A', 'I', 'N'}, .typeStrLen = 4, .valueExtractPtr = &extractValeFromStrList, .lastVal = {0}, .valType = XV_RAIN }
};

/**
 * @brief Initialize the XML parser.
 * 
 * This function resets the XML parsing flags and state.
 * 
 * @param None
 * @return None
 */
void XmlParser_init()
{
    xmlStartFlag = 0;
    previousChar = ' ';
}

/**
 * @brief Parse the XML data from a string.
 * 
 * This function takes an XML string, splits it into lines, and analyzes each line
 * to extract relevant weather data.
 * 
 * @param str String containing the XML data.
 * @return None
 */
void XmlParser_parse(String str)
{
    // Process the payload line by line
    int start = 0;
    int end = str.indexOf('\n');
    while (end != -1)
    {
        analyze_line(str.substring(start, end).c_str(), end - start);
        start = end + 1;
        end = str.indexOf('\n', start);
    }
}

/**
 * @brief Get the last extracted value for a specific weather data type.
 * 
 * This function returns the last extracted value for the specified weather data type.
 * 
 * @param vType Enumeration of the weather data type.
 * @return char* Pointer to the last extracted value string.
 */
char* XmlParser_GetValue(ValueType_e vType)
{
    if (vType >= 0 && vType < NUM_OF_W_DT)
    {
        return WeatherDataTypes[vType].lastVal;
    }
    return NULL;
}

/**
 * @brief Get the substance type from the XML string.
 * 
 * This function compares the given string to known weather data types and extracts
 * the corresponding value if a match is found.
 * 
 * @param str Pointer to the XML string containing the substance type.
 * @return char* NULL (not used for return value).
 */
static char* getSubstanceType(const char* str)
{
    for (int i = 0; i < NUM_OF_W_DT; i++)
    {
        if (memcmp(str + 1, WeatherDataTypes[i].typeStr, WeatherDataTypes[i].typeStrLen) == 0)
        {
            Serial.println("Found substance");
            Serial.println(WeatherDataTypes[i].typeStr);
            (*WeatherDataTypes[i].valueExtractPtr)(str + 1, (char*)WeatherDataTypes[i].lastVal);
            break;
        }
    }

    return NULL;
}

/**
 * @brief Analyze a line of XML data.
 * 
 * This function analyzes a single line of the XML data, identifying whether it contains
 * station information or substance data, and processes it accordingly.
 * 
 * @param buf Pointer to the line buffer.
 * @param size Size of the line buffer.
 * @return None
 */
static void analyze_line(const char* buf, int size)
{
    char* c_ptr = NULL;
    switch (analyze_state)
    {
        case AS_IDLE:
            c_ptr = strstr(buf, stationPattern);
            if (c_ptr != NULL)
            {
                // "<station name="AM5"" found
                analyze_state = AS_STATION;
                Serial.println("Found station AM5");
                Serial.printf("Station line: %s\n", buf);
            }
            break;

        case AS_STATION:
            c_ptr = strstr(buf, substancePattern);
            if (c_ptr != NULL)
            {
                Serial.printf("Substance pattern line: %s\n", buf);
                char* typeStr = getSubstanceType(c_ptr + strlen(substancePattern));
            }
            c_ptr = strstr(buf, stationEndPattern);
            if (c_ptr != NULL)
            {
                Serial.println("Station ended");
                analyze_state = AS_IDLE;
            }
            break;
    }
}

/**
 * @brief Extract value from a string list.
 * 
 * This function extracts a value from a given string list, ignoring specific
 * characters and handling specific value conditions.
 * 
 * @param str Pointer to the input string list.
 * @param outStr Pointer to the output string to store the extracted value.
 * @return None
 */
static void extractValeFromStrList(const char* str, char* outStr)
{
    char valBuf[7] = {0};
    int valBufIdx = 0;
    char* startValStr = strchr(str, '>');
    char* startEndStr = strchr(startValStr, '<');
    if ((startValStr != NULL) && (startEndStr != NULL))
    {
        for (char* c_ptr = startValStr + 1; c_ptr < startEndStr; c_ptr++)
        {
            if (*c_ptr != '|')
            {
                valBuf[valBufIdx++] = *c_ptr;
            }
            else
            {
                if (memcmp(valBuf, "-999", 4) != 0)
                {
                    memset(outStr, 0, 6);
                    memcpy(outStr, valBuf, valBufIdx);
                }
                valBufIdx = 0;
            }
        }
    }
}

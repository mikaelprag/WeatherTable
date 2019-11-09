#ifndef WEATHERREPORT_H
#define WEATHERREPORT_H

#include <string>

#ifdef __APPLE__
	#include <json/json.h>
#else
	#include <jsoncpp/json/json.h>
#endif

enum WD {
    NONE = 1,
    UP = 2,
    DOWN = 3,
    CITY = 4
};

class WeatherReport
{
private:
    bool _hasData;
    int _humidity;
    float _temperature;
    
    std::string _timeStamp;
    std::string _apiUrl;
    std::string _city;
    std::string _apiKey;
    
    std::string _completeApiUrl();
    void _generateTimeStamp();
    static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp);
    
public:
    WeatherReport();
    WeatherReport(std::string city);
    ~WeatherReport();

    bool fetch();
    float getTemperature();
    int getHumidity();
    
    std::string getTimeStamp();
    std::string getCityNameFixedLength(int fixedLength);
    bool hasData();
    
};

#endif // WEATHERREPORT_H

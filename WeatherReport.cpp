#include <curl/curl.h>

#ifdef __APPLE__
	#include <json/json.h>
#else
	#include <jsoncpp/json/json.h>
#endif

#include <string>
#include <iostream>
#include <string.h>
#include "WeatherReport.h"

WeatherReport::WeatherReport()
{
    _hasData = false;
    _apiKey = "";
    _apiUrl = "api.openweathermap.org/data/2.5/weather";
}

WeatherReport::WeatherReport(std::string city) : WeatherReport() {
    _city = city;
}

WeatherReport::~WeatherReport()
{
}

bool WeatherReport::fetch() {
    bool retVal = false;
    
    curl_global_init(CURL_GLOBAL_ALL);
    CURLcode res;
    CURL* easyhandle = curl_easy_init();
    std::string readBuffer;
    Json::CharReaderBuilder builder;
    Json::CharReader * reader = builder.newCharReader();

    Json::Value root;
    std::string jsonErrors;
    
    curl_easy_setopt(easyhandle, CURLOPT_URL, _completeApiUrl().c_str());
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);
    
    res = curl_easy_perform(easyhandle);
    
    if(res == CURLE_OK) {
        bool parsingSuccessful = reader->parse(readBuffer.c_str(), readBuffer.c_str() + readBuffer.size(), &root, &jsonErrors);
    
        if(parsingSuccessful) {
            if(!Json::Value(root["main"]).isNull()) {
                
                // If we have a previous timestamp, this is not the first fetch.
                if(_timeStamp != "") {
                    _hasData = true;
                }
                
                _temperature = Json::Value(root["main"]["temp"]).asFloat();
                _humidity = Json::Value(root["main"]["humidity"]).asInt();
                _temperature = _temperature - 270;
                _generateTimeStamp();
                
                retVal = true;
            } else {
                std::cout << "Invalid JSON response: " << readBuffer << std::endl;
            }
        } else {
            std::cout << "Response error: " << readBuffer << std::endl;
        }
    } else {
        std::cout << "HTTP error: " << res << std::endl;
    }
    
    curl_easy_cleanup(easyhandle);
    
    return retVal;
}

void WeatherReport::_generateTimeStamp() {
    _timeStamp = "";
    
    time_t rawtime;
    time(&rawtime);
    
    char *t = ctime(&rawtime);
    if (t[strlen(t)-1] == '\n') {
        t[strlen(t)-1] = '\0';
    }
    _timeStamp = t;
}

std::string WeatherReport::_completeApiUrl() {
    std::string url = "";
    return url
        .append(_apiUrl)
        .append("?q=")
        .append(_city)
        .append("&APPID=")
        .append(_apiKey);
}

size_t WeatherReport::WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

float WeatherReport::getTemperature() {
    return _temperature;
}

int WeatherReport::getHumidity() {
    return _humidity;
}

std::string WeatherReport::getTimeStamp() {
    return _timeStamp;
}

std::string WeatherReport::getCityNameFixedLength(int fixedLength) {
    std::string cityFixedSize = "";
    
    for(int i = 0; i <= fixedLength; i++) {
        if((int)_city.size() > i) {
            std::string singleChar(1, _city.at(i));
            cityFixedSize.append(singleChar);
        } else {
            cityFixedSize.append(" ");
        }
    }
    return cityFixedSize;
}

bool WeatherReport::hasData() {
    return _hasData;
}

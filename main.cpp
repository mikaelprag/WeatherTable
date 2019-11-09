#include <stdio.h>
#include <chrono>
#include <thread>
#include <string>
#include <ncurses.h>
#include "WeatherReport.h"
using namespace std;

void printCity(string city);
void printTemperature(bool hasData, float newTemp, float oldTemp);
void printHumidity(bool hasData, int newHumid, int oldHumid);
void printTimestamp(string timeStamp);
void printLine(int lineType);
void printStraightLine(int times);

enum {
    LINE_TYPE_UPPER,
    LINE_TYPE_LOWER,
    LINE_TYPE_PLAIN
};

int main(int argc, char **argv)
{
    initscr();
    
    if(has_colors() == FALSE) {
        printf("No color support!\n");
        return 1;
    }
    
    start_color();
    curs_set(FALSE);
    
    init_pair(WD::NONE, COLOR_WHITE, COLOR_BLACK);
    init_pair(WD::UP, COLOR_RED, COLOR_BLACK);
    init_pair(WD::DOWN, COLOR_CYAN, COLOR_BLACK);
    init_pair(WD::CITY, COLOR_YELLOW, COLOR_BLACK);
    
    int waitTimeMinutes = 2;
	int waitTimeMilliSeconds = (waitTimeMinutes * 60 * 1000);
    
    WeatherReport cities[] = {
        WeatherReport("Spijkenisse"),
        WeatherReport("Degerfors"),
        WeatherReport("Willemstad"),
        WeatherReport("London"),
        WeatherReport("Los Angeles"),
        WeatherReport("New York"),
        WeatherReport("Albuquerque"),
        WeatherReport("Barcelona"),
        WeatherReport("Nairobi"),
        WeatherReport("Cape Town"),
        WeatherReport("Moscow"),
        WeatherReport("Santiago"),
        WeatherReport("Lima"),
        WeatherReport("Torshavn"),
        WeatherReport("Longyearbyen"),
        WeatherReport("Resolute"),
        WeatherReport("Pevek")
    };
    
    int nrOfCities = sizeof(cities)/sizeof(cities[0]);
    
    while(true) {
        int currentRow = 0;
        
        printLine(LINE_TYPE_UPPER);
        
        for(int i = 0; i < nrOfCities; i++) {
            
            float oldTemp = cities[i].getTemperature();
            int oldHumidity = cities[i].getHumidity();
            
            if(cities[i].fetch()) {
                
                currentRow++;
                move(currentRow, 0);
                clrtoeol();
                
                bool hasData = cities[i].hasData();
                
                addch(ACS_VLINE);
                printCity(cities[i].getCityNameFixedLength(24));
                printTemperature(hasData, cities[i].getTemperature(), oldTemp);
                addstr(" - ");
                printHumidity(hasData, cities[i].getHumidity(), oldHumidity);
                addstr("\t");                
                printTimestamp(cities[i].getTimeStamp());
                addch(ACS_VLINE);
                
            }
            
        }
        
        currentRow++;
        move(currentRow, 0);
        clrtoeol();
        printLine(LINE_TYPE_LOWER);
        
        currentRow++;
        move(currentRow, 0);
        clrtoeol();
        
        refresh();
        
        this_thread::sleep_for(chrono::milliseconds(waitTimeMilliSeconds));
    }
    
    endwin();
	return 0;
}


void printCity(string city) {
    attron(COLOR_PAIR(4));
    addstr(city.c_str());
    attroff(COLOR_PAIR(4));
}

void printTemperature(bool hasData, float newTemp, float oldTemp) {
    
    char tempBuffer[16];
    snprintf(tempBuffer, 8, "%.2f", newTemp);
    
    int palette = WD::NONE;
    
    if(hasData) {
        if(newTemp > oldTemp) {
            palette = WD::UP;
        } else if (newTemp < oldTemp) {
            palette = WD::DOWN;
        }
    }
    
    attron(COLOR_PAIR(palette));
    addstr(tempBuffer);
    addstr("c");
    attroff(COLOR_PAIR(palette));
}

void printHumidity(bool hasData, int newHumid, int oldHumid) {
    int palette = WD::NONE;
    
    if(hasData) {
        if(newHumid > oldHumid) {
            palette = WD::UP;
        } else if (newHumid < oldHumid) {
            palette = WD::DOWN;
        }
    }
        
    attron(COLOR_PAIR(palette));
    addstr(to_string(newHumid).c_str());
    addstr("%");
    attroff(COLOR_PAIR(palette));
}

void printTimestamp(string timeStamp) {
    addstr(timeStamp.c_str());
}

void printLine(int lineType) {
    
    switch(lineType) {
        case LINE_TYPE_UPPER:
            addch(ACS_ULCORNER);
            printStraightLine(63);
            addch(ACS_URCORNER);
            break;
        case LINE_TYPE_LOWER:
            addch(ACS_LLCORNER);
            printStraightLine(63);
            addch(ACS_LRCORNER);
            break;
        case LINE_TYPE_PLAIN:
            printStraightLine(65);
            break;
    }
}

void printStraightLine(int times) {
    for(int i = 0; i < times; i++) {
        addch(ACS_HLINE);
    }
}
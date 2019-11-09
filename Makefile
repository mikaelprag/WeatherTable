all:
	g++ WeatherReport.cpp main.cpp -o WeatherTable -lcurl -ljsoncpp -lncurses -std=c++11 

clean:
	rm WeatherTable
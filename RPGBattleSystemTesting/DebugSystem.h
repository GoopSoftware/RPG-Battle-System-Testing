#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>



enum class LogLevel {
	INFO,
	INPUT,
	WARNING,
	ERROR,
	TOOLTIP,
	DEBUG,
	EVENT
};


class DebugSystem
{

public:
	DebugSystem();
	~DebugSystem();

	//static LogLevel minLevel = LogLevel::DEBUG;

	static void log(std::string system, LogLevel level, std::string message);

private:


	static std::string getTimestamp();
	static std::string levelToString(LogLevel level);

};


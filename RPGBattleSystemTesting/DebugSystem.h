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

	LogLevel minLevel = LogLevel::DEBUG;

	void log(std::string system, LogLevel level, std::string message);

private:


	std::string getTimestamp();
	std::string levelToString(LogLevel level);

};


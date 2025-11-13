#include "DebugSystem.h"


DebugSystem::DebugSystem() {

}


DebugSystem::~DebugSystem() {

}


void DebugSystem::log(std::string system, LogLevel level, std::string message) {

	std::ostringstream stream;
	std::string timeStr = getTimestamp();
	std::string levelStr = levelToString(level);
	stream << "[" << timeStr << "] "
		<< "[" << system << "] "
		<< "[" << levelStr << "] "
		<< message;

	std::cout << stream.str() << std::endl;
}


std::string DebugSystem::getTimestamp() {

	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm localTime;

	#ifdef _WIN32
	localtime_s(&localTime, &now);
	#else
	localtime_r(&now, &localTime);
	#endif
	std::ostringstream ss;
	ss << std::put_time(&localTime, "%H:%M:%S");

	return ss.str();
}


std::string DebugSystem::levelToString(LogLevel level) {
	switch (level) {
	case LogLevel::INFO:
		return "INFO";
	case LogLevel::INPUT:
		return "INPUT";
	case LogLevel::WARNING:
		return "WARNING";
	case LogLevel::ERROR:
		return "ERROR";
	case LogLevel::TOOLTIP:
		return "TOOLTIP";
	case LogLevel::DEBUG:
		return "DEBUG";
	case LogLevel::EVENT:
		return "EVENT";
	default:
		return "UNKNOWN";
	}
}
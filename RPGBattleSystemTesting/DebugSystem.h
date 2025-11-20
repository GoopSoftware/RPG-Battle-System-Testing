#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <unordered_map>

class GameStateManager;
class BattleSystem;
struct SpriteComponent;
using Entity = std::uint32_t;


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
	void handleAnimationDebug(GameStateManager& game, const BattleSystem* battle);
	void log(std::string system, LogLevel level, std::string message);

private:

	bool debugOn = false;
	std::string getTimestamp();
	std::string levelToString(LogLevel level);

};


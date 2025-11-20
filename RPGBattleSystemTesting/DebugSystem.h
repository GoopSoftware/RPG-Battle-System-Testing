#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <unordered_map>
#include "raylib.h"


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


	static void enableDebug();
	static void handleAnimationDebug(GameStateManager& game, const BattleSystem* battle);
	static void log(std::string system, LogLevel level, std::string message);
	
	static LogLevel minLevel;
	static bool debug;

	static std::string getTimestamp();
	static std::string levelToString(LogLevel level);

private:

	DebugSystem() = delete;
	~DebugSystem() = delete;


};


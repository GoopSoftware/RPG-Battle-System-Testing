#include "BattleSystem.h"
#include "DebugSystem.h"
#include "GameStateManager.h"
#include "SpriteComponent.h"
#include "AnimationSystem.h"
#include "raylib.h"

DebugSystem::DebugSystem() {

}


DebugSystem::~DebugSystem() {

}


void DebugSystem::log(std::string system, LogLevel level, std::string message) {
	if (debugOn) {
		std::ostringstream stream;
		std::string timeStr = getTimestamp();
		std::string levelStr = levelToString(level);
		stream << "[" << timeStr << "] "
			<< "[" << system << "] "
			<< "[" << levelStr << "] "
			<< message;

		std::cout << stream.str() << std::endl;
	}
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

void DebugSystem::handleAnimationDebug(GameStateManager& game, const BattleSystem* battle)
{
	if (debugOn) {
		if (!battle) return;

		const auto& livingEnemies = battle->getLivingEnemies();

		auto setAll = [&](AnimationState state, const char* name) {
			for (Entity e : livingEnemies) {
				SpriteComponent& sprite = game.getSprite(e);
				game.animationSystem.setState(sprite, state);
			}
			log("Debug", LogLevel::INFO, std::string("Set all enemies to animation: ") + name);
			};

		if (IsKeyPressed(KEY_ONE)) {
			setAll(AnimationState::Idle, "Idle");
		}
		if (IsKeyPressed(KEY_TWO)) {
			setAll(AnimationState::Walk, "Walk");
		}
		if (IsKeyPressed(KEY_THREE)) {
			setAll(AnimationState::Attack, "Attack");
		}
		if (IsKeyPressed(KEY_FOUR)) {
			setAll(AnimationState::Hurt, "Hurt");
		}
		if (IsKeyPressed(KEY_FIVE)) {
			setAll(AnimationState::Dead, "Dead");
		}
		if (IsKeyPressed(KEY_SIX)) {
			AnimationState randomState = static_cast<AnimationState>(GetRandomValue(0, 4));
			setAll(randomState, "Random");
		}
	}
	
}

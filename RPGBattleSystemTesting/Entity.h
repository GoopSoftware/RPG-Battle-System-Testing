#pragma once
#include <cstdint>

using Entity = std::uint32_t;



inline Entity createEntity() {

	static Entity nextEntityId = 1;
	return nextEntityId++;
}

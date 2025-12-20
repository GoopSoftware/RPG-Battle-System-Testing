#pragma once
#include <string>
#include <vector>

struct LoadError {
	std::string file;
	std::string message;
};


template <typename T>

struct LoadResult {
	T value{};
	std::vector<LoadError> errors;

	bool ok() const { return errors.empty(); }
};
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <chrono>

struct race_t {
	uint64_t time;
	uint64_t distance;
};

//std::array<race_t, 3> races_p1 = { { {7, 9}, {15, 40}, {30, 200} } };
std::array<race_t, 4> races_p1 = { { {44, 208}, {80, 1581}, {65, 1050}, {72, 1102} } };

//std::array<race_t, 3> races_p2 = { { {71530, 940200} } };
std::array<race_t, 1> races_p2 = { { { 44806572, 208158110501102} } };

template<size_t N>
uint64_t p1(const std::array<race_t, N>& input) {
	uint64_t result = 1;
	for (auto& race : input) {
		uint64_t wins = 0;
		for (unsigned i = 1; i < race.time; ++i) {
			uint64_t dist = i * (race.time - i);
			if (dist > race.distance) {
				++wins;
			}
		}
		result *= wins;
	}

	return result;
}

template<class array_t>
uint64_t p2(const array_t& input) {
	uint64_t result = 0;
	for (auto& race : input) {
		for (uint64_t i = 1; i < race.time; ++i) {
			uint64_t dist = i * (race.time - i);
			if (dist > race.distance) {
				++result;
			}
		}
	}

	return result;
}

int main() {
	auto t0 = std::chrono::high_resolution_clock::now();

	auto t1 = std::chrono::high_resolution_clock::now();
	auto r1 = p1(races_p1);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto r2 = p2(races_p2);

	auto t3 = std::chrono::high_resolution_clock::now();

	std::cout << "P1 result: " << r1 << "\n";
	std::cout << "P2 result: " << r2 << "\n";
	std::cout << "Input duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
	std::cout << "P1 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	std::cout << "P2 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << "ms" << std::endl;
}
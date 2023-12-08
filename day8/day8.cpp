#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <string>
#include <array>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <ranges>
#include <functional>
#include <numeric>

using commands_t = std::vector<uint8_t>;
using network_t = std::map<std::string, std::array<std::string, 2>>;

std::tuple<commands_t, network_t> parse_input(std::istream& is) {
	commands_t commands;
	network_t network;

	std::string line;
	std::getline(is, line);
	for (char c : line) {
		if (c == 'L') {
			commands.push_back(0);
		}
		else {
			commands.push_back(1);
		}
	}

	while (std::getline(is, line)) {
		if (line.empty())
			continue;

		std::string key = line.substr(0, 3);
		std::string left = line.substr(7, 3);
		std::string right = line.substr(12, 3);

		network[key] = { left, right };
	}

	return std::make_tuple(commands, network);
}

uint64_t lcm(std::vector<uint64_t>& nums) {
	auto result = nums[0];
	for (size_t i = 1; i < nums.size(); ++i) {
		result = std::lcm(result, nums[i]);
	}
	return result;
}

uint64_t travel(const commands_t& cmds, const network_t& network, std::string pos, std::function<bool(const std::string&)> is_done) {
	uint64_t result = 0;

	while (true) {
		for (uint8_t cmd : cmds) {
			++result;
			pos = network.at(pos)[cmd];
			if (is_done(pos))
				return result;
		}
	}
}

uint64_t p1(const commands_t& cmds, const network_t& network) {
	return travel(cmds, network, "AAA", [](auto p) { return p == "ZZZ"; });
}

uint64_t p2(const commands_t& cmds, const network_t& network) {

	auto p = network | std::views::filter([](const auto& p) { return p.first[2] == 'A'; }) | std::views::transform([](const auto& p) { return p.first; });
	std::vector<std::string> positions(p.begin(), p.end());
	std::vector<uint64_t> results;

	for (std::string pos : positions) {
		results.push_back(travel(cmds, network, pos, [](auto p) { return p[2] == 'Z'; }));
	}

	return lcm(results);
}

int main() {
	std::ifstream ifs("data/input");
	if (!ifs) {
		std::cout << "input not found\n";
		return 1;
	}

	auto t0 = std::chrono::high_resolution_clock::now();
	auto [commands, network] = parse_input(ifs);

	auto t1 = std::chrono::high_resolution_clock::now();
	auto r1 = p1(commands, network);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto r2 = p2(commands, network);
	auto t3 = std::chrono::high_resolution_clock::now();

	std::cout << "P1 result: " << r1 << "\n";
	std::cout << "P2 result: " << r2 << "\n";
	std::cout << "Input duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
	std::cout << "P1 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	std::cout << "P2 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << "ms" << std::endl;
}
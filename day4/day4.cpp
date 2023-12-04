#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>
#include <tuple>
#include <vector>
#include <numeric>
#include <chrono>

const int offset = 10;

using cards_t = std::vector<std::tuple<std::set<unsigned>, std::set<unsigned>>>;

cards_t parse_input(std::istream& is) {
	std::vector<std::tuple<std::set<unsigned>, std::set<unsigned>>> cards;

	is.ignore(offset, '\n');
	for (std::string str; std::getline(is, str) && !str.empty(); is.ignore(offset, '\n')) {
		auto& [wins, mines] = cards.emplace_back();

		std::stringstream ss(str);

		bool mine = false;
		std::string token;
		while (ss >> token) {
			if (token == "|") {
				mine = true;
				continue;
			}

			if (!mine) {
				wins.insert(std::stoi(token));
			}
			else {
				mines.insert(std::stoi(token));
			}
		}
	}

	return cards;
}

unsigned p1(const cards_t& cards) {
	unsigned result = 0;


	for (auto& [wins, mines] : cards)
	{
		std::set<unsigned> intersection;
		std::set_intersection(wins.begin(), wins.end(), mines.begin(), mines.end(), std::inserter(intersection, intersection.begin()));

		if (intersection.size() > 0) {
			result += static_cast<unsigned>(pow(2, intersection.size() - 1));
		}
	}

	return result;
}

void calculate_wins(const cards_t& cards, unsigned current, unsigned &result) {
	auto & [wins, mines] = cards[current - 1];
	std::set<unsigned> intersection;
	std::set_intersection(wins.begin(), wins.end(), mines.begin(), mines.end(), std::inserter(intersection, intersection.begin()));

	auto wins_count = std::min(current + intersection.size(), cards.size()) - current;

	for (auto i = current + 1; i <= current + wins_count; ++i) {
		result += 1;
		calculate_wins(cards, i, result);
	}	
}

unsigned p2(const cards_t& cards) {
	unsigned result = 0;

	for (auto i = 1; i <= cards.size(); ++i) {
		result += 1;
		calculate_wins(cards, i, result);
	}

	return result;
}

int main() {
	std::ifstream ifs("data/input");
	if (!ifs) {
		std::cout << "input not found\n";
		return 1;
	}

	auto t0 = std::chrono::high_resolution_clock::now();
	auto input = parse_input(ifs);

	auto t1 = std::chrono::high_resolution_clock::now();
	auto r1 = p1(input);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto r2 = p2(input);
	
	auto t3 = std::chrono::high_resolution_clock::now();

	std::cout << "P1 result: " << r1 << "\n";
	std::cout << "P2 result: " << r2 << "\n";

	std::cout << "Input duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
	std::cout << "P1 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	std::cout << "P2 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << "ms" << std::endl;
}
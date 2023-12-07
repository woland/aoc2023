#include <fstream>
#include <iostream>
#include <chrono>
#include <array>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>

enum class type_t { high_card, pair, two_pair, three, full_house, four_row, five_row };

struct hand_t {
	std::string str;
	std::array<uint8_t, 5> cards;
	type_t type;
	unsigned bet;

	hand_t(std::string s, std::array<uint8_t, 5> cs, unsigned b)
		: str(s), cards(std::move(cs)), type(get_type(cards)), bet(b) {
	}

	bool operator<(const hand_t& h) const {
		if (type != h.type) {
			return type < h.type;
		}
		else {
			for (int i = 0; i < 5; ++i) {
				if (cards[i] != h.cards[i])
					return cards[i] < h.cards[i];
			}
		}
		return false;
	}

	static type_t get_type(const std::array<uint8_t, 5>& cs) {
		std::array<uint8_t, 13> counts = { 0 };
		for (auto c : cs) {
			++counts[c];
		}
		auto jcount = counts[0];
		counts[0] = 0;
		std::sort(std::begin(counts), std::end(counts));

		if (counts[12] + jcount == 5)
			return type_t::five_row;
		else if (counts[12] + jcount == 4)
			return type_t::four_row;
		else if (counts[12] + jcount == 3) {
			if (counts[11] == 2)
				return type_t::full_house;
			else
				return type_t::three;
		}
		else if (counts[12] + jcount == 2) {
			if (counts[11] == 2)
				return type_t::two_pair;
			else
				return type_t::pair;
		}
		else
			return type_t::high_card;
	}
};

std::vector<hand_t> parse_input(std::istream& is) {
	std::vector<hand_t> hands;

	for (std::string line; std::getline(is, line);) {
		std::array<uint8_t, 5> cards;
		std::istringstream iss(line);
		for (int i = 0; i < 5; ++i) {
			char c;
			iss >> c;
			switch (c) {
			case 'T':
				cards[i] = 9;
				break;
			case 'J':
				cards[i] = 0;
				break;
			case 'Q':
				cards[i] = 10;
				break;
			case 'K':
				cards[i] = 11;
				break;
			case 'A':
				cards[i] = 12;
				break;
			default:
				cards[i] = c - '2' + 1;
				break;
			}
		}
		unsigned bet;
		iss >> bet;
		hands.emplace_back(line.substr(0, 5), cards, bet);
	}
	return hands;
}

unsigned p1(std::vector<hand_t> hands) {
	unsigned result = 0;
	std::sort(hands.begin(), hands.end());

	for (auto i = 0; i < hands.size(); ++i) {
		std::cout << hands[i].str << " type=" << static_cast<int>(hands[i].type) << " bet=" << hands[i].bet << "\n";

		result += hands[i].bet * (i + 1);
	}
	return result;
}

unsigned p2(const std::vector<hand_t>& hands) {
	return 0;
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
// day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using num_world_t = std::vector<std::vector<unsigned>>;
using char_world_t = std::vector<std::vector<char>>;

struct world_t {
	num_world_t nums;
	char_world_t chars;

	size_t size_x() const {
		return nums[0].size();
	}
	size_t size_y() const {
		return nums.size();
	}
};

void fill_nums(std::vector<unsigned>* row, unsigned value, size_t start, size_t end) {
	for (auto i = start; i < end; ++i)
		(*row)[i] = value;
}

world_t parse_world(std::ifstream& ifs) {
	world_t world;

	for (std::string str; std::getline(ifs, str) && !str.empty();)
	{
		world.chars.push_back({ std::begin(str), std::end(str) });
		world.nums.push_back(std::vector<unsigned>(world.chars.back().size()));

		auto& chars_row = world.chars.back();
		auto& nums_row = world.nums.back();

		bool has_num = false;
		std::string buffer;

		size_t num_pos = 0;
		for (auto char_pos = 0; char_pos <= chars_row.size(); ++char_pos) {
			if (char_pos == chars_row.size()) {
				if (has_num) {
					fill_nums(&nums_row, std::stoi(buffer), num_pos, char_pos);
				}
			}
			else if (!std::isdigit(chars_row[char_pos]) && !has_num) {
				nums_row[num_pos] = 0;
				num_pos = char_pos;
			}
			else if (!std::isdigit(chars_row[char_pos]) && has_num) {
				fill_nums(&nums_row, std::stoi(buffer), num_pos, char_pos);
				num_pos = char_pos;
				has_num = false;
				buffer.clear();
			}
			else if (std::isdigit(chars_row[char_pos])) {
				if (!has_num) {
					num_pos = char_pos;
				}
				has_num = true;
				buffer.push_back(chars_row[char_pos]);
			}
		}
	}

	return world;
}

unsigned look_n(const world_t& w, size_t x, size_t y) {
	if (y == 0)
		return 0;
	return w.nums[y - 1][x];
}

unsigned look_ne(const world_t& w, size_t x, size_t y) {
	if (y == 0 || x + 1 == w.size_x())
		return 0;
	return w.nums[y - 1][x + 1];
}

unsigned look_e(const world_t& w, size_t x, size_t y) {
	if (x + 1 == w.size_x())
		return 0;
	return w.nums[y][x + 1];
}

unsigned look_se(const world_t& w, size_t x, size_t y) {
	if (x + 1 == w.size_x() || y + 1 == w.size_y())
		return 0;
	return w.nums[y + 1][x + 1];
}

unsigned look_s(const world_t& w, size_t x, size_t y) {
	if (y + 1 == w.size_y())
		return 0;
	return w.nums[y + 1][x];
}

unsigned look_sw(const world_t& w, size_t x, size_t y) {
	if (x == 0 || y + 1 == w.size_y())
		return 0;
	return w.nums[y + 1][x - 1];
}

unsigned look_w(const world_t& w, size_t x, size_t y) {
	if (x == 0)
		return 0;
	return w.nums[y][x - 1];
}

unsigned look_nw(const world_t& w, size_t x, size_t y) {
	if (x == 0 || y == 0)
		return 0;
	return w.nums[y - 1][x - 1];
}

unsigned look_p1(const world_t& w, size_t x, size_t y) {
	unsigned result;

	if (look_n(w, x, y) == 0) {
		std::cout << look_ne(w, x, y) << "+" << look_nw(w, x, y) << "+";
		result = look_ne(w, x, y) + look_nw(w, x, y);
	}
	else {
		std::cout << look_n(w, x, y) << "+";
		result = look_n(w, x, y);
	}

	std::cout << look_e(w, x, y) << "+" << look_w(w, x, y) << "+";
	result += look_e(w, x, y) + look_w(w, x, y);

	if (look_s(w, x, y) == 0) {
		std::cout << look_se(w, x, y) << "+" << look_sw(w, x, y) << "\n";
		result += look_se(w, x, y) + look_sw(w, x, y);
	}
	else {
		std::cout << look_s(w, x, y) << "\n";
		result += look_s(w, x, y);
	}

	return result;
}

unsigned look_p2(const world_t& w, size_t x, size_t y) {

	std::vector<unsigned> nums;
	if (look_n(w, x, y) == 0) {
		nums.push_back(look_ne(w, x, y));
		nums.push_back(look_nw(w, x, y));
	}
	else {
		nums.push_back(look_n(w, x, y));
	}

	nums.push_back(look_e(w, x, y));
	nums.push_back(look_w(w, x, y));

	if (look_s(w, x, y) == 0) {
		nums.push_back(look_se(w, x, y));
		nums.push_back(look_sw(w, x, y));
	}
	else {
		nums.push_back(look_s(w, x, y));
	}

	auto result = 0;
	if (std::count_if(std::begin(nums), std::end(nums), [](auto& n) { return n != 0;  }) == 2) {
		result = 1;
		for (auto& n : nums) {
			if (n != 0) {
				std::cout << n << "*";
				result *= n;
			}
		}
	}
	std::cout << "=" << result <<"\n";
	return result;
}

void p1(const world_t& world) {
	unsigned result = 0;
	for (auto y = 0; y < world.size_y(); ++y) {
		unsigned row_sum = 0;
		for (auto x = 0; x < world.size_x(); ++x) {
			if (world.nums[y][x] == 0 && world.chars[y][x] != '.') {
				std::cout << world.chars[y][x] << "[" << x << "]" << "[" << y << "]" << "=";
				row_sum += look_p1(world, x, y);
			}
		}
		result += row_sum;
	}

	std::cout << "P1 Result: " << result << "\n";
}

void p2(const world_t& world) {
	unsigned result = 0;
	for (auto y = 0; y < world.size_y(); ++y) {
		for (auto x = 0; x < world.size_x(); ++x) {
			if (world.nums[y][x] == 0 && world.chars[y][x] == '*') {
				std::cout << world.chars[y][x] << "[" << x << "]" << "[" << y << "]" << "=";
				result += look_p2(world, x, y);
			}
		}
	}
	std::cout << "P2 Result: " << result << "\n";
}

int main()
{
	std::ifstream ifs("data/input");
	if (!ifs) {
		std::cout << "input not found\n";
		return 1;
	}

	auto world = parse_world(ifs);

	p1(world);
	std::cout << "\n";
	p2(world);

	return 0;
}

#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
#include <map>

enum class color_t { red, blue, green };

const std::map<std::string, color_t> colors_map = {
	{"red", color_t::red},
	{"green", color_t::green},
	{"blue", color_t::blue}
};

const std::map<color_t, int> limits_map = {
	{color_t::red, 12},
	{color_t::green, 13},
	{color_t::blue, 14}
};

struct cube_t {
	color_t color;
	int number;
};

struct game_t {
	int id;
	std::vector<std::vector<cube_t>> subsets;
};

std::vector<game_t> parse_games(const std::string& data) {
	std::vector<game_t> games;
	std::regex gameRegex(R"(Game (\d+): (.+))");
	std::regex subsetRegex(R"(([^;]+))");
	std::regex cubeRegex(R"((\d+) (\w+))");

	std::sregex_iterator gameIter(data.begin(), data.end(), gameRegex);
	std::sregex_iterator gameEnd;

	while (gameIter != gameEnd) {
		std::smatch gameMatch = *gameIter;
		game_t game;
		game.id = std::stoi(gameMatch[1]);

		std::string subsetsStr = gameMatch[2];
		std::sregex_iterator subsetIter(subsetsStr.begin(), subsetsStr.end(), subsetRegex);
		std::sregex_iterator subsetEnd;

		while (subsetIter != subsetEnd) {
			std::smatch subsetMatch = *subsetIter;
			std::vector<cube_t> cubes;

			std::string cubesStr = subsetMatch[1];
			std::sregex_iterator cubeIter(cubesStr.begin(), cubesStr.end(), cubeRegex);
			std::sregex_iterator cubeEnd;

			while (cubeIter != cubeEnd) {
				std::smatch cubeMatch = *cubeIter;
				cube_t cube;
				cube.number = std::stoi(cubeMatch[1]);
				cube.color = colors_map.at(cubeMatch[2]);
				cubes.push_back(cube);
				++cubeIter;
			}

			game.subsets.push_back(cubes);
			++subsetIter;
		}

		games.push_back(game);
		++gameIter;
	}

	return games;
}

void p1(std::istream& ifs) {
	int result = 0;

	auto games = parse_games(std::string(std::istreambuf_iterator<char>(ifs), {}));


	for (auto& g : games) {
		bool bad = false;
		for (auto& s : g.subsets) {
			for (auto& c : s) {
				if (c.number > limits_map.at(c.color)) {
					bad = true;
					break;
				}
			}
		}
		if (!bad)
			result += g.id;
	}

	std::cout << "P1 Result: " << result << "\n";
}

void p2(std::istream& ifs) {
	int result = 0;

	auto games = parse_games(std::string(std::istreambuf_iterator<char>(ifs), {}));

	for (auto& gm : games) {
		int r = 1, g = 1, b = 1;
		for (auto& ss : gm.subsets) {
			for (auto& cu : ss) {
				switch (cu.color) {
				case color_t::red:
					r = std::max(r, cu.number);
					break;
				case color_t::green:
					g = std::max(g, cu.number);
					break;
				case color_t::blue:
					b = std::max(b, cu.number);
					break;
				}
			}
		}

		auto score = r * g * b;
		result += score;
	}

	std::cout << "P2 Result: " << result << "\n";
}

int main() {
	std::ifstream ifs("data/input");
	if (!ifs) {
		std::cout << "input not found\n";
		return 1;
	}

	p1(ifs);

	ifs.clear();
	ifs.seekg(0);

	p2(ifs);
}
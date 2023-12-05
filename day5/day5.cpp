#include <fstream>
#include <iostream>
#include <chrono>
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>
#include <set>

using seed_t = std::vector<unsigned>;

struct rule_t {
	unsigned destination;
	unsigned source;
	unsigned length;

	unsigned execute(unsigned input) const {
		if (input > source + length - 1)
			return input;
		return input + destination - source;
	}

	bool operator<(const rule_t& other) const {
		return source > other.source;
	}
};

struct ruleset_t {
	std::set<rule_t> rules;

	void add(rule_t&& rule) {
		rules.insert(std::move(rule));
	}

	unsigned execute(unsigned input) const {
		auto rule = std::find_if(std::begin(rules), std::end(rules), [input](const rule_t& rule) {
			return rule.source <= input;
		});

		if (rule == std::end(rules))
			return input;

		return rule->execute(input);
	}
};

struct process_t {
	std::vector<ruleset_t> rulesets;

	unsigned execute(unsigned input) const {
		//std::cout << input << ": ";
		for (auto& ruleset : rulesets) {
			input = ruleset.execute(input);
			//std::cout << " -> " << input;
		}
		//std::cout << "\n";

		return input;
	}
};

std::tuple<seed_t, process_t> parse_input(std::istream& is) {
	seed_t seeds;
	process_t process;
	process.rulesets.resize(7);

	std::string line;
	while (std::getline(is, line)) {
		if (line.find("seeds:") != std::string::npos) {
			std::istringstream seedStream(line.substr(line.find(":") + 1));
			unsigned seed;
			while (seedStream >> seed) {
				seeds.push_back(seed);
			}
		}
		else if (line.find("map:") != std::string::npos) {
			std::string rule;
			std::vector<unsigned> mapRow;
			while (std::getline(is, rule) && !rule.empty()) {
				std::istringstream mapStream(rule);
				unsigned value;
				while (mapStream >> value) {
					mapRow.push_back(value);
				}
				if (!mapRow.empty()) {
					if (line.find("seed-to-soil") != std::string::npos) {
						process.rulesets[0].add({ mapRow[0], mapRow[1], mapRow[2] });
					}
					else if (line.find("soil-to-fertilizer") != std::string::npos) {
						process.rulesets[1].add({ mapRow[0], mapRow[1], mapRow[2] });
					}
					else if (line.find("fertilizer-to-water") != std::string::npos) {
						process.rulesets[2].add({ mapRow[0], mapRow[1], mapRow[2] });
					}
					else if (line.find("water-to-light") != std::string::npos) {
						process.rulesets[3].add({ mapRow[0], mapRow[1], mapRow[2] });
					}
					else if (line.find("light-to-temperature") != std::string::npos) {
						process.rulesets[4].add({ mapRow[0], mapRow[1], mapRow[2] });
					}
					else if (line.find("temperature-to-humidity") != std::string::npos) {
						process.rulesets[5].add({ mapRow[0], mapRow[1], mapRow[2] });
					}
					else if (line.find("humidity-to-location") != std::string::npos) {
						process.rulesets[6].add({ mapRow[0], mapRow[1], mapRow[2] });
					}
					mapRow.clear();
				}
			}
		}
	}

	return { seeds, process };
}

unsigned p1(const seed_t& seeds, const process_t& process) {
	unsigned result = -1;

	for (auto& s : seeds) {
		result = std::min(result, process.execute(s));
	}

	return result;
}

unsigned p2(const seed_t& seeds, const process_t& process) {
	unsigned result = -1;

	std::vector<std::tuple<unsigned, unsigned>> ranges;
	for (auto i = 0; i < seeds.size(); i += 2) {
		ranges.push_back({ seeds[i], seeds[i + 1] });
	}

	for (auto& [k,l] : ranges) {
		for (auto i = k; i < k + l; ++i) {
			result = std::min(result, process.execute(i));
		}
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
	auto [seeds, process] = parse_input(ifs);

	auto t1 = std::chrono::high_resolution_clock::now();
	auto r1 = p1(seeds, process);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto r2 = p2(seeds, process);

	auto t3 = std::chrono::high_resolution_clock::now();

	std::cout << "P1 result: " << r1 << "\n";
	std::cout << "P2 result: " << r2 << "\n";
	std::cout << "Input duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
	std::cout << "P1 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	std::cout << "P2 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << "ms" << std::endl;
}
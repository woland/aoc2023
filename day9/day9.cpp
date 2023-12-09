#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>

using data_t = std::vector<std::vector<int>>;

data_t parse_input(std::istream& is) {
	data_t data;

	std::string line;
	while (std::getline(is, line)) {
		if (line.empty())
			continue;
		data.emplace_back();
		std::stringstream ss(line);
		
		int value;
		while (ss >> value) {
			data.back().push_back(value);
		}
	}

	return data;
}

std::vector<int> get_difference(const std::vector<int>& row) {
	std::vector<int> result;
	for (size_t i = 1; i < row.size(); ++i) {
		result.push_back(row[i] - row[i - 1]);
	}
	return result;
}

data_t analize(const std::vector<int>& row) {
	data_t data;
	data.emplace_back(row);

	while (data.back().end() != std::find_if(data.back().begin(), data.back().end(), [](int x) { return x != 0; })) {
		data.emplace_back(get_difference(data.back()));
	}

	return data;
}

int predict_next(const std::vector<int>& source, std::vector<int>& next) {
	next.push_back(next.back() + source.back());
	return next.back();
}

int predict_first(const std::vector<int>& source, std::vector<int>& next) {
	next.insert(next.begin(), *next.begin() - *source.begin());
	return *next.begin();
}

auto p1(data_t data) {
	int result = 0;
	for (auto& row : data) {
		auto differences = analize(row);
		for (size_t i = differences.size() - 1; i > 0; --i) {
			predict_next(differences[i], differences[i - 1]);
		}
		result += differences[0].back();
	}

	return result;
}

auto p2(data_t data) {
	int result = 0;
	for (auto& row : data) {
		auto differences = analize(row);
		for (size_t i = differences.size() - 1; i > 0; --i) {
			predict_first(differences[i], differences[i - 1]);
		}
		result += *differences[0].begin();
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
	auto data = parse_input(ifs);

	auto t1 = std::chrono::high_resolution_clock::now();
	auto r1 = p1(data);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto r2 = p2(data);

	auto t3 = std::chrono::high_resolution_clock::now();

	std::cout << "P1 result: " << r1 << "\n";
	std::cout << "P2 result: " << r2 << "\n";
	std::cout << "Input duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << "ms" << std::endl;
	std::cout << "P1 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	std::cout << "P2 duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << "ms" << std::endl;
}
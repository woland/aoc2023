#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

void p1(std::istream& ifs)
{
	int sum = 0;
	for (std::string str; std::getline(ifs, str) && !str.empty();)
	{
		int first = *std::find_if(std::begin(str), std::end(str), [](unsigned char ch) { return std::isdigit(ch); }) - '0';
		int last = *std::find_if(std::rbegin(str), std::rend(str), [](unsigned char ch) { return std::isdigit(ch); }) - '0';
		sum += first * 10 + last;
	}

	std::cout << " P1 result: " << sum << std::endl;
}

void p2(std::istream& ifs)
{
	static const std::vector<std::tuple<std::string, int>> digits = {
		std::make_tuple("one", 1),
		std::make_tuple("two", 2),
		std::make_tuple("three", 3),
		std::make_tuple("four", 4),
		std::make_tuple("five", 5),
		std::make_tuple("six", 6),
		std::make_tuple("seven", 7),
		std::make_tuple("eight", 8),
		std::make_tuple("nine", 9),
		std::make_tuple("1", 1),
		std::make_tuple("2", 2),
		std::make_tuple("3", 3),
		std::make_tuple("4", 4),
		std::make_tuple("5", 5),
		std::make_tuple("6", 6),
		std::make_tuple("7", 7),
		std::make_tuple("8", 8),
		std::make_tuple("9", 9)
	};

	int sum = 0;

	for (std::string str; std::getline(ifs, str) && !str.empty();)
	{
		auto pp = str.rfind("nine");

		int first = 0, last = 0;
		
		auto fpos = std::string::npos;
		for (auto& [digit, num] : digits)
		{
			auto npos = std::min(fpos, str.find(digit));
			if (fpos != npos)
			{
				fpos = npos;
				first = num;
			}
		}

		size_t lpos = std::string::npos;
		for (auto& [digit, num] : digits)
		{
			auto npos = str.rfind(digit);
			if (npos != std::string::npos && lpos == std::string::npos)
			{
				lpos = npos;
				last = num;
			}

			if (npos != std::string::npos && lpos != npos && std::max(npos, lpos) > lpos)
			{
				lpos = npos;
				last = num;
			}
		}

		sum += first * 10 + last;
	}

	std::cout << " P2 result: " << sum << std::endl;
}

int main()
{
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

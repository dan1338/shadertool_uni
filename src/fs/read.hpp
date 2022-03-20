#pragma once
#include <fstream>
#include <string>
#include <vector>

namespace fs
{
	static inline std::vector<std::string> read_lines(const std::string &path)
	{
		std::ifstream fp(path);

		if (!fp.is_open())
			throw std::runtime_error(std::string("Cannot open script file ") + path);

		std::string s;
		std::vector<std::string> lines;

		while (std::getline(fp, s))
			lines.push_back(s);

		return lines;
	}

	static inline std::string read_string(const std::string &path)
	{
		std::string s;
		
		for (const auto &line : read_lines(path))
			s += line + "\n";

		return s;
	}
}


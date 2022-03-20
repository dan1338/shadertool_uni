#pragma once
#include <vector>
#include <string>

namespace script
{
	class File
	{
	public:
		File(const std::string &path);
		auto begin() const -> const std::string* { return lines.data(); }
		auto end() const -> const std::string* { return lines.data() + lines.size(); }
		auto get_name() const -> const std::string;
	private:
		const std::string path;
		std::vector<std::string> lines;
	};
}


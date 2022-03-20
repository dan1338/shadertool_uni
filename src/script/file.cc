#include <string>
#include "file.hpp"
#include "fs/read.hpp"

namespace script
{

File::File(const std::string &path):
	path(path),
	lines(fs::read_lines(path))
{
}

auto File::get_name() const -> const std::string
{
	const auto i = path.rfind("/");

	if (i == std::string::npos)
		return path;

	return path.substr(i + 1);
}

}


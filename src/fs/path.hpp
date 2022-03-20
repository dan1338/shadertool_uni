#pragma once
#include <string>

namespace fs
{
	static inline std::string basename(const std::string &path)
	{
		const size_t p = path.rfind("/");

		if (p == std::string::npos)
			return path;

		return path.substr(p + 1, path.size());
	}

	static inline std::string dirname(const std::string &path)
	{
		const size_t p = path.rfind("/");

		if (p == std::string::npos)
			return ".";
		if (p == 0)
			return "/";

		return path.substr(0, p);
	}

	static inline std::string join_path(const std::string &base, const std::string &suffix)
	{
		std::string path(base);

		if (path.back() != '/')
			path += '/';

		return path + suffix;
	}
}


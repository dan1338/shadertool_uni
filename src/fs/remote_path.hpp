#pragma once
#include <string>
#include <functional>
#include "cmdline/args.hpp"

namespace fs
{
	// Wrapper class for representing fs path
	// Supports deferring path resolution to cmdline::Args
	class RemotePath
	{
	public:
		RemotePath(const std::string &arg, cmdline::Args &cargs);
		operator std::string();
	private:
		std::string path;
		std::function<std::string()> resolve;
	};

}


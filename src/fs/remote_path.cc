#include "remote_path.hpp"
#include "path.hpp"
#include <iostream>

namespace fs
{

RemotePath::RemotePath(const std::string &arg, cmdline::Args &cargs)
{
	const size_t at = arg.find("@");

	if (at == std::string::npos || at > 0)
	{
		// Rebase path target with respect to script basedir
		const auto base = fs::dirname(cargs.first());
		path = fs::join_path(base, arg);
		std::cerr << "path=" << path << "\n";
		return;
	}

	char sym;

	if (sscanf(arg.c_str(), "@OPT(%c)", &sym) == 1)
	{
		cargs.register_sym(sym, true);

		// TODO: replace with ref cargs capture and sym value pass
		struct {
			cmdline::Args &args;
			char sym;
		} tmp = { cargs, sym };

		resolve = [=](){ return tmp.args[tmp.sym]; };

		return;
	}

	throw std::invalid_argument("Not a valid path");
}

RemotePath::operator std::string()
{
	if (!path.size())
	{
		if (!resolve)
			throw std::invalid_argument("Empty path argument");

		path = resolve();
	}

	return path;
}

}


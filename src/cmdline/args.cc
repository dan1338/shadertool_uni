#include <cstring>
#include <stdexcept>
#include "args.hpp"

namespace cmdline
{

Args::Args(const int argc, const char **argv):
	argc(argc), argv(argv)
{
	if (!first())
		throw cmdline::invalid_argument("Missing positional argument");
}

auto Args::parse_opts() -> void
{
	const char *prev_arg = 0;
	bool has_arg = false;

	const auto lookup = [=](const char* x){
		size_t pos = fmt.find(x);
		if (pos == fmt.npos)
			return -1;
		if ((pos + 1 < fmt.size()) && fmt[pos + 1] == ':')
			return 1;
		return 0;
	};

	for (size_t i = 2; i < argc; ++i)
	{
		const char *arg = argv[i];

		if (prev_arg && has_arg)
		{
			opts.push_back({ prev_arg[1], arg });
			prev_arg = 0;
			has_arg = false;
		}
		else if (*arg == '-')
		{
			int res = lookup(arg + 1);

			if (res == -1)
				throw cmdline::invalid_argument("Unrecognized option", arg);

			has_arg = res;

			if (has_arg)
				prev_arg = arg;
			else
				opts.push_back({ arg[1], "" });

		}
		else
		{
			throw cmdline::invalid_argument("Unrecognized argument", arg);
		}
	}

	if (has_arg)
		throw cmdline::invalid_argument("Missing option argument", prev_arg);
}

auto Args::require(const char sym, const bool has_arg) -> void
{
	const Opt *found = 0;

	for (const auto &opt : opts)
	{
		if (opt.sym == sym)
		{
			found = &opt;
			break;
		}
	}

	if (!found)
		throw std::invalid_argument("Required opt missing");

	if (has_arg && !found->arg.size())
		throw std::invalid_argument("Required arg missing");
}

auto Args::register_sym(const char sym, const bool has_arg) -> void
{
	fmt += sym;

	if (has_arg)
		fmt.append(":");
}

const std::string &Args::operator[](const char sym) const
{
	for (const auto &opt : opts)
	{
		if (opt.sym == sym)
			return opt.arg;
	}

	throw std::invalid_argument(std::string("Missing required option ") + sym);
}

}


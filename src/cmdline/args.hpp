#pragma once
#include <vector>
#include <string>
#include <stdexcept>

namespace cmdline
{
	class invalid_argument : public std::invalid_argument
	{
	public:
		invalid_argument(const std::string &what, const char *arg = nullptr):
			std::invalid_argument(arg? (what + " " + arg) : what)
		{
		}
	};

	class Args
	{
	public:
		struct Opt
		{
			const char sym;
			const std::string arg;
		};

		Args(const int argc, const char **argv);
		auto parse_opts() -> void;
		auto require(const char sym, const bool has_arg = false) -> void;
		auto register_sym(const char sym, const bool has_arg) -> void;

		auto first() const -> const char* { return (argc > 1)? argv[1] : 0; };
		auto last() const -> const char* { return (argc > 1)? argv[argc - 1] : 0; };
		auto begin() const -> const Opt* { return opts.data(); }
		auto end() const -> const Opt* { return opts.data() + opts.size(); }

		const std::string &operator[](const char sym) const;
	private:
		const int argc;
		const char **argv;
		std::vector<Opt> opts;
		std::string fmt;
	};

	const std::string usage = "usage: ./shadertool script OPTIONS...";
}


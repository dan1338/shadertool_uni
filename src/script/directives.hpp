#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include "cmdline/args.hpp"
#include "fs/remote_path.hpp"

namespace script
{
	struct ExecutionContext;

	class invalid_directive_nargs : public std::invalid_argument
	{
	public:
		invalid_directive_nargs(const std::string &name):
			std::invalid_argument(std::string("Invalid number of arguments to ") + name)
		{
		}
	};

	class Directive
	{
	public:
		Directive(const std::string &name, size_t nargs, const std::vector<std::string> args):
			name(name), args(args)
		{
			if (args.size() != nargs)
				throw invalid_directive_nargs(name);
		}
		virtual ~Directive() = default;
		virtual auto apply(ExecutionContext &ctx) -> void = 0;
		const std::string &get_name() const { return name; }
	protected:
		const std::string name;
		const std::vector<std::string> args;
	};

	class ShaderDirective : public Directive
	{
	public:
		ShaderDirective(const std::vector<std::string> &args, cmdline::Args &cargs);
		auto apply(ExecutionContext &ctx) -> void override;
	private:
		const std::string alias;
		fs::RemotePath path;
	};

	class SourceDirective : public Directive
	{
	public:
		SourceDirective(const std::vector<std::string> &args, cmdline::Args &cargs);
		auto apply(ExecutionContext &ctx) -> void override;
	private:
		const std::string alias;
		fs::RemotePath path;
	};

	class SinkDirective : public Directive
	{
	public:
		SinkDirective(const std::vector<std::string> &args, cmdline::Args &cargs);
		auto apply(ExecutionContext &ctx) -> void override;
	private:
		const std::string alias;
		fs::RemotePath path;
	};

	class ExecDirective : public Directive
	{
	public:
		ExecDirective(const std::vector<std::string> &args, cmdline::Args &cargs);
		auto apply(ExecutionContext &ctx) -> void override;
	};
}


#include <memory>
#include <cstring>
#include "parser.hpp"
#include "directives.hpp"

namespace script
{

Parser::Parser(const script::File &file, cmdline::Args &args):
	cargs(args)
{
	size_t line_num = 0;
	bool header = true;

	for (const auto &line : file)
	{
		line_num += 1;

		if (line.size() < 1)
			continue;

		try
		{
			if (header)
			{
				header = parse_head(line);
			}
			else
			{
				parse_body(line);
			}
		}
		catch (std::invalid_argument &e)
		{
			std::string what;
			what += file.get_name();
			what += ":";
			what += std::to_string(line_num);

			throw std::invalid_argument(what + " " + e.what());
		}
	}
}

auto Parser::parse_head(const std::string &line) -> bool
{
	if (line[0] == '.')
	{
		auto directive = parse_directive(line);

		if (dynamic_cast<ExecDirective*>(directive.get()))
			return false;

		directives.push_back(std::move(directive));
	}

	return true;
}

auto Parser::parse_body(const std::string &line) -> void
{
	if (line[0] == '.')
		throw std::invalid_argument("Directive found in exec section");

	auto insn = parse_instruction(line);
	instructions.push_back(std::move(insn));
}

static inline std::string next_word(const char *p, char delim)
{
	const char *s = strchr(p, delim);

	if (!s)
		return std::string(p);

	return std::string(p).substr(0, s - p);
}

static inline const char *skip_whitespace(const char *p)
{
	while (*p == ' ' || *p == '\t')
		++p;

	return p;
}

static inline std::vector<std::string> parse_args(const char *p, char delim)
{
	std::vector<std::string> args;

	while (1)
	{
		const auto arg = next_word(p, delim);

		if (!arg.size())
			break;

		args.push_back(arg);
		p = skip_whitespace(p + arg.size() + 1);
	}

	return args;
}

auto Parser::parse_directive(const std::string &line) -> std::unique_ptr<Directive>
{
	const char *p = line.c_str();

	if (*p == '.')
		++p;

	const auto name = next_word(p, ' ');
	const auto args = parse_args(skip_whitespace(p + name.size() + 1), ' ');

	if (name == "shader")
		return std::make_unique<ShaderDirective>(args, cargs);
	else if (name == "source")
		return std::make_unique<SourceDirective>(args, cargs);
	else if (name == "sink")
		return std::make_unique<SinkDirective>(args, cargs);
	else if (name == "exec")
		return std::make_unique<ExecDirective>(args, cargs);

	throw std::invalid_argument("Invalid directive");
}

auto Parser::parse_instruction(const std::string &line) -> std::unique_ptr<Instruction>
{
	const char *p = line.c_str();
	const auto name = next_word(p, ' ');
	const auto args = parse_args(skip_whitespace(p + name.size() + 1), ',');

	if (name == "pull")
		return std::make_unique<PullInstruction>(args);
	else if (name == "push")
		return std::make_unique<PushInstruction>(args);
	else if (name == "apply")
		return std::make_unique<ApplyInstruction>(args);
	else if (name == "jmp")
		return std::make_unique<JmpInstruction>(args);
	else if (name == "jmpif")
		return std::make_unique<JmpIfInstruction>(args);
	else if (name == "mov")
		return std::make_unique<MovInstruction>(args);
	else if (name == "add")
		return std::make_unique<AddInstruction>(args);

	throw std::invalid_argument("Invalid instruction");
}

}


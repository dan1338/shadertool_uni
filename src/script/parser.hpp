#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "file.hpp"
#include "directives.hpp"
#include "instructions.hpp"
#include "cmdline/args.hpp"

namespace script
{
	class Parser
	{
	public:
		Parser(const script::File &file, cmdline::Args &args);
		auto get_directives() const -> const std::vector<std::unique_ptr<Directive>>& { return directives; }
		auto get_instructions() const -> const std::vector<std::unique_ptr<Instruction>>& { return instructions; }
	private:
		auto parse_head(const std::string &line) -> bool;
		auto parse_body(const std::string &line) -> void;
		auto parse_directive(const std::string &line) -> std::unique_ptr<Directive>;
		auto parse_instruction(const std::string &line) -> std::unique_ptr<Instruction>;

		cmdline::Args &cargs;
		std::vector<std::unique_ptr<Directive>> directives;
		std::vector<std::unique_ptr<Instruction>> instructions;
	};
}


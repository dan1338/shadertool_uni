#include <stdexcept>
#include <iostream>
#include "interpreter.hpp"
#include "directives.hpp"

namespace script
{

Interpreter::Interpreter(const script::File &file, cmdline::Args &args):
	parser(file, args), file(file), args(args)
{
	args.parse_opts();

	for (const auto &directive : parser.get_directives())
		directive->apply(ctx);
}

auto Interpreter::exec() -> bool
{
	if (ctx.pc >= parser.get_instructions().size())
		return false;

	const auto &insn = parser.get_instructions()[ctx.pc];

	try
	{
		insn->exec(ctx);
		ctx.pc += 1;
	}
	catch (std::runtime_error &e)
	{
		std::cout << "*** interpreter error @ pc=" << std::to_string(ctx.pc) << " ***\n";
		throw e;
	}

	return true;
}

}


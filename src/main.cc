#include <iostream>
#include <stdexcept>
#include <vector>

#include "cmdline/args.hpp"
#include "script/file.hpp"
#include "script/interpreter.hpp"

int main(int argc, char **argv)
{
	try
	{
		cmdline::Args args(argc, const_cast<const char**>(argv));

		script::File file(args.first());
		script::Interpreter interp(file, args);

		while (1)
		{
			if (!interp.exec())
				break;
		}
	}
	catch (std::invalid_argument &e)
	{
		std::cout << cmdline::usage << std::endl;
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}


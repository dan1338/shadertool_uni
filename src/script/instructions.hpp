#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>

namespace script
{
	struct ExecutionContext;

	class invalid_insn_nargs : public std::invalid_argument
	{
	public:
		invalid_insn_nargs(const std::string &name):
			std::invalid_argument(std::string("Invalid number of arguments to ") + name)
		{
		}
	};

	class Instruction
	{
	public:
		Instruction(const std::string &name, size_t nargs, const std::vector<std::string> &args):
			name(name), args(args)
		{
			if (args.size() < nargs)
				throw invalid_insn_nargs(name);
		}

		virtual auto exec(ExecutionContext &ctx) -> void = 0;
		virtual ~Instruction() = default;
	protected:
		const std::string name;
		const std::vector<std::string> args;
	};

	// Load frame from <source_name> into r<regid> register
	// usage: pull r<regid>, <source_name>
	class PullInstruction : public Instruction
	{
	public:
		PullInstruction(const std::vector<std::string> &args);
		auto exec(ExecutionContext &ctx) -> void override;
	private:
		const size_t regid;
		const std::string source_name;
	};

	// Store frame from r<regid> into <sink_name>
	// usage: push <sink_name>, r<regid>
	class PushInstruction : public Instruction
	{
	public:
		PushInstruction(const std::vector<std::string> &args);
		auto exec(ExecutionContext &ctx) -> void override;
	private:
		const size_t regid;
		const std::string sink_name;
	};

	// Apply <shader_name> and store result in r0
	// usage: apply <shader_name>, [uniform=r<regid>]...
	class ApplyInstruction : public Instruction
	{
	public:
		ApplyInstruction(const std::vector<std::string> &args);
		auto exec(ExecutionContext &ctx) -> void override;
	private:
		const std::string shader_name;
		std::map<std::string, std::string> uniforms;
	};

	// Jump <offset> instructions relative to pc
	// usage: jmp <offset>
	class JmpInstruction : public Instruction
	{
	public:
		JmpInstruction(const std::vector<std::string> &args);
		auto exec(ExecutionContext &ctx) -> void override;
	private:
		const int offset;
	};

	// Jump <offset> instructions relative to pc if condition is true
	// usage: jmpif <offset>, [!]<cond>
	//
	// <cond> can be any of ctx.flags [rdok, rdany, wrok, zero]
	// the optional ! prefix negates the <cond> expression
	class JmpIfInstruction : public Instruction
	{
	public:
		JmpIfInstruction(const std::vector<std::string> &args);
		auto exec(ExecutionContext &ctx) -> void override;
	private:
		const int offset;
		const bool neg;
		const std::function<bool(const ExecutionContext&)> eval;
	};
}


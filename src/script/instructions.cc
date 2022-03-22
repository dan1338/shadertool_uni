#include <cstring>
#include <iostream>
#include "interpreter.hpp"
#include "instructions.hpp"

namespace script
{

static inline size_t get_regid(const std::string &s)
{
	if (s.size() < 1)
		throw std::invalid_argument("Invalid register");
	
	unsigned id;

	if (sscanf(s.c_str(), "r%u", &id) == 1)
	{
		if (id < num_registers)
			return id;
	}

	throw std::invalid_argument("Invalid register");
}

PullInstruction::PullInstruction(const std::vector<std::string> &args):
	Instruction("pull", 2, args),
	regid(get_regid(args[0])),
	source_name(args[1])
{
}

auto PullInstruction::exec(ExecutionContext &ctx) -> void
{
	const auto source_pair = ctx.sources.find(source_name);

	if (source_pair == ctx.sources.end())
		throw std::runtime_error("Invalid frame source");

	auto &reg = ctx.regs[regid];
	auto &source = source_pair->second;

	// Create new frame only if it doesn't already exist or has wrong size
	if (!reg->try_cast<video::Frame>() || reg->cast<video::Frame>()->get_spec() != source.get_frame_spec())
	{
		ctx.regs[regid] = std::make_unique<video::Frame>(source.get_frame_spec());
	}

	auto &frame = *ctx.regs[regid]->cast<video::Frame>();
	ctx.flags.rdok = source >> frame;
}

PushInstruction::PushInstruction(const std::vector<std::string> &args):
	Instruction("push", 2, args),
	sink_name(args[0]),
	regid(get_regid(args[1]))
{
}

auto PushInstruction::exec(ExecutionContext &ctx) -> void
{
	const auto sink_pair = ctx.sinks.find(sink_name);

	if (sink_pair == ctx.sinks.end())
		throw std::runtime_error("Invalid frame sink");

	auto &sink = sink_pair->second;
	auto &frame = *ctx.regs[regid]->cast<video::Frame>();
	ctx.flags.wrok = sink << frame;
}

static inline std::pair<std::string, std::string> split_uniform(const std::string &s)
{
	const size_t eq = s.find('=');

	if (eq == std::string::npos)
		throw std::invalid_argument("Invalid uniform parameter");

	return std::make_pair(s.substr(0, eq), s.substr(eq + 1, s.size()));
}

ApplyInstruction::ApplyInstruction(const std::vector<std::string> &args):
	Instruction("apply", 1, args),
	shader_name(args[0])
{
	for (size_t i = 1; i < args.size(); ++i)
	{
		const auto uniform = split_uniform(args[i]);
		uniforms.insert(uniform);
	}
}	

auto ApplyInstruction::exec(ExecutionContext &ctx) -> void
{
	const auto shader_pair = ctx.shaders.find(shader_name);

	if (shader_pair == ctx.shaders.end())
		throw std::runtime_error("Invalid shader name");

	auto &shader = shader_pair->second;
	auto &dev = ctx.render_dev;

	dev.set_target(*ctx.render_target);
	dev.use_shader(shader);

	for (const auto &uniform : uniforms)
	{
		const auto &name = uniform.first;
		const auto regid = get_regid(uniform.second);
		auto &reg = ctx.regs[regid];

		if (reg->try_cast<video::Frame>())
			dev.load_texture(name, *reg->cast<video::Frame>());
	}

	dev.clear();
	dev.draw();

	if (!ctx.regs[0]->try_cast<video::Frame>())
		ctx.regs[0] = std::make_unique<video::Frame>(dev.get_frame_spec());
	
	auto &dstframe = *ctx.regs[0]->cast<video::Frame>();
	dev.read_frame(dstframe);
}

static inline int get_offset(const std::string &s)
{
	try
	{
		return std::stoi(s);
	}
	catch (...)
	{
		throw std::invalid_argument("Invalid jmp offset");
	}
}

JmpInstruction::JmpInstruction(const std::vector<std::string> &args):
	Instruction("jmp", 1, args),
	offset(get_offset(args[0]))
{
}

auto JmpInstruction::exec(ExecutionContext &ctx) -> void
{
	ctx.pc += offset;
	ctx.pc -= 1; // Accounts for interpreter post increment
}

static inline bool is_expr_neg(const std::string &s)
{
	return s[0] == '!';
}

static inline std::function<bool(const ExecutionContext&)> get_eval_func(const std::string &s)
{
	std::string expr = s.substr(is_expr_neg(s)? 1:0, s.size());

	if (expr == "rdok")
		return [](const ExecutionContext &ctx){ return ctx.flags.rdok; };
	else if (expr == "wrok")
		return [](const ExecutionContext &ctx){ return ctx.flags.wrok; };
	else if (expr == "zero")
		return [](const ExecutionContext &ctx){ return ctx.flags.zero; };

	throw std::invalid_argument("Invalid conditional jump expression");
}

JmpIfInstruction::JmpIfInstruction(const std::vector<std::string> &args):
	Instruction("jmpif", 2, args),
	offset(get_offset(args[0])),
	neg(is_expr_neg(args[1])),
	eval(get_eval_func(args[1]))
{
}

auto JmpIfInstruction::exec(ExecutionContext &ctx) -> void
{
	bool cond = eval(ctx);

	if (cond ^ neg)
	{
		ctx.pc += offset;
		ctx.pc -= 1; // Accounts for interpreter post increment
	}
}


}


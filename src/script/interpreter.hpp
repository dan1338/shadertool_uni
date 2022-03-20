#pragma once
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include "file.hpp"
#include "parser.hpp"
#include "register.hpp"
#include "cmdline/args.hpp"
#include "video/frame_source.hpp"
#include "video/frame_sink.hpp"
#include "graphics/shader_program.hpp"
#include "graphics/render_device.hpp"
#include "graphics/render_target.hpp"

namespace script
{
	static constexpr auto num_registers = 8U;

	struct ExecutionContext
	{
		size_t pc;
		std::map<std::string, video::FrameSource> sources;
		std::map<std::string, video::FrameSink> sinks;
		std::map<std::string, graphics::ShaderProgram> shaders;
		graphics::RenderDevice render_dev;
		std::unique_ptr<graphics::RenderTarget> render_target;
		std::array<std::unique_ptr<Register>, num_registers> regs;
		struct {
			int rdok:1;
			int wrok:1;
			int zero:1;
		} flags;

		ExecutionContext():
			pc(0), render_dev(1, 1)
		{
		}
	};

	class Interpreter
	{
	public:
		friend class Instruction;
		friend class Directive;
		Interpreter(const script::File &file, cmdline::Args &args);
		auto exec() -> void;
		auto get_ctx() -> ExecutionContext& { return ctx; }
	private:
		ExecutionContext ctx;
		script::Parser parser;
		const script::File &file;
		const cmdline::Args &args;
	};
}


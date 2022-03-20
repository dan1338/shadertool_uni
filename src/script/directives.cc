#include <string>
#include <cstring>
#include "interpreter.hpp"
#include "directives.hpp"
#include "video/frame_source.hpp"
#include "cmdline/args.hpp"

namespace script
{

static const std::string ensure_alias(const std::string &path)
{
	for (const char c : path)
	{
		if (!isalnum(c))
			throw std::invalid_argument(std::string("Invalid alias ") + path);
	}

	return path;
}

ShaderDirective::ShaderDirective(const std::vector<std::string> &args, cmdline::Args &cargs):
	Directive("shader", 2, args),
	alias(ensure_alias(args[0])),
	path(args[1], cargs)
{
}

auto ShaderDirective::apply(ExecutionContext &ctx) -> void
{
	const auto shader = graphics::Shader(graphics::Shader::FRAGMENT, path);
	ctx.shaders.emplace(std::piecewise_construct,
			std::forward_as_tuple(alias),
			std::forward_as_tuple(shader));
}

SourceDirective::SourceDirective(const std::vector<std::string> &args, cmdline::Args &cargs):
	Directive("source", 2, args),
	alias(ensure_alias(args[0])),
	path(args[1], cargs)
{
}

auto SourceDirective::apply(ExecutionContext &ctx) -> void
{
	ctx.sources.emplace(std::piecewise_construct,
			std::forward_as_tuple(alias),
			std::forward_as_tuple(path));

	if (!ctx.render_target)
	{
		const auto &source = ctx.sources.begin()->second;
		const auto spec = source.get_frame_spec();
		ctx.render_target = std::make_unique<graphics::RenderTarget>(spec.width, spec.height);
	}
}

SinkDirective::SinkDirective(const std::vector<std::string> &args, cmdline::Args &cargs):
	Directive("sink", 2, args),
	alias(ensure_alias(args[0])),
	path(args[1], cargs)
{
}

auto SinkDirective::apply(ExecutionContext &ctx) -> void
{
	if (ctx.sources.size() < 1)
		throw std::invalid_argument("Sink has no matching source");

	const auto &source = ctx.sources.begin()->second;
	ctx.sinks.emplace(std::piecewise_construct,
			std::forward_as_tuple(alias),
			std::forward_as_tuple(path, source));
}

ExecDirective::ExecDirective(const std::vector<std::string> &args, cmdline::Args &cargs):
	Directive("exec", 0, args)
{
}

auto ExecDirective::apply(ExecutionContext &ctx) -> void
{
}

}


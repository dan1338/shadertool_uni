#pragma once
#include <string>
#include <vector>

namespace graphics
{
	class ShaderProgram;

	class Shader
	{
	public:
		friend class ShaderProgram;

		enum Type {
			VERTEX,
			FRAGMENT
		};

		Shader(const Shader::Type type, const std::string &path);
		Shader(const Shader::Type type, const std::string &name, const std::string &src);
		~Shader();
	private:
		int id;
		const Shader::Type type;
		const std::string src;
		const std::string name;
	};
}


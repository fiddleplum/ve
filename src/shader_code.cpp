#include "shader_code.h"
#include "util.h"

namespace ve
{
	ShaderCode::ShaderCode(std::string const & filename)
	{
		code = readFile(filename);
		if (endsWith(filename, ".vert"))
		{
			type = Type::Vertex;
		}
		else if (endsWith(filename, ".frag"))
		{
			type = Type::Fragment;
		}
		else
		{
			throw std::runtime_error("Invalid extension for shader code: " + filename);
		}
	}

	ShaderCode::ShaderCode(std::string const & code_, Type type_)
	{
		code = code_;
		type = type_;
	}

	std::string const & ShaderCode::getCode() const
	{
		return code;
	}

	ShaderCode::Type ShaderCode::getType() const
	{
		return type;
	}
}


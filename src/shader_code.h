#include "cache.h"
#include <string>

namespace ve
{
	class ShaderCode
	{
	public:
		// The possible types of shader codes.
		enum Type { Vertex, Fragment, NumCodeTypes };

		// Constructs from a .vert or .frag file.
		ShaderCode(std::string const & filename);

		// Constructs a ShaderCode from code and a type.
		ShaderCode(std::string const & code, Type type);

		// Returns the code.
		std::string const & getCode() const;

		// Returns the type.
		Type getType() const;

		// Cache for ShaderCodes
		static Cache<ShaderCode> cache;

	private:
		std::string code;
		Type type;
	};
}


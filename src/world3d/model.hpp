#pragma once

#include "render/model.hpp"

namespace ve
{
	namespace world3d
	{
		class Model final
		{
		public:
			Model();

			Model(std::string const & filename);

			~Model();

		private:
			OwnPtr<ve::Model> model;
		};
	}
}
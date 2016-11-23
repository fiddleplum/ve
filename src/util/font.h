#pragma once

#include "render/texture.h"
#include "util/ptr.h"
#include <SDL_ttf.h>
#include <string>
#include <map>

namespace ve
{
	class Font final
	{
	public:
		Font(std::string const & filename, int size);

		~Font();

	private:
		struct Block
		{
			Ptr<Texture> texture;
			std::vector<unsigned int> widths;
			unsigned int cellSize;
		};

		void loadBlock(int blockStart);

		static int numFontsLoaded;
		static unsigned int numCharsInRow;
		static unsigned int numCharsInCol;
		TTF_Font * ttfFont;
		std::map<int, Block> blocks;
		int size;
		unsigned int heightOfChar;
	};
}
#include "font.h"
#include "math.hpp"
#include "app_internal.h"

namespace ve
{
	int Font::numFontsLoaded = 0;
	unsigned int Font::numCharsInRow = 16;
	unsigned int Font::numCharsInCol = 8;

	Font::Font(std::string const & filename, int size_)
	{
		size = size_;

		// Initialize SDL TTF if needed.
		if (numFontsLoaded == 0)
		{
			auto status = TTF_Init();
			if (status == -1)
			{
				throw std::runtime_error(std::string() + "SDL_ttf failed to initialize. " + TTF_GetError());
			}
		}

		// Load the font.
		ttfFont = TTF_OpenFont(filename.c_str(), size);
		if (ttfFont == 0)
		{
			throw std::runtime_error("The font '" + filename + "' at size " + std::to_string(size) + " could not be loaded. ");
		}
		numFontsLoaded++;
		loadBlock(0);
	}

	Font::~Font()
	{
		TTF_CloseFont(ttfFont);
		numFontsLoaded--;
		if (numFontsLoaded == 0)
		{
			TTF_Quit();
		}
	}

	void Font::loadBlock(int blockStart)
	{
		// Create a texture for a block of characters starting with the blockStart.
		Block block;
		int numCharsInBlock = numCharsInRow * numCharsInCol;
		block.widths.resize(numCharsInBlock);
		block.cellSize = 2 * math::ceilPow2(size);
		SDL_Color white = {255, 255, 255, 255};
		SDL_Surface * surface = SDL_CreateRGBSurface(0, block.cellSize * numCharsInRow, block.cellSize * numCharsInCol, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		for (int i = 0; i < numCharsInBlock; i++)
		{
			SDL_Surface * glyphSurface = TTF_RenderGlyph_Blended(ttfFont, blockStart + i, white);
			if (glyphSurface != nullptr)
			{
				heightOfChar = glyphSurface->h;
				block.widths[i] = glyphSurface->w;
				SDL_Rect rect;
				rect.x = (i % numCharsInRow) * block.cellSize + (block.cellSize - glyphSurface->w) / 2;
				rect.y = (i / numCharsInRow) * block.cellSize + (block.cellSize - glyphSurface->h) / 2;
				SDL_BlitSurface(glyphSurface, 0, surface, &rect);
				SDL_FreeSurface(glyphSurface);
			}
		}
		std::string resourceName = TTF_FontFaceFamilyName(ttfFont) + std::to_string(blockStart);
		auto store = getAppInternal()->getStoreInternal();
		auto image = store->createImage(resourceName, surface);
		SDL_FreeSurface(surface);
		block.texture = store->createTexture(resourceName, image);
		blocks[blockStart] = block;
	}

}
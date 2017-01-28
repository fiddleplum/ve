#include "render/font.hpp"
#include "util/math.hpp"
#include <SDL_ttf.h>
#include <SDL.h>

namespace ve
{
	namespace render
	{
		unsigned int Font::numFontsLoaded = 0;
		unsigned int Font::numCharsInRow = 16;
		unsigned int Font::numCharsInCol = 8;

		Font::Font(std::string const & filename, int size_)
		{
			size = size_;
			cellSize = 2 * math::ceilPow2(size);

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

			lineHeight = TTF_FontLineSkip((TTF_Font *)ttfFont);
			ascent = TTF_FontAscent((TTF_Font *)ttfFont);

			loadBlock(0);
		}

		Font::~Font()
		{
			TTF_CloseFont((TTF_Font *)ttfFont);
			numFontsLoaded--;
			if (numFontsLoaded == 0)
			{
				TTF_Quit();
			}
		}

		int Font::getLineHeight() const
		{
			return lineHeight;
		}

		Font::GlyphCoords const & Font::getGlyphCoordsFromChar(unsigned int c)
		{
			Block const & block = getBlock(c);
			return block.glyphCoords[c - block.start];
		}

		Ptr<Image> Font::getImageFromChar(unsigned int c)
		{
			Block const & block = getBlock(c);
			return block.image;
		}

		Font::Block const & Font::getBlock(unsigned int c)
		{
			auto blockStart = c / (numCharsInCol * numCharsInRow) * (numCharsInCol * numCharsInRow);
			auto blockIt = blocks.find(blockStart);
			if (blockIt == blocks.end())
			{
				loadBlock(blockStart);
				blockIt = blocks.find(blockStart);
			}
			return blockIt->second;
		}

		void Font::loadBlock(int blockStart)
		{
			// Create a image for a block of characters starting with the blockStart.
			Block block;
			unsigned int numCharsInBlock = numCharsInRow * numCharsInCol;
			block.glyphCoords.resize(numCharsInBlock);
			SDL_Color white = {255, 255, 255, 255};
			SDL_Surface * surface = SDL_CreateRGBSurface(0, cellSize * numCharsInRow, cellSize * numCharsInCol, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
			for (unsigned int i = 0; i < numCharsInBlock; i++)
			{
				int available = TTF_GlyphIsProvided((TTF_Font *)ttfFont, blockStart + i);
				SDL_Surface * glyphSurface = TTF_RenderGlyph_Blended((TTF_Font *)ttfFont, blockStart + i, white);
				int minX, maxY, advance, minY;
				TTF_GlyphMetrics((TTF_Font *)ttfFont, blockStart + i, &minX, nullptr, &minY, &maxY, &advance);
				if (glyphSurface != nullptr)
				{
					auto & coords = block.glyphCoords[i];
					coords.offset = {0, -ascent};
					coords.uvBounds.min = {(int)((i % numCharsInRow) * cellSize),(int)((i / numCharsInRow) * cellSize)};
					coords.uvBounds.setSize({glyphSurface->w, glyphSurface->h});
					coords.advance = advance;
					SDL_Rect rect;
					rect.x = coords.uvBounds.min[0];
					rect.y = coords.uvBounds.min[1];
					SDL_BlitSurface(glyphSurface, 0, surface, &rect);
					SDL_FreeSurface(glyphSurface);
				}
			}
			std::string resourceName = TTF_FontFaceFamilyName((TTF_Font *)ttfFont) + std::to_string(blockStart);
			block.image = OwnPtr<Image>::returnNew(Vector2i{surface->w, surface->h}, Image::RGBA32);
			std::vector<uint8_t> pixels {&((uint8_t *)surface->pixels)[0], &((uint8_t *)surface->pixels)[surface->pitch * surface->h]};
			block.image->setPixels(pixels);
			block.image->save("test.png");
			SDL_FreeSurface(surface);
			block.start = blockStart;
			blocks[blockStart] = block;
		}

		//void Font::getInfoFromChar(unsigned int c, GlyphInfo & glyphInfo)
		//{
		//	if (c == '\n')
		//	{
		//		
		//		pos[1] += heightOfChar;
		//		pos[0] = 0;
		//		return false;
		//	}
		//	if (c == '\t')
		//	{
		//		pos[0] += heightOfChar;
		//		return false;
		//	}
		//	// Get the block (or load a new one)
		//	auto blockStart = c / numCharsInBlock * numCharsInBlock;
		//	auto blockIt = blocks.find(blockStart);
		//	if (blockIt == blocks.end())
		//	{
		//		loadBlock(blockStart);
		//		blockIt = blocks.find(blockStart);
		//	}
		//	auto block = blockIt->second;
		//	// Set the image.
		//	image = block.image;
		//	// Set the bounds and uvBounds.
		//	bounds.min = {pos[0] - (int)(block.cellSize - block.widths[c - blockStart]) / 2, pos[1] - (int)(block.cellSize - heightOfChar) / 2};
		//	bounds.max = bounds.min + Coord2i {(int)block.cellSize, (int)block.cellSize};
		//	uvBounds.min = {(int)((c - blockStart) % numCharsInRow * block.cellSize), (int)((c - blockStart) / numCharsInRow * block.cellSize)};
		//	uvBounds.max = uvBounds.min + Coord2i {(int)block.cellSize, (int)block.cellSize};
		//	pos[0] += block.widths[c - blockStart];
		//	return true;
		//}

		//void Font::getModelsFromText(std::string const & text, std::vector<Ptr<Model>> & models, Vector2i & textSize)
		//{
		//	models.clear();
		//	textSize = {0, 0};
		//	std::map<Ptr<Image>, int> imagesToModels;
		//	std::vector<std::vector<GuiModel::Vertex>> vertices;
		//	std::vector<std::vector<unsigned int>> indices;
		//	Coord2i pos;
		//	for (auto c : text)
		//	{
		//		Recti bounds;
		//		Recti uvBounds;
		//		Ptr<Image> image;
		//		bool hasInfo = getInfoFromChar(c, pos, bounds, uvBounds, image);
		//		if (!hasInfo)
		//		{
		//			continue;
		//		}
		//		// Get the model (or setup a new one)
		//		int modelIndex = 0;
		//		auto imagesToModelsIt = imagesToModels.find(image);
		//		if (imagesToModelsIt == imagesToModels.end()) // Create a new model for new image.
		//		{
		//			OwnPtr<GuiModel> model;
		//			model.setNew();
		//			model->setImage(image);
		//			models.push_back(model);
		//			vertices.push_back(std::vector<GuiModel::Vertex>());
		//			indices.push_back(std::vector<unsigned int>());
		//			modelIndex = models.size() - 1;
		//			imagesToModels[image] = modelIndex;
		//		}
		//		// Do the vertices
		//		indices[modelIndex].push_back(vertices[modelIndex].size() + 0);
		//		indices[modelIndex].push_back(vertices[modelIndex].size() + 3);
		//		indices[modelIndex].push_back(vertices[modelIndex].size() + 2);
		//		indices[modelIndex].push_back(vertices[modelIndex].size() + 2);
		//		indices[modelIndex].push_back(vertices[modelIndex].size() + 1);
		//		indices[modelIndex].push_back(vertices[modelIndex].size() + 0);
		//		GuiModel::Vertex vertex;
		//		vertex.pos = {bounds.min[0], bounds.min[1]};
		//		vertex.uv = {uvBounds.min[0], uvBounds.min[1]};
		//		vertices[modelIndex].push_back(vertex);
		//		vertex.pos = {bounds.max[0], bounds.min[1]};
		//		vertex.uv = {uvBounds.max[0], uvBounds.min[1]};
		//		vertices[modelIndex].push_back(vertex);
		//		vertex.pos = {bounds.max[0], bounds.max[1]};
		//		vertex.uv = {uvBounds.max[0], uvBounds.max[1]};
		//		vertices[modelIndex].push_back(vertex);
		//		vertex.pos = {bounds.min[0], bounds.max[1]};
		//		vertex.uv = {uvBounds.min[0], uvBounds.max[1]};
		//		vertices[modelIndex].push_back(vertex);
		//		textSize[0] = Math::max(textSize[0], bounds.max[0]);
		//		textSize[1] = Math::max(textSize[1], bounds.max[1]);
		//	}
		//	for (unsigned int i = 0; i < models.size(); i++)
		//	{
		//		models[i]->setVertices(vertices[i]);
		//		models[i]->setIndices(indices[i]);
		//	}
		//}
	}
}
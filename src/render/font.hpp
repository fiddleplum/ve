#pragma once

#include "render/image.hpp"
#include "util/ptr.hpp"
#include "util/rect.hpp"
#include <string>
#include <map>

namespace ve
{
	namespace render
	{
		class Font final
		{
		public:
			struct GlyphCoords
			{
				Vector2i offset; //< The offset to add when placing the character so that the position is the origin.
				Recti uvBounds; //< The image coordinates for the character's glyph.
				int advance; //< The amount to move forward when writing text.
			};

			//! Construct font from file at the given size.
			Font(std::string const & filename, int size);

			//! Destructor.
			~Font();

			//! Returns the height of a line of text.
			int getLineHeight() const;

			//! Get coordinate info about the glyph of a given character.
			GlyphCoords const & getGlyphCoordsFromChar(unsigned int c);

			//! Get image containing the character's glyph.
			Ptr<Image> getImageFromChar(unsigned int c);

			//! Creates a series of models for rendering the given text and calculates the text size.
			//void getModelsFromText(std::string const & text, std::vector<Ptr<Model>> & models, Vector2i & textSize);

		private:
			struct Block
			{
				unsigned int start;
				OwnPtr<Image> image;
				std::vector<GlyphCoords> glyphCoords;
			};

			Block const & getBlock(unsigned int c);

			void loadBlock(int blockStart);

			static unsigned int numFontsLoaded;
			static unsigned int numCharsInRow;
			static unsigned int numCharsInCol;
			int cellSize;
			void * ttfFont;
			std::map<int, Block> blocks;
			int size;
			int lineHeight;
			int ascent;
		};
	}
}
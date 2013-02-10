#pragma once

#include "ui/GUIChanEx/GUIChanEx.h"
#include "soraguiimage.hpp"

namespace gcn
{
	class ImageDisplayerEx :
		public Container
	{
	private:
		Image* images[2];
		Rectangle srcRects[2];
		int curIndex;
		
		sora::SoraSprite* getSprite(Image* image);
		void drawOneImage(Graphics *graphics, int index);

	public:
		ImageDisplayerEx();
		~ImageDisplayerEx();

		void display(const std::wstring filename, Rectangle srcRect = Rectangle(0, 0, 0, 0), bool autoScale = false);

		int getCurIndex() const;
		int getBackIndex() const;
		void flipIndex();

		virtual void draw(Graphics *graphics);
	};
}
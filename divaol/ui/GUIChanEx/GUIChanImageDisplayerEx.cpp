#include "GUIChanImageDisplayerEx.h"

#include "SoraImageEffect.h"
#include "SoraCore.h"

namespace gcn
{
	ImageDisplayerEx::ImageDisplayerEx()
	{
		setOpaque(false);
		images[0] = images[1] = NULL;
		curIndex = 0;
	}

	ImageDisplayerEx::~ImageDisplayerEx()
	{
		for (int i=0; i<1; i++)
			if (images[i])
				delete images[i];
	}

	void ImageDisplayerEx::display(const std::wstring filename, Rectangle srcRect, bool autoScale)
	{
		flipIndex();
		// clear back image effects
		if (images[getBackIndex()] != NULL)
		{
			getSprite(images[getBackIndex()])->clearEffects();
			getSprite(images[getBackIndex()])->setColor(0xFFFFFFFF);
		}

		// if exist current image
		if (images[getCurIndex()] != NULL)
		{
			delete images[getCurIndex()];
			images[getCurIndex()] = NULL;
		}

		// if current no image
		if (filename == L"" || filename == L"NONE")
			images[getCurIndex()] = NULL;
		else
		{
			// load file
			images[getCurIndex()] = Image::load(filename);

			// set srcRect
			if (srcRect.width == 0)
				srcRects[getCurIndex()] = Rectangle(0, 0, images[getCurIndex()]->getWidth(), images[getCurIndex()]->getHeight());
			else
				srcRects[getCurIndex()] = srcRect;

			// set scale
			if (autoScale)
			{
				getSprite(images[getCurIndex()])->setScale(double(getWidth()) / srcRects[getCurIndex()].width,
					double(getHeight()) / srcRects[getCurIndex()].height);
			}
			else
				getSprite(images[getCurIndex()])->setScale(1, 1);

			// add effects
			getSprite(images[getCurIndex()])->addEffect(new sora::SoraImageEffectFade(0.0, 1.0, 0.5));
		}
	}

	sora::SoraSprite* ImageDisplayerEx::getSprite(Image* image)
	{
		return ((SoraGUIImage*)image)->getSprite();
	}

	void ImageDisplayerEx::flipIndex()
	{
		curIndex = curIndex ^ 1;
	}

	int ImageDisplayerEx::getBackIndex() const
	{
		return curIndex ^ 1;
	}

	int ImageDisplayerEx::getCurIndex() const
	{
		return curIndex;
	}

	void ImageDisplayerEx::draw(Graphics *graphics)
	{
		drawOneImage(graphics, getBackIndex());
		drawOneImage(graphics, getCurIndex());
	}

	void ImageDisplayerEx::drawOneImage(Graphics *graphics, int index)
	{
		if (images[index] == NULL)
			return;
		getSprite(images[index])->update(sora::SoraCore::Instance()->getDelta());
		unsigned int t = getSprite(images[index])->getColor();
		graphics->setColor(Color((t & 0x000000FF), (t & 0x0000FF00) >> 8, (t & 0x00FF0000) >> 16, (t & 0xFF000000) >> 24));
		graphics->drawImage(images[index], srcRects[index].x, srcRects[index].y, 0, 0, srcRects[index].width, srcRects[index].height);
	}
}
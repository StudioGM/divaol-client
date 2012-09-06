#include "ui/GUIChanEx/GUIChanContainerEx.h"

namespace gcn
{
	ContainerEx::ContainerEx()
	{
		image = NULL;
		expandBound = false;
		text = L"";
	}

	ContainerEx::~ContainerEx()
	{
		if (image)
		{
			delete image;
			image = NULL;
		}
	}

	void ContainerEx::setExpandBound(bool value)
	{
		expandBound = value;
	}

	bool ContainerEx::getExpandBound() const
	{
		return expandBound;
	}

	void ContainerEx::setText(const std::wstring& text)
	{
		this->text = text;
	}

	void ContainerEx::load(const std::wstring &filename, const Rectangle& srcRect, bool autoScale)
	{
		if (image)
		{
			delete image;
			image = NULL;
		}
		if (filename == L"NONE" || filename == L"")
			image = NULL;
		else
		{
			image = Image::load(filename);
			if (srcRect.width == 0)
				this->srcRect = Rectangle(0, 0, image->getWidth(), image->getHeight());
			else
				this->srcRect = srcRect;

			this->autoScale = autoScale;
		}

		setOpaque(false);
	}

	void ContainerEx::draw(Graphics *graphics)
	{
		if (image)
		{
			graphics->setColor(Color(255,255,255,getAlpha()));
			if (!autoScale)
				((SoraGUIImage*)image)->getSprite()->setScale(1.0, 1.0);
			else
				((SoraGUIImage*)image)->getSprite()->setScale(double(getWidth()) / srcRect.width, double(getHeight()) / srcRect.height);
			graphics->drawImage(image, srcRect.x, srcRect.y, 0, 0, srcRect.width, srcRect.height);
		}

		if (text != L"")
		{
			graphics->setFont(getFont());
			graphics->setColor(Color(255,255,255,getAlpha()));
			graphics->drawTextW(text, (getWidth() - getFont()->getWidthW(text))/2, (getHeight() - getFont()->getHeight())/2);
		}

		gcn::Container::draw(graphics);
	}

	Image* ContainerEx::getImage()
	{
		return image;
	}
}
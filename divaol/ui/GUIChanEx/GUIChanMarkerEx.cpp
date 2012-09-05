#include "GUIChanMarkerEx.h"
#include "soraguiimage.hpp"
#include "SoraSprite.h"

namespace gcn
{

	MarkerEx::MarkerEx()
	{
		image = NULL;
	}

	MarkerEx::~MarkerEx()
	{
	}

	void MarkerEx::setLook(const std::wstring &filename, Rectangle upNormal, Rectangle upHightlight, 
		Rectangle upPressed, Rectangle downNormal, Rectangle downHightlight, 
		Rectangle downPressed, Rectangle midNormal, Rectangle midHightlight, Rectangle midPressed,
		int ox , int oy)
	{
		if (image)
		{
			delete image;
			image = NULL;
		}
		image = Image::load(filename);
		this->upNormal = upNormal;
		this->upHightlight = upHightlight;
		this->upPressed = upPressed;
		this->downNormal = downNormal;
		this->downHightlight = downHightlight;
		this->downPressed = downPressed;
		this->midNormal = midNormal;
		this->midHightlight = midHightlight;
		this->midPressed = midPressed;
		offx = ox;
		offy = oy;
	}

	void MarkerEx::draw(Graphics* graphics)
	{
		Rectangle upRect, downRect, midRect;
		if (!isEnabled())
		{
			upRect = upNormal;
			downRect = downNormal;
			midRect = midNormal;
		}
		else if (!isHasMouse())
		{
			//image->setTextureRect(normalRect);
			upRect = upNormal;
			downRect = downNormal;
			midRect = midNormal;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else if (!isPressed())
		{
			//image->setTextureRect(highlightRect);
			upRect = upHightlight;
			downRect = downHightlight;
			midRect = midHightlight;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else
		{
			upRect = upPressed;
			downRect = downPressed;
			midRect = midPressed;
			//image->setTextureRect(pressedRect);
		}
		sora::SoraSprite* sprite = ((SoraGUIImage*)image)->getSprite();
		graphics->setColor(Color(255,255,255,getAlpha()));
		// Up
		sprite->setScale(1, 1.0);
		graphics->drawImage(image, upRect.x, upRect.y, offx, offy, upRect.width, upRect.height);
		// Down
		sprite->setScale( 1, 1.0);
		graphics->drawImage(image, downRect.x, downRect.y, offx, offy + getHeight() - downRect.height, downRect.width, downRect.height);
		// mid
		sprite->setScale(1, double(getHeight() - upRect.height - downRect.height) / midRect.height);
		graphics->drawImage(image, midRect.x, midRect.y, offx, offy + upRect.height, midRect.width, midRect.height);
	}
}
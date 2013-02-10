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
		
		upX1 = upX2 = upX3 = downX1 = downX2 = downX3 = midX1 = midX2 = midX3 = ox;
		upY1 = upY2 = upY3 = downY1 = downY2 = downY3 = midY1 = midY2 = midY3 = oy;
	}

	void MarkerEx::setOffset(int upX1,int upY1, int upX2, int upY2, int upX3, int upY3, int downX1, 
		int downY1, int downX2, int downY2, int downX3, int downY3, int midX1, int midY1, int midX2, int midY2, int midX3, int midY3)
	{
		this->upX1 = upX1;
		this->upY1 = upY1;
		this->upX2 = upX2;
		this->upY2 = upY2;
		this->upX3 = upX3;
		this->upY3 = upY3;

		this->downX1 = downX1;
		this->downY1 = downY1;
		this->downX2 = downX2;
		this->downY2 = downY2;
		this->downX3 = downX3;
		this->downY3 = downY3;

		this->midX1 = midX1;
		this->midY1 = midY1;
		this->midX2 = midX2;
		this->midY2 = midY2;
		this->midX3 = midX3;
		this->midY3 = midY3;
	}

	void MarkerEx::draw(Graphics* graphics)
	{
		Rectangle upRect, downRect, midRect;
		int upx, upy, downx, downy, midx, midy;
		if (!isEnabled())
		{
			upRect = upNormal;
			downRect = downNormal;
			midRect = midNormal;
			upx = upX1;
			upy = upY1;
			downx = downX1;
			downy = downY1;
			midx = midX1;
			midy = midY1;
		}
		else if (!isHasMouse())
		{
			//image->setTextureRect(normalRect);
			upRect = upNormal;
			downRect = downNormal;
			midRect = midNormal;
			upx = upX1;
			upy = upY1;
			downx = downX1;
			downy = downY1;
			midx = midX1;
			midy = midY1;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else if (!isPressed())
		{
			//image->setTextureRect(highlightRect);
			upRect = upHightlight;
			downRect = downHightlight;
			midRect = midHightlight;
			upx = upX2;
			upy = upY2;
			downx = downX2;
			downy = downY2;
			midx = midX2;
			midy = midY2;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else
		{
			upRect = upPressed;
			downRect = downPressed;
			midRect = midPressed;
			upx = upX3;
			upy = upY3;
			downx = downX3;
			downy = downY3;
			midx = midX3;
			midy = midY3;
			//image->setTextureRect(pressedRect);
		}
		sora::SoraSprite* sprite = ((SoraGUIImage*)image)->getSprite();
		graphics->setColor(Color(255,255,255,getAlpha()));
		// Up
		sprite->setScale(1, 1.0);
		graphics->drawImage(image, upRect.x, upRect.y, upx, upy, upRect.width, upRect.height);
		// Down
		sprite->setScale( 1, 1.0);
		graphics->drawImage(image, downRect.x, downRect.y, downx, downy + getHeight() - downNormal.height, downRect.width, downRect.height);
		// mid
		sprite->setScale(1, double(getHeight() - upNormal.height - downNormal.height) / midRect.height);
		graphics->drawImage(image, midRect.x, midRect.y, midx, midy + upNormal.height, midRect.width, midRect.height);
	}
}
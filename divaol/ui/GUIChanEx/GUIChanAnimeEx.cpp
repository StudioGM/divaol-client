#include "ui/GUIChanEx/GUIChanAnimeEx.h"

namespace gcn
{
	AnimeEx::AnimeEx()
	{
		dx = dy = 0;
		cx = cy = 0;
	}

	AnimeEx::~AnimeEx()
	{
	}

	void AnimeEx::setImagePosition(int dx, int dy)
	{
		this->dx = dx;
		this->dy = dy;
	}

	void AnimeEx::load(const std::wstring &wfilename, const Rectangle& srcRect, int cx /* = 0 */, int cy /* = 0 */)
	{
		ContainerEx::load(wfilename, srcRect, false);
		this->cx = cx;
		this->cy = cy;
		restart();
	}

	void AnimeEx::restart()
	{
	}

	void AnimeEx::update(float dt)
	{
		
	}

	void AnimeEx::draw(Graphics *graphics)
	{
		if (image)
		{
			sora::SoraSprite* sprite = ((SoraGUIImage*)image)->getSprite();
			graphics->setColor(Color(255,255,255,getAlpha()));
			sprite->setCenter(cx, cy);

			graphics->drawImage(image, srcRect.x, srcRect.y, dx + sprite->getCenterX(), dy + sprite->getCenterY(), srcRect.width, srcRect.height);
		}
		gcn::Container::draw(graphics);
	}
}
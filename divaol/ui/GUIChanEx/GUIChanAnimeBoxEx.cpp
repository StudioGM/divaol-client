#include "GUIChanAnimeBoxEx.h"

namespace gcn
{

	AnimeBoxEx::AnimeBoxEx()
	{
		setOpaque(false);
		setBaseColor(Color(255, 255, 255));
		fps = 1;
		time = 0;
		tiles.clear();
	}

	AnimeBoxEx::~AnimeBoxEx()
	{
		for (ImageTiles::iterator i = tiles.begin(); i != tiles.end(); i++)
			delete (*i);
		//tiles.clear();
	}

	void AnimeBoxEx::loadAnime(const ImageTiles& tiles, double fps, int mode)
	{
		this->tiles.clear();
		this->tiles = tiles;
		this->fps = fps;
		this->mode = mode;
	}

	void AnimeBoxEx::draw(Graphics *graphics)
	{
		if (tiles.size() == 0)
			return;
		int index = 0;
		double spf = 1 / fps;
		if (mode == MODE_REPEAT)
			index = (int(time / spf)) % tiles.size();
		else
		{
			if (time < spf * tiles.size())
				index = int(time / spf);
			else
				index = tiles.size() - 1;
		}
		Color c = getBaseColor();
		graphics->setColor(Color(c.r, c.g, c.b, getAlpha()));
		tiles[index]->draw(graphics, 0, 0, getWidth(), getHeight());
	}

	void AnimeBoxEx::restart()
	{
		time = 0;
	}

	void AnimeBoxEx::update(float dt)
	{
		//if (isEnabled())
		time += dt;
	}
}
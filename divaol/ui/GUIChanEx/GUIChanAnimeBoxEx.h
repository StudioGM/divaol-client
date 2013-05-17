#pragma once

#include "GUIChanContainerEx.h"
#include "GUIChanImageTileEx.h"

namespace gcn
{

	//typedef std::vector<std::wstring> AnimeImages;

	

	class AnimeBoxEx :
		public Container
	{
	private:
		ImageTiles tiles;
		double fps;
		int mode;
		double time;

	public:
		enum {MODE_ONCE, MODE_REPEAT};

		AnimeBoxEx();
		virtual ~AnimeBoxEx();

		void restart();
		void update(float dt);
		void loadAnime(const ImageTiles& tiles, double fps, int mode = MODE_ONCE);

		virtual void draw(Graphics *graphics);
	};

}
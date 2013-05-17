#pragma once

#include "ui/GUIChanEx/GUIChanContainerEx.h"

namespace gcn
{
	class AnimeEx : public ContainerEx
	{
	protected:
		int cx, cy;
		int dx, dy;

	public:
		AnimeEx();
		virtual~AnimeEx();

		void setImagePosition(int dx, int dy);

		virtual void load(const std::wstring &filename, const Rectangle& srcRect, int cx = 0, int cy = 0);
		virtual void restart();

		virtual void update(float dt);
		virtual void draw(Graphics *graphics);
	};
}
#pragma once

#include "guichan.hpp"
#include "soraguiimage.hpp"
#include "SoraSprite.h"
#include "SoraCore.h"
#include <string>

namespace gcn
{
	class ContainerEx : public Container
	{
	protected:
		Image* image;
		Rectangle srcRect;
		bool expandBound;
		bool autoScale;

	public:
		ContainerEx();
		~ContainerEx();

		void load(const std::wstring &filename, const Rectangle& srcRect, bool autoScale = false);

		void setExpandBound(bool value);
		bool getExpandBound() const;

		Image* getImage();

		virtual void draw(Graphics *graphics);
	};
}
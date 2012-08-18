#ifndef BoarderedContainer_H
#define BoarderedContainer_H

#include "guichan/widgets/container.hpp"

#include "guichan/exception.hpp"
#include "guichan/graphics.hpp"


namespace gcn
{
	class GCN_CORE_DECLSPEC BoarderedContainer : public gcn::Container
	{
	public:
		virtual void draw(gcn::Graphics* graphics)
		{
			gcn::Container::draw(graphics);

			graphics->setColor(getForegroundColor());
			gcn::Rectangle boarder(0,0,getWidth()-1,getHeight()-1);
			graphics->drawLine(boarder.x,boarder.y,boarder.x + boarder.width, boarder.y);
			graphics->drawLine(boarder.x + boarder.width,boarder.y,boarder.x + boarder.width, boarder.y+boarder.height);
			graphics->drawLine(boarder.x + boarder.width, boarder.y+boarder.height,boarder.x, boarder.y+boarder.height);
			graphics->drawLine(boarder.x, boarder.y+boarder.height, boarder.x,boarder.y);
		}
	};
}



#endif
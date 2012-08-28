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
		virtual void setBoarderSize(int size)
		{
			boarderSize = size;
			if(boarderSize<2) boarderSize=2;
		}

		virtual void draw(gcn::Graphics* graphics)
		{
			gcn::Rectangle backColorRect(boarderSize,boarderSize,getWidth()-boarderSize*2,getHeight()-boarderSize*2);
			graphics->setColor(getBaseColor());
			graphics->fillRectangle(backColorRect);

			gcn::Rectangle boarderRect;
			gcn::Color boarderColor = getForegroundColor();
			graphics->setColor(boarderColor);

			boarderRect = gcn::Rectangle(0,2,boarderSize,getHeight()-4);
			graphics->fillRectangle(boarderRect);

			boarderRect = gcn::Rectangle(2,0,getWidth()-4,boarderSize);
			graphics->fillRectangle(boarderRect);

			boarderRect = gcn::Rectangle(getWidth()-boarderSize,2,boarderSize,getHeight()-4);
			graphics->fillRectangle(boarderRect);

			boarderRect = gcn::Rectangle(2,getHeight()-boarderSize,getWidth()-4,boarderSize);
			graphics->fillRectangle(boarderRect);

			graphics->drawPoint(1,2);
			graphics->drawPoint(getWidth()-2,2);
			graphics->drawPoint(1,getHeight()-1);
			graphics->drawPoint(getWidth()-2,getHeight()-1);
			
			boarderColor.a/=2;
			graphics->setColor(boarderColor);
			graphics->drawPoint(0,2);
			graphics->drawPoint(1,1);
			graphics->drawPoint(getWidth()-1,2);
			graphics->drawPoint(getWidth()-2,1);
			graphics->drawPoint(1,getHeight());
			graphics->drawPoint(0,getHeight()-1);
			graphics->drawPoint(getWidth()-1,getHeight()-1);
			graphics->drawPoint(getWidth()-2,getHeight());

			drawChildren(graphics);
			
		}


	protected:
		int boarderSize;
	};
}



#endif
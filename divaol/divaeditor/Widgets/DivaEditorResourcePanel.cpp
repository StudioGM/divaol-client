
#include<cmath>

#include "divaeditor/Widgets/DivaEditorResourcePanel.h"
#include "divaeditor/DivaEditorCommon.h"

#include "divacore/Core/DivaCore.h"
#include "divacore/Component/DivaStandardCoreFlow.h"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouseinput.hpp"

namespace divaeditor
{

#define MainSceneImageFile L"standard/pic/editor_001.png"

	ResourcePanel::ResourcePanel()
	{
		_selectedIndex=-1;
		_elementGridSize=-1;
		_gridPerPage=-1;

		addMouseListener(this);
		addKeyListener(this);

		setFocusable(true);

		image = gcn::Image::load(MainSceneImageFile);
	}

	void ResourcePanel::adjustSize()
	{
		int width = getWidth(), height = getHeight();

		_gridPerWidth = width/(_elementGridSize+gridOffset*2);
		_gridPerHeight = height/(_elementGridSize+gridOffset+gridTextHeight);
		_gridPerPage = _gridPerWidth*_gridPerHeight;

		if(_gridPerPage<=0) return;

		setWidth(_gridPerWidth*(_elementGridSize+gridOffset*2));
		setHeight(_gridPerHeight*(_elementGridSize+gridOffset+gridTextHeight));

		_nowPage=0;
	}

	void ResourcePanel::setSelectedIndex(int index)
	{
		if(index>=0&&index<EDITOR_PTR->mapData->coreInfoPtr->resources.size())
		{
			_selectedIndex = index;
			distributeValueChangedEvent();
		}
		else
		{
			_selectedIndex = -1;
			distributeValueChangedEvent();
		}
	}

	int ResourcePanel::getSelectedIndex()
	{
		//Check if is out of boarder
		if(_elementGridSize!=-1)
			if(_selectedIndex>=EDITOR_PTR->mapData->coreInfoPtr->resources.size())
				_selectedIndex = -1;

		return _selectedIndex;
	}

	void ResourcePanel::setElementGridSize(int gridSize)
	{
		_elementGridSize = gridSize;
	}


	void ResourcePanel::nextPage()
	{
		int totalPage = (EDITOR_PTR->mapData->coreInfoPtr->resources.size()-1)/_gridPerPage;
		if(_nowPage<totalPage)
			_nowPage++;
		else
			_nowPage=0;
	}

	void ResourcePanel::prevPage()
	{
		int totalPage = (EDITOR_PTR->mapData->coreInfoPtr->resources.size()-1)/_gridPerPage;
		if(_nowPage<=0)
			_nowPage=totalPage;
		else
			_nowPage--;
	}

	void ResourcePanel::logic()
	{
		if(_elementGridSize!=-1)
			if(_selectedIndex>=EDITOR_PTR->mapData->coreInfoPtr->resources.size())
				setSelectedIndex(-1);
	}

	void ResourcePanel::draw(gcn::Graphics* graphics)
	{
		gcn::Color baseColor = getBaseColor();
		gcn::Color selectionColor = gcn::Color(255-baseColor.r,255-baseColor.g,255-baseColor.b,255);
		gcn::Color textColor = getForegroundColor();
		textColor.a = 150;
		gcn::Color selectionTextColor = gcn::Color(textColor.r,textColor.g,textColor.b,255);

		float width = getWidth(),height = getHeight();

		const int boarderSize=3;

		gcn::Rectangle backColorRect(boarderSize,boarderSize,getWidth()-boarderSize*2,getHeight()-boarderSize*2);
		graphics->setColor(baseColor);
		graphics->fillRectangle(backColorRect);

		gcn::Rectangle boarderRect;
		gcn::Color boarderColor = gcn::Color(53,53,53,150);
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
		


		if(_gridPerPage<=0) return;

		const int gridWidth = _elementGridSize+gridOffset*2,
					gridHeight = _elementGridSize+gridOffset+gridTextHeight;

		//Draw Resource Previews
		for (int nowY=0;nowY<_gridPerHeight;nowY++)
		{
			for (int nowX=0;nowX<_gridPerWidth;nowX++)
			{
				int thisIndex = nowY * _gridPerHeight + nowX;
				if(thisIndex>=0&&thisIndex< EDITOR_PTR->mapData->coreInfoPtr->resources.size())
				{
					//Draw Selected Area
					graphics->setColor(gcn::Color(255,255,255,255));
					if(thisIndex==_selectedIndex)
					{
						graphics->drawImage(image,0,0,nowX*gridWidth+gridOffset-5,nowY*gridHeight+gridOffset-5,138,145);
					}

					//Draw Resource Preview
					divacore::MapResourceInfo &thisResource = EDITOR_PTR->mapData->coreInfoPtr->resources[EDITOR_PTR->mapData->findResourceIDByIndex(thisIndex)];

					if(thisResource.type == divacore::MapResourceInfo::VIDEO)
					{
						graphics->drawImage(image,266,0,nowX*gridWidth+gridOffset,nowY*gridHeight+gridOffset,_elementGridSize,_elementGridSize);
					}
					else if(thisResource.type == divacore::MapResourceInfo::AUDIO)
					{
						graphics->drawImage(image,138,0,nowX*gridWidth+gridOffset,nowY*gridHeight+gridOffset,_elementGridSize,_elementGridSize);
					}
					else if(thisResource.type == divacore::MapResourceInfo::IMAGE)
					{
						graphics->drawImage(image,0,145,nowX*gridWidth+gridOffset,nowY*gridHeight+gridOffset,_elementGridSize,_elementGridSize);
					}

					//Draw Resource Name
					graphics->setColor((thisIndex==_selectedIndex)?selectionTextColor:textColor);
					graphics->setFont(getFont());
					std::wstring thisDescription = EDITOR_PTR->mapData->getResourceDescriptionByIndex(thisIndex);
					int drawPos = 0;
					for(;drawPos!=thisDescription.length() && getFont()->getWidthW(thisDescription.substr(0,drawPos))<=_elementGridSize;drawPos++)
					{}
					
					graphics->drawTextW(thisDescription.substr(0,drawPos==thisDescription.length()?(drawPos):(drawPos-1)),
						nowX*gridWidth+gridOffset,nowY*gridHeight+gridOffset+_elementGridSize);
				}

			}
		}
	}

	void ResourcePanel::mousePressed(gcn::MouseEvent& mouseEvent)
	{
		int pressedX = mouseEvent.getX()/(gridOffset*2+_elementGridSize),pressedY = mouseEvent.getY()/(gridOffset+_elementGridSize+gridTextHeight);
		int toSelect = pressedX+pressedY*_gridPerWidth+_nowPage*_gridPerPage;
		setSelectedIndex(toSelect);
	}

	void ResourcePanel::addSelectionListener(gcn::SelectionListener* selectionListener)
	{
		mSelectionListeners.push_back(selectionListener);
	}

	void ResourcePanel::removeSelectionListener(gcn::SelectionListener* selectionListener)
	{
		mSelectionListeners.remove(selectionListener);
	}

	void ResourcePanel::distributeValueChangedEvent()
	{
		SelectionListenerIterator iter;

		for (iter = mSelectionListeners.begin(); iter != mSelectionListeners.end(); ++iter)
		{
			SelectionEvent event(this);
			(*iter)->valueChanged(event);
		}
	}

}
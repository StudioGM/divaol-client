
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
	ResourcePanel::ResourcePanel()
	{
		_selectedIndex=-1;
		_elementGridSize=-1;
		_gridPerPage=-1;

		addMouseListener(this);
		addKeyListener(this);

		setFocusable(true);
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
		gcn::Color selectionTextColor = gcn::Color(255-textColor.r,255-textColor.g,255-textColor.b,textColor.a);

		float width = getWidth(),height = getHeight();

		//Draw background
		graphics->setColor(baseColor);
		graphics->fillRectangle(gcn::Rectangle(0,0,width,height));

		//Draw boarder line
		graphics->setColor(textColor);
		gcn::Rectangle boarder(0,0,width-1,height-1);
		graphics->drawLine(boarder.x,boarder.y,boarder.x + boarder.width, boarder.y);
		graphics->drawLine(boarder.x + boarder.width,boarder.y,boarder.x + boarder.width, boarder.y+boarder.height);
		graphics->drawLine(boarder.x + boarder.width, boarder.y+boarder.height,boarder.x, boarder.y+boarder.height);
		graphics->drawLine(boarder.x, boarder.y+boarder.height, boarder.x,boarder.y);
		


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
					if(thisIndex==_selectedIndex)
					{
						graphics->setColor(selectionColor);
						graphics->fillRectangle(gcn::Rectangle(nowX*gridWidth, nowY*gridHeight,gridWidth,gridHeight));
					}

					//Draw Resource Preview
					graphics->setColor(textColor);
					gcn::Rectangle previewGrid(nowX*gridWidth+gridOffset,nowY*gridHeight+gridOffset,_elementGridSize-1,_elementGridSize-1);
					graphics->drawLine(previewGrid.x,previewGrid.y,previewGrid.x + previewGrid.width, previewGrid.y);
					graphics->drawLine(previewGrid.x + previewGrid.width,previewGrid.y,previewGrid.x + previewGrid.width, previewGrid.y+previewGrid.height);
					graphics->drawLine(previewGrid.x + previewGrid.width, previewGrid.y+previewGrid.height,previewGrid.x, previewGrid.y+previewGrid.height);
					graphics->drawLine(previewGrid.x, previewGrid.y+previewGrid.height, previewGrid.x,previewGrid.y);


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
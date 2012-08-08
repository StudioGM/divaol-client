#include "divaeditor/DivaEditorScene/DivaEditorNoteArea.h"
#include "divaeditor/DivaEditorMapData.h"
#include "divaeditor/DivaEditorCommon.h"

#include "divacore/Core/DivaCore.h"
#include "divacore/Component/DivaStandardCoreFlow.h"

#include "guichan/widgets/button.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouseinput.hpp"

#include<cmath>

namespace divaeditor
{


#define NOTEAREA_DEBUG

	NoteArea::NoteArea(): isSelecting(false),isMouseOn(false)
	{
		nowMousePos.x=-1;
		nowMousePos.y=-1;
		
		setFocusable(true);
		setFrameSize(1);

		addMouseListener(this);
		addKeyListener(this);
		addFocusListener(this);

		setBaseColor(gcn::Color(0,0,0,0));
	}

	void NoteArea::adjustSize(float fact)
	{
		factor = fact;
		float width = (EDITCONFIG->NoteAreaWidth+EDITCONFIG->NoteAreaTailAreaSize*2+1)*EDITCONFIG->NoteAreaGridSize;
		float height = (EDITCONFIG->NoteAreaHeight+EDITCONFIG->NoteAreaTailAreaSize*2+1)*EDITCONFIG->NoteAreaGridSize;

		setWidth(width*factor);
		setHeight(height*factor);

		setPosition(getX()-(EDITCONFIG->NoteAreaGridSize*factor*0.5)+2,getY()-(EDITCONFIG->NoteAreaGridSize*factor*0.5)+2);
	}

	void NoteArea::draw(gcn::Graphics* graphics)
	{

		gcn::Color normalLineColor(255,255,255,255);

		float width = getWidth(),height = getHeight();

#ifdef NOTEAREA_DEBUG

		//Draw Boarder
		graphics->setColor(normalLineColor);
		graphics->drawLine(0,0,0,height-1);
		graphics->drawLine(0,height-1,width-1,height-1);
		graphics->drawLine(0,0,width-1,0);
		graphics->drawLine(width-1,0,width-1,height-1);


		if(nowGridSelectX!=-1 && nowGridSelectY!=-1)
		{
			graphics->setColor(normalLineColor);
			graphics->fillRectangle(gcn::Rectangle(float(nowGridSelectX*EDITCONFIG->NoteAreaGridSize)*factor,
													float(nowGridSelectY*EDITCONFIG->NoteAreaGridSize)*factor,
													float(EDITCONFIG->NoteAreaGridSize)*factor,
													float(EDITCONFIG->NoteAreaGridSize)*factor));
		}

#endif



	}

	void NoteArea::logic()
	{
		float width = getWidth(),height = getHeight();
		if(nowMousePos.x>=0&&nowMousePos.x<width)
		{
			nowGridSelectX = nowMousePos.x/(width)*float(EDITCONFIG->NoteAreaWidth+EDITCONFIG->NoteAreaTailAreaSize*2+1);
		}
		else
			nowGridSelectX=-1;

		if(nowMousePos.y>=0&&nowMousePos.y<height)
		{
			nowGridSelectY = nowMousePos.y/(height)*float(EDITCONFIG->NoteAreaHeight+EDITCONFIG->NoteAreaTailAreaSize*2+1);
		}
		else
			nowGridSelectY=-1;
		
	}

	void NoteArea::mousePressed(gcn::MouseEvent& mouseEvent)
	{

	}

	void NoteArea::mouseReleased(gcn::MouseEvent& mouseEvent)
	{

	}

	void NoteArea::mouseDragged(gcn::MouseEvent& mouseEvent)
	{

	}

	void NoteArea::keyPressed(gcn::KeyEvent& keyEvent)
	{
		//if(keyEvent.
	}


	void NoteArea::keyReleased(gcn::KeyEvent& keyEvent)
	{

	}


	void NoteArea::mouseEntered(gcn::MouseEvent& mouseEvent)
	{
		isMouseOn=true;
	}

	void NoteArea::mouseExited(gcn::MouseEvent& mouseEvent)
	{
		isMouseOn=false;
	}

	void NoteArea::mouseMoved(MouseEvent& mouseEvent)
	{
		nowMousePos.x = mouseEvent.getX();
		nowMousePos.y=mouseEvent.getY();
	}

}
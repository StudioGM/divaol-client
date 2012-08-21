#ifndef DIVAEDITORNOTEAREA_H
#define DIVAEDITORNOTEAREA_H

#include <string>

#include "divaeditor/Core/DivaEditorMapData.h"

#include "Hoshizora.h"

#include "guichan/focuslistener.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"




namespace divaeditor
{

	class GCN_CORE_DECLSPEC NoteArea : public gcn::Widget,
		public gcn::MouseListener,
		public gcn::KeyListener,
		public gcn::FocusListener
	{
	private:
		void changeSelectedNoteTailOver();
		void placingLongNoteOver();
		void placingComboNoteOver();
		void updateMousePos();

	public:
		NoteArea();


		void adjustSize(float fact);

		//Inherited from Widget
		virtual void draw(gcn::Graphics* graphics);
		virtual void logic();
		

		// Inherited from FocusListener
		virtual void focusLost(const gcn::Event& event){}


		// Inherited from MouseListener
		virtual void mousePressed(gcn::MouseEvent& mouseEvent);
		virtual void mouseReleased(gcn::MouseEvent& mouseEvent);
		virtual void mouseEntered(gcn::MouseEvent& mouseEvent);
		virtual void mouseExited(gcn::MouseEvent& mouseEvent);
		virtual void mouseDragged(gcn::MouseEvent& mouseEvent);
		virtual void mouseMoved(gcn::MouseEvent& mouseEvent);


		// Inherited from KeyListener
		//virtual void keyPressed(gcn::KeyEvent& keyEvent){}
		//virtual void keyReleased(gcn::KeyEvent& keyEvent){}

		//Receive Sora KeyEvent
		void onKeyPressed(sora::SoraKeyEvent& event);
		void onKeyReleased(sora::SoraKeyEvent& event);

	protected:

		float factor;

		int nowGridSelectX,nowGridSelectY;

		bool isMouseOn;

		int notePlacedPos;
		
		divacore::Point nowMousePos;

		bool moveTail;
		int moveTailX,moveTailY;

		bool placingLong;
		bool placingCombo;
		divacore::MapNote placingNote;


		//Select Area
		bool isSelecting;
		int selectBeginX,selectBeginY;
		int selectBeginGridX,selectBeginGridY;


		//Drag notes
		bool isDraging;
		gcn::Rectangle dragAreaRect;
		int dragingPosX,dragingPosY;

		//Drag note time position
		int pressedTimePos;
		bool isDragingTime;

		bool isFristDrag;
	};




}







#endif
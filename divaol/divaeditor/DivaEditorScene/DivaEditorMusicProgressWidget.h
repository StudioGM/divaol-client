#ifndef DivaEditorMusicProgressWidget_H
#define DivaEditorMusicProgressWidget_H


#include <string>

#include "Hoshizora.h"

#include "guichan/focuslistener.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

#include "divaeditor/DivaEditorMapData.h"


namespace divaeditor
{



	class GCN_CORE_DECLSPEC DivaEditorMusicProgressWidget : 
		public gcn::Widget,
		public gcn::MouseListener,
		public gcn::KeyListener,
		public gcn::FocusListener
	{
	public:
		DivaEditorMusicProgressWidget();

		//Inherited from Widget
		virtual void draw(gcn::Graphics* graphics);

		// Inherited from FocusListener
		virtual void focusLost(const gcn::Event& event){}


		// Inherited from MouseListener
		virtual void mousePressed(gcn::MouseEvent& mouseEvent);
		virtual void mouseReleased(gcn::MouseEvent& mouseEvent);
		virtual void mouseEntered(gcn::MouseEvent& mouseEvent){}
		virtual void mouseExited(gcn::MouseEvent& mouseEvent){}
		virtual void mouseDragged(gcn::MouseEvent& mouseEvent);


		// Inherited from KeyListener
		virtual void keyPressed(gcn::KeyEvent& keyEvent){}
		virtual void keyReleased(gcn::KeyEvent& keyEvent){}
		

	protected:
		int nowMouseXPos;
		bool mouseDown;
		
	};













}







#endif
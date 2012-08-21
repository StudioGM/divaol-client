#ifndef DivaEditorResourcePanel_H
#define DivaEditorResourcePanel_H

#include <string>

#include "divaeditor/Core/DivaEditorMapData.h"

#include "guichan/focuslistener.hpp"
#include "guichan/graphics.hpp"
#include "guichan/keylistener.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouselistener.hpp"
#include "guichan/platform.hpp"
#include "guichan/widget.hpp"

#include "Hoshizora.h"
#include "soraguichan/SoraGUI.h"
#include "Soraguichan/SoraGUIResponserMap.h"

namespace divaeditor
{
	class SelectionListener;


	class GCN_CORE_DECLSPEC ResourcePanel : 
		public gcn::Widget,
		public gcn::MouseListener,
		public gcn::KeyListener,
		public gcn::FocusListener
	{
	public:
		ResourcePanel();

		//Inherited from Widget
		virtual void draw(gcn::Graphics* graphics);
		virtual void logic();

		// Inherited from FocusListener
		virtual void focusLost(const gcn::Event& event){}


		// Inherited from MouseListener
		virtual void mousePressed(gcn::MouseEvent& mouseEvent);

		// Inherited from KeyListener
		virtual void keyPressed(gcn::KeyEvent& keyEvent){}

		//Public functions
		void adjustSize();

		void nextPage();
		void prevPage();

		void setSelectedIndex(int index);
		void setElementGridSize(int gridSize);
		
		int getSelectedIndex();

		//Listener functions
		void addSelectionListener(gcn::SelectionListener* selectionListener);
		void removeSelectionListener(gcn::SelectionListener* selectionListener);
		void distributeValueChangedEvent();

		int _gridPerWidth,_gridPerHeight,_gridPerPage,_nowPage;

	protected:

		static const int gridOffset = 5;
		static const int gridTextHeight = 20;

		int _selectedIndex;
		int _elementGridSize;

		

		typedef std::list<gcn::SelectionListener*> SelectionListenerList;
		SelectionListenerList mSelectionListeners;

		typedef SelectionListenerList::iterator SelectionListenerIterator;

	};
}



#endif
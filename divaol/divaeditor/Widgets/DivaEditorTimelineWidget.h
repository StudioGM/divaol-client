#ifndef DivaEditorTimelineWidget
#define DivaEditorTimelineWidget

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

namespace divaeditor
{

	

    class GCN_CORE_DECLSPEC Timeline : public gcn::Widget,
                                     public gcn::MouseListener,
                                     public gcn::KeyListener,
                                     public gcn::FocusListener
    {
    public:
        Timeline();

        //Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);

        // Inherited from FocusListener
        virtual void focusLost(const gcn::Event& event){}


        // Inherited from MouseListener
        virtual void mousePressed(gcn::MouseEvent& mouseEvent);
        virtual void mouseReleased(gcn::MouseEvent& mouseEvent);
        virtual void mouseEntered(gcn::MouseEvent& mouseEvent){isMouseOn=true;}
        virtual void mouseExited(gcn::MouseEvent& mouseEvent){isMouseOn=false;}
        virtual void mouseDragged(gcn::MouseEvent& mouseEvent);
		virtual void mouseMoved(gcn::MouseEvent& mouseEvent);

		void onKeyPressed(sora::SoraKeyEvent& event);
		void onKeyReleased(sora::SoraKeyEvent& event);

		//Public functions
		void setMaxGridHeightFactor(float factor);


		//void registerSelectedChangedFunc(sora::SoraFunction<void(const PointF&)> &func);

    protected:

		int findNoteByMousePos(int x,int y);

		float _selectedBegin,_selectedEnd;
		float _maxGridHeightFactor;
		bool isSelecting,isDraggingNote;

		int nowMouseXPos,nowMouseYPos;

		int lastDragPos;
		bool lockDragPos;
		int lastDragType;
		bool lockDragType;
		bool firstDrag;

		bool isMouseOn;

		bool placingLong;
		bool placingCombo;

		bool dragingLongPos;
		int dragingLongPosIndex;

		int _placeNoteBeginPos;
		int _placeNoteType;
		//divacore::MapNote placingNote;

    };
}



#endif
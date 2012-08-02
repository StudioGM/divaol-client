#ifndef DivaEditorTimelineWidget
#define DivaEditorTimelineWidget

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

	static const int gridToShowPerBeatTable[6] = {1,2,3,4,6,8};
	static const int gridToShowPerBeatTableCount = 6;

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
        virtual void mouseEntered(gcn::MouseEvent& mouseEvent){}
        virtual void mouseExited(gcn::MouseEvent& mouseEvent){}
        virtual void mouseDragged(gcn::MouseEvent& mouseEvent);


        // Inherited from KeyListener
        virtual void keyPressed(gcn::KeyEvent& keyEvent){}
        virtual void keyReleased(gcn::KeyEvent& keyEvent){}
        //virtual bool isHasMouse() const{return true;}

		//Public functions
		void setBarNumberPerScreen(int barNumberPerScreen){_beatNumberPerScreen=barNumberPerScreen;}
		const int getBarNumberPerScreen() const{return _beatNumberPerScreen;}

		void increaseGridToShowPerBeat(){if(_gridToShowPerBeat<gridToShowPerBeatTableCount-1)_gridToShowPerBeat++;}
		void decreaseGridToShowPerBeat(){if(_gridToShowPerBeat>0)_gridToShowPerBeat--;}
		void setGridToShowPerBeat(int gridToShowPerBeat){_gridToShowPerBeat=gridToShowPerBeat;}
		const int getGridToShowPerBeat() const{return _gridToShowPerBeat;}

		void increaseShowRangeScale(){if(_showRangeFactor<0.1)_showRangeFactor*=2.0f;else if(_showRangeFactor<2)_showRangeFactor+=0.1;else if(_showRangeFactor<5)_showRangeFactor+=0.25;}
		void decreaseShowRangeScale(){if(_showRangeFactor>2)_showRangeFactor-=0.25;else if(_showRangeFactor>0.1)_showRangeFactor-=0.1;else if(_showRangeFactor>0.05)_showRangeFactor/=2.0f;}
		const float getShowRangeScale() const{return _showRangeFactor;}

		//void registerSelectedChangedFunc(sora::SoraFunction<void(const PointF&)> &func);

    protected:
		static const int GridPerBeat = 48;

		int _beatNumberPerScreen;
		int _gridToShowPerBeat;
		float _showRangeFactor;
		//sora::SoraFunction<void(float,float)> _selectedChanged;

		float _selectedBegin,_selectedEnd;
		bool isSelecting;

		int nowMouseXPos;

		

    };
}



#endif
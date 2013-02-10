#ifndef SORA_GUI_RESPONSER_H
#define SORA_GUI_RESPONSER_H

#include <guichan.hpp>

#include "SoraPlatform.h"
#include "SoraException.h"

namespace sora {

	class SoraGUIResponser {
		friend class SoraGUI;
	public:
		// gcn::ActionEvent
		virtual void action() { }
		
		// gcn::KeyEvent
		virtual void keyDown(const gcn::KeyEvent& key) { }
		virtual void keyUp(const gcn::KeyEvent& key) { }

		// gcn::FocusEvent
		virtual void focusGained() { }
		virtual void focusLost() { }

		// gcn::DeathEvent
		virtual void death() { }

		// gcn::MouseEvent
		virtual void mousePressed(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseReleased(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseClicked(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseMoved(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseEntered(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseExited(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseWheelMovedDown(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseWheelMovedUp(const gcn::MouseEvent& mouseEvent) { }
		virtual void mouseDragged(const gcn::MouseEvent& mouseEvent) { }
		
		virtual void valueChanged() {}

		SoraString getID() const { return widgetID; }
		gcn::Widget* getSource() const { return source; }

		void setID(const SoraString& id) { widgetID = id; }
		void setSource(gcn::Widget* s) { source = s; }
	private:
		SoraString widgetID;
		gcn::Widget* source;
	};
    
    class SoraGUIResponserExtern: public SoraGUIResponser {
    public:
      //  SoraString getHandleSrc() const { return id2str(SoraGUIResponserMap::Instance()->getHandleSrcFromWidgetHandle((SoraHandle)getSource())); }

    };

	/*class SoraLuaGUIResponser: public SoraGUIResponser {
	public:
		SoraLuaGUIResponser(const SoraString& luaScript);

		// gcn::ActionEvent
		void action();
		
		// gcn::KeyEvent
		void keyDown(gcn::KeyEvent& key);
		void keyUp(gcn::KeyEvent& key);

		// gcn::FocusEvent
		void focusGain();
		void focusLost();

		// gcn::DeathEvent
		void death();

		// gcn::MouseEvent
		void mousePressed(gcn::MouseEvent& mouseEvent);
		void mouseReleased(gcn::MouseEvent& mouseEvent);
		void mouseClicked(gcn::MouseEvent& mouseEvent);
		void mouseMoved(gcn::MouseEvent& mouseEvent);
		void mouseEntered(gcn::MouseEvent& mouseEvent);
		void mouseExited(gcn::MouseEvent& mouseEvent);
		void mouseWheelMovedDown(gcn::MouseEvent& mouseEvent);

	private:
		// LuaObject* pObj;
	};*/
} // namespace sora

#endif
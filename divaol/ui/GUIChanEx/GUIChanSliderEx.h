#pragma once

#include "guichan.hpp"
#include "SoraSprite.h"
#include "SoraCore.h"
#include "ui/GUIChanEx/GUIChanButtonEx.h"
#include "GUIChanMarkerEx.h"
#include <string>

namespace gcn
{
	class SliderEx :
		public Container,
		public MouseListener,
		public KeyListener
	{
	private:
		ButtonEx* upButton;
		ButtonEx* downButton;
		ButtonEx* markButton;
		int markStart, markEnd;
		int markPosition;
		int markerMinHeight;
		int total;

		virtual void adjustMarkerHeight();

	public:
		SliderEx();
		virtual ~SliderEx();
		
		void setLook(ButtonEx* up, ButtonEx* down, ButtonEx* mark);
		void setMarkScale(int start, int end, int total);
		void setMarkPosition(int v);
		void setMarkerUp();
		void setMarkerDown();

		int getMarkPosition() const;


		virtual void mouseDragged(MouseEvent& mouseEvent);

		virtual void logic();

		virtual void markerPositionChanged(int v);

		virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

		virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

		//virtual void mouseMoved(MouseEvent& mouseEvent);

		//virtual void mouseEntered(MouseEvent& mouseEvent);

		//virtual void mouseExited(MouseEvent& mouseEvent);

		//virtual void mousePressed(MouseEvent& mouseEvent);

		//virtual void mouseReleased(MouseEvent& mouseEvent);

		//virtual void mouseClicked(MouseEvent& mouseEvent);

	};

	class SliderEx_UpButton_MouseListener :
		public MouseListener
	{
	public:
		virtual void mouseClicked(MouseEvent& mouseEvent);
	};

	class SliderEx_DownButton_MouseListener :
		public MouseListener
	{
	public:
		virtual void mouseClicked(MouseEvent& mouseEvent);
	};
}
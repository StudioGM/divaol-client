#pragma once

#include "guichan.hpp"
#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "SoraSprite.h"
#include "SoraCore.h"
#include "ui/GUIChanEx/GUIChanButtonEx.h"
#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "DivaSongListItem.h"
#include <string>
#include <vector>

namespace diva
{
	namespace MusicUI
	{

	/*
	 * an expended vision of gcn::ListBox.
	 * It supports that every item has different renderer.
	 */
	using namespace gcn;

	class DivaSongList :
		public ListBoxEx
	{
	private:
		std::vector<ButtonEx*> leftButtons;
		std::vector<ButtonEx*> rightButtons;
		gcn::Rectangle leftNormal, leftHighlight, leftPressed;
		gcn::Rectangle rightNormal, rightHighlight, rightPressed;
		std::wstring filename;
		
		bool difButtonDisplayed;

		int oxl, oyl;
		int oxr, oyr;

		void adjustButtons();
		void clearButtons();

	public:
		DivaSongList();
		~DivaSongList();

		void loadButtonImage(std::wstring filename, gcn::Rectangle leftNormal,gcn::Rectangle leftHighlight,gcn::Rectangle leftPressed,
			int oxl, int oyl,
			gcn::Rectangle rightNormal,gcn::Rectangle rightHighlight,gcn::Rectangle rightPressed,
			int oxr, int oyr);

		void DifNext(int ind);
		void DifLast(int ind);

		void setDifButtonDisplayed(bool v);

		virtual void logic();

		virtual void itemClicked(int itemIndex);

		virtual void itemChanged();

		virtual void mouseMoved(MouseEvent& mouseEvent);

		virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

		virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

		virtual void firstIndexChanged(int v);

		virtual void highlightItemChanged(int index);

		//virtual void mouseDragged(MouseEvent& mouseEvent);
	};

	class DivaSongList_Clicked : public gcn::MouseListener
	{
	public:
		virtual void mouseClicked(MouseEvent& mouseEvent);
	};
	}
}
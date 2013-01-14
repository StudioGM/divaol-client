#include "DivaSongList.h"
#include "MusicUI.h"

namespace diva
{
	namespace MusicUI
	{

	using namespace gcn;

	DivaSongList::DivaSongList()
	{
		difButtonDisplayed = true;
	}

	DivaSongList::~DivaSongList()
	{
		//clearButtons();
	}

	void DivaSongList::clearButtons()
	{
		for (int i=0; i<leftButtons.size(); i++)
		{
			remove(leftButtons[i]);
			delete leftButtons[i];
			remove(rightButtons[i]);
			delete rightButtons[i];
		}
		leftButtons.clear();
		rightButtons.clear();
	}

	void DivaSongList::setDifButtonDisplayed(bool v)
	{
		difButtonDisplayed = v;
	}

	void DivaSongList::adjustButtons()
	{
		if (filename == L"")
			return;
		if (leftButtons.size() != maxItem)
		{
			clearButtons();
			for (int i=0; i<maxItem; i++)
			{
				ButtonEx* b = new ButtonEx();
				b->setLook(filename, leftNormal, leftHighlight, leftPressed, leftNormal, oxl, oyl);
				b->setSize(leftNormal.width + oxl * 2, leftNormal.height + oyl * 2);
				b->userData = (void*) (i*2);
				add(b, 
					firstRect.x + 10 - oxl,
					firstRect.y + (firstRect.height + itemGap) * i + 95 - oyl);
				b->addMouseListener(new DivaSongList_Clicked());
				leftButtons.push_back(b);

				b = new ButtonEx();
				b->setLook(filename, rightNormal, rightHighlight, rightPressed, rightNormal, oxr, oyr);
				b->setSize(rightNormal.width + oxr * 2, rightNormal.height + oyr * 2);
				b->userData = (void*) (i*2+1);
				add(b, 
					firstRect.x + 130 - oxr,
					firstRect.y + (firstRect.height + itemGap) * i + 95 - oyr);
				b->addMouseListener(new DivaSongList_Clicked());
				rightButtons.push_back(b);
			}
		}
	}

	void DivaSongList::loadButtonImage(std::wstring filename, gcn::Rectangle leftNormal,
		gcn::Rectangle leftHighlight,gcn::Rectangle leftPressed, int oxl, int oyl,
		gcn::Rectangle rightNormal,gcn::Rectangle rightHighlight,gcn::Rectangle rightPressed,
		int oxr, int oyr)
	{
		this->filename = filename;
		this->leftNormal = leftNormal;
		this->leftHighlight = leftHighlight;
		this->leftPressed = leftPressed;
		this->rightNormal = rightNormal;
		this->rightHighlight = rightHighlight;
		this->rightPressed = rightPressed;
		this->oxl = oxl;
		this->oyl = oyl;
		this->oxr = oxr;
		this->oyr = oyr;
		clearButtons();
		adjustButtons();
	}

	void DivaSongList::logic()
	{
		adjustButtons();
		int end = getDisplayedItems();
		for (int i=0; i<end; i++)
		{
			leftButtons[i]->setVisible(difButtonDisplayed);
			rightButtons[i]->setVisible(difButtonDisplayed);
		}
		for (int i=end; i<maxItem; i++)
		{
			leftButtons[i]->setVisible(false);
			rightButtons[i]->setVisible(false);
		}
	}

	void DivaSongList::mouseMoved(MouseEvent& mouseEvent)
	{
		int ori = highlightItemIndex;

		int x = mouseEvent.getX(), y = mouseEvent.getY();
		y -= firstRect.y;
		x -= firstRect.x;
		if (x > firstRect.width || x < 0)
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}
		if (y < 0)
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}
		int ind = y / (firstRect.height + itemGap);
		y %= firstRect.height + itemGap;
		if (y > firstRect.height)
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}
		if (ind >= getDisplayedItems())
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}
		if (difButtonDisplayed)
		{
			if ( leftButtons[ind]->getDimension().isPointInRect(mouseEvent.getX(), mouseEvent.getY()) )
			{
				highlightItemIndex = -1;
				if (ori != highlightItemIndex)
					highlightItemChanged(highlightItemIndex);
				return;
			}
			if ( rightButtons[ind]->getDimension().isPointInRect(mouseEvent.getX(), mouseEvent.getY()) )
			{
				highlightItemIndex = -1;
				if (ori != highlightItemIndex)
					highlightItemChanged(highlightItemIndex);
				return;
			}
		}
		highlightItemIndex = ind + firstIndex;
		if (ori != highlightItemIndex)
			highlightItemChanged(highlightItemIndex);
	}

	void DivaSongList::DifNext(int ind)
	{
		((SongListItem*)items[firstIndex + ind])->nextDif();
		if (selectedItemIndex == ind)
			MusicUI::Instance()->refreshRankingList(true, true);
	}

	void DivaSongList::DifLast(int ind)
	{
		((SongListItem*)items[firstIndex + ind])->lastDif();
		if (selectedItemIndex == ind)
			MusicUI::Instance()->refreshRankingList(true, true);
	}

	void DivaSongList::mouseWheelMovedUp(MouseEvent& mouseEvent)
	{
		if (getFirstIndex() > 0)
			setFirstIndex(getFirstIndex() - 1);
	}

	void DivaSongList::mouseWheelMovedDown(MouseEvent& mouseEvent)
	{
		if (items.size() <= maxItem)
			return;
		if (getFirstIndex() < items.size() - maxItem)
			setFirstIndex(getFirstIndex() + 1);
	}

	void DivaSongList::itemClickedSelectedMode(int itemIndex, int lastSelectedIndex)
	{
		MusicUI* ui = MusicUI::Instance();
		if (lastSelectedIndex == selectedItemIndex)
			ui->SongListItemDoubleClicked(itemIndex);
		else
			ui->SongListItemClicked(itemIndex);
	}

	void DivaSongList::firstIndexChanged(int v)
	{
		MusicUI::Instance()->slider->setMarkPosition(v);
		MusicUI::Instance()->refreshSongList();
	}

	

	void DivaSongList::itemChanged()
	{
		int t = getItemCount() - maxItem;
		if (t < 0)
			t = 0;
		MusicUI::Instance()->slider->setMarkScale(0, t, getItemCount());

		mapHash.clear();
		for (int i=0; i < items.size(); i++)
			mapHash[((SongListItem*)items[i])->getMapInfo().id] = i;
	}

	void DivaSongList::highlightItemChanged(int index)
	{
		MusicUI::Instance()->SongListHighlightItemChanged(index);
	}

	int DivaSongList::getIndexByMapId(int id) const
	{
		if (mapHash.find(id) != mapHash.end())
			return mapHash.at(id);
		else
			return -1;
	}

	//////////////////////////////////////////////////////////////////////////

	void DivaSongList_Clicked::mouseClicked(MouseEvent& mouseEvent)
	{
		ButtonEx* b = (ButtonEx*)mouseEvent.getSource();
		int t = int(b->userData);
		if (t&1)
			((DivaSongList*)b->getParent())->DifNext(t>>1);
		else
			((DivaSongList*)b->getParent())->DifLast(t>>1);
	}

	}
}
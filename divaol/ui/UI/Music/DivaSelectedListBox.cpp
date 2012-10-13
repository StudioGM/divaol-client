#include "DivaSelectedListBox.h"
#include "MusicUI.h"

namespace diva
{
	namespace MusicUI
	{

	using namespace gcn;

	SongInfo::SongInfo(const divamap::DivaMap& a, int b, int c)
	{
		mapInfo = a;
		difIndex = b;
		type = c;
	}

	SongInfo::SongInfo()
	{
	}

	divamap::DivaMap::LevelType SongInfo::getLevel() const
	{
		int j = difIndex;
		divamap::DivaMap::Levels::const_iterator i = mapInfo.levels.cbegin();
		while (j)
			i++;
		return i->first;
	}



	DivaSelectedListBox::DivaSelectedListBox()
	{
		setOpaque(false);
		setSize(529 ,40 * maxItem);
		items.clear();
		for (int i=0; i<maxItem; i++)
		{
			ButtonEx* b = new ButtonEx();
			b->setSize(529, 38);
			b->setEnabled(false);
			b->userData = (void*) i;
			b->setLook(L"res/UI2.png", 
				gcn::Rectangle(412,861,567,76),
				gcn::Rectangle(412,785,567,76),
				gcn::Rectangle(412,709,567,76),
				gcn::Rectangle(412,861,567,76), -19, -19);
			b->addMouseListener(new DivaSelectedListBox_ItemMouseListener());
			add(b, 0, 40 * i);
			items.push_back(b);
		}
		deleteButtons.clear();
		for (int i=0; i<maxItem; i++)
		{
			ButtonEx* b = new ButtonEx();
			b->setSize(34, 33);
			b->setVisible(false);
			b->userData = (void*) i;
			b->setLook(L"res/UI2.png", 
				gcn::Rectangle(888,0,54,53),
				gcn::Rectangle(834,0,54,53),
				gcn::Rectangle(780,0,54,53),
				gcn::Rectangle(888,0,54,53), -10, -10);
			b->addMouseListener(new DivaSelectedListBox_DeleteMouseListener());
			add(b, 502 - 9, 40 * i + 3);
			deleteButtons.push_back(b);
		}
		songInfo.clear();
	}

	DivaSelectedListBox::~DivaSelectedListBox()
	{

	}

	void DivaSelectedListBox::refreshButtonText(int index)
	{
		wchar_t temp[200];
		if (songInfo[index].type == RANDOM)
			_swprintf(temp, L"RANDOM -- %s", config[L"difNames"][songInfo[index].difIndex].asCString());
		else if (songInfo[index].type = SPECIFIC)
			_swprintf(temp, L"%s -- %s",  songInfo[index].mapInfo.header.name.c_str(), config[L"difNames"][songInfo[index].getLevel()].asCString());
		items[index]->setText(temp);
	}

	void DivaSelectedListBox::pushItem(const divamap::DivaMap& mapInfo, int difIndex, int type)
	{
		if (songInfo.size() >= maxItem)
		{
			throw "items man!";
			return;
		}
		songInfo.push_back(SongInfo(mapInfo, difIndex, type));
		int n = songInfo.size() - 1;
		deleteButtons[n]->setVisible(true);
		items[n]->setEnabled(true);
		refreshButtonText(n);
	}

	void DivaSelectedListBox::clearItems()
	{
		songInfo.clear();
	}

	void DivaSelectedListBox::displayItem(int index)
	{
		MusicUI *ui = MusicUI::Instance();
		if (songInfo[index].type == RANDOM)
		{
			ui->thumbImage->load(ui->randomFileName, ui->randomRect, true);
			ui->songInfoContainer->setTextVisible(false);
			sora::SoraBGMManager::Instance()->stop(false);
			ui->countStarted = false;
			return;
		}

		// DISPLAY SONGLIST
		//@SonicMisora
		//if (songInfo[index].mapInfo.mapThumbFileName != L"NONE")
		//{
		//	ui->thumbImage->load(songInfo[index].mapInfo.mapThumbFileName, gcn::Rectangle(0, 0, 0, 0), true);
		//	ui->background->display(songInfo[index].mapInfo.mapThumbFileName, gcn::Rectangle(0, 0, 0, 0), true);
		//}
		//else
			ui->thumbImage->load(ui->noimageFileName, ui->noimageRect, true);

		//if (songInfo[index].mapInfo.listenFileName != L"NONE" && songInfo[index].mapInfo.listenFileName != L"")
		//{
		//	//sora::SoraBGMManager::Instance()->play(sora::s2ws(songInfo[index].mapInfo.listenFileName), false);
		//	ui->countStarted = true;
		//	ui->listenFileName = songInfo[index].mapInfo.listenFileName;
		//	ui->playTimer.reset();
		//	ui->playTimer.start();
		//}
		//else
		{
			ui->countStarted = false;
			sora::SoraBGMManager::Instance()->stop(false);
		}

		ui->songInfoContainer->setMap(songInfo[index].mapInfo);
		ui->songInfoContainer->setTextVisible(true);
	}

	const SongInfo& DivaSelectedListBox::getSong(int index)
	{
		return songInfo[index];
	}

	int DivaSelectedListBox::getItemCount() const
	{
		return songInfo.size();
	}

	void DivaSelectedListBox::removeItem(int index)
	{
		if (songInfo.size() == 0)
		{
			throw "already no items!";
			return;
		}
		songInfo.erase(songInfo.begin() + index);
		int n = songInfo.size();
		deleteButtons[n]->setVisible(false);
		items[n]->setEnabled(false);
		items[n]->setText(L"");
		for (int i = 0; i < n; i++)
			refreshButtonText(i);
	}



	//////////////////////////////////////////////////////////////////////////

	void DivaSelectedListBox_DeleteMouseListener::mouseClicked(MouseEvent& mouseEvent)
	{
		((DivaSelectedListBox*)(mouseEvent.getSource()->getParent()))->removeItem((int)((ButtonEx*)mouseEvent.getSource())->userData);
		MusicUI::Instance()->AdjustStartButton();
	}

	void DivaSelectedListBox_ItemMouseListener::mouseEntered(MouseEvent& mouseEvent)
	{
		if (!mouseEvent.getSource()->isEnabled())
			return;
		((DivaSelectedListBox*)(mouseEvent.getSource()->getParent()))->displayItem((int)((ButtonEx*)mouseEvent.getSource())->userData);
	}

	void DivaSelectedListBox_ItemMouseListener::mouseExited(MouseEvent& mouseEvent)
	{
		if (!mouseEvent.getSource()->isEnabled())
			return;
		sora::SoraBGMManager::Instance()->stop(false);
		MusicUI::Instance()->countStarted = false;
	}

	}
}
#pragma once

#include "ui/GUIChanEx/GUIChanContainerEx.h"
#include "ui/GUIChanEx/GUIChanButtonEx.h"
#include "ui/structure.h"
#include <vector>
#include "divasongmgr/DivaMapManager.h"
#include "DivaSongListItem.h"

namespace diva
{

	namespace MusicUI
	{

	using namespace gcn;

	struct SongInfo
	{
		divamap::DivaMap mapInfo;
		int difIndex;
		int type;
		SongListItem* item;
		SongInfo(const divamap::DivaMap& a, int b, int c, SongListItem* item);

		SongInfo();
		divamap::DivaMap::LevelType getLevel() const;
		int getDifIndex(divamap::DivaMap::LevelType l) const;
	};

	class DivaSelectedListBox :
		public ContainerEx
	{
	private:

		const static int maxItem = 5;
		std::vector<ButtonEx*> items;
		std::vector<ButtonEx*> deleteButtons;
		std::vector<SongInfo> songInfo;

		void refreshButtonText(int index);

	public:
		enum {RANDOM, SPECIFIC};

		DivaSelectedListBox();
		~DivaSelectedListBox();

		void pushItem(const divamap::DivaMap& mapInfo, divamap::DivaMap::LevelType level, SongListItem* item, int type = SPECIFIC);
		void removeItem(int index);
		void clearItems();
		int getItemCount() const;

		const SongInfo &getSong(int index);
		
		void displayItem(int index);

		//virtual void logic();
	};

	class DivaSelectedListBox_DeleteMouseListener :
		public MouseListener
	{
	public:
		virtual void mouseClicked(MouseEvent& mouseEvent);
	};

	class DivaSelectedListBox_ItemMouseListener :
		public MouseListener
	{
	public:
		virtual void mouseEntered(MouseEvent& mouseEvent);
		virtual void mouseExited(MouseEvent& mouseEvent);
	};

	}
};
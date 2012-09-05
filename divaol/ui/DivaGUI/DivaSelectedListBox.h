#pragma once

#include "ui/GUIChanEx/GUIChanContainerEx.h"
#include "ui/GUIChanEx/GUIChanButtonEx.h"
#include "ui/structure.h"
#include <vector>

namespace diva
{
	using namespace gcn;

	struct SongInfo
	{
		Network::Map mapInfo;
		int difIndex;
		int type;
		SongInfo(const Network::Map& a, int b, int c);

		SongInfo();
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

		void pushItem(const Network::Map& mapInfo, int difIndex, int type = SPECIFIC);
		void removeItem(int index);
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
};
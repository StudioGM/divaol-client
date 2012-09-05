#include "ui/DivaGUI/DivaSongInfoContainer.h"

namespace diva
{
	DivaSongInfoContainer::DivaSongInfoContainer()
	{
		textVisible = false;
	}

	void DivaSongInfoContainer::setMap(const Network::Map& mapInfo)
	{
		this->mapInfo = mapInfo;
	}

	void DivaSongInfoContainer::setTextVisible(bool v)
	{
		textVisible = v;
	}

	void DivaSongInfoContainer::draw(Graphics *graphics)
	{
		ContainerEx::draw(graphics);

		if (textVisible)
		{

			Font* font = getFont();
			graphics->setFont(font);
		
			int h = getHeight() / 5;
		
			char temp[100];
			// Name
			sprintf(temp, "歌曲名 %s", mapInfo.origName.c_str());
			graphics->drawText(temp, 30, h * 1 - font->getHeight() / 2);

			// Artist
			if (mapInfo.artists.size() == 0)
				sprintf(temp, "作曲 未知");
			else
				sprintf(temp, "作曲 %s", mapInfo.artists[0].c_str());
			graphics->drawText(temp, 30, h * 2 - font->getHeight() / 2);

			// Lyric
			if (mapInfo.lyricAuthors.size() == 0)
				sprintf(temp, "作词 未知");
			else
				sprintf(temp, "作词 %s", mapInfo.lyricAuthors[0].c_str());
			graphics->drawText(temp, 200, h * 2 - font->getHeight() / 2);

			// Lyric
			if (mapInfo.composers.size() == 0)
				sprintf(temp, "编曲 未知");
			else
				sprintf(temp, "编曲 %s", mapInfo.composers[0].c_str());
			graphics->drawText(temp, 310, h * 2 - font->getHeight() / 2);

			// noter
			if (mapInfo.noterName == L"NONE")
				sprintf(temp, "Noter 未知");
			else
				sprintf(temp, "Noter %s", mapInfo.noterName.c_str());
			graphics->drawText(temp, 30, h * 3 - font->getHeight() / 2);

			// BPM
			sprintf(temp, "BPM %d", mapInfo.bpm);
			graphics->drawText(temp, 200, h * 3 - font->getHeight() / 2);

			// VOCOLOID
			if (mapInfo.vocaloidUsages.size() == 0)
				sprintf(temp, "使用VOCALOID  未知");
			else
				sprintf(temp, "使用VOCALOID  %s", mapInfo.vocaloidUsages[0].c_str());
			graphics->drawText(temp, 310, h * 3 - font->getHeight() / 2);

			//LINK
			if (mapInfo.directLinkURLs.size() == 0)
				sprintf(temp, "Link  未知");
			else
				sprintf(temp, "Link  %s", mapInfo.directLinkURLs[0].c_str());
			graphics->drawText(temp, 30, h * 4 - font->getHeight() / 2);
		}
	}
}
#include "DivaSongInfoContainer.h"

namespace diva
{
	namespace MusicUI
	{

	DivaSongInfoContainer::DivaSongInfoContainer()
	{
		textVisible = false;
	}

	void DivaSongInfoContainer::setMap(const divamap::DivaMap& mapInfo)
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
		
			wchar_t temp[100];
			// Name
			wsprintf(temp, L"歌曲名 %s", mapInfo.header.name.c_str());
			graphics->drawTextW(temp, 30, h * 1 - font->getHeight() / 2);

			// Artist
			if (mapInfo.header.artists.size() == 0)
				wsprintf(temp, L"作曲 未知");
			else
				wsprintf(temp, L"作曲 %s", mapInfo.header.artists[0].c_str());
			graphics->drawTextW(temp, 30, h * 2 - font->getHeight() / 2);

			// Lyric
			if (mapInfo.header.lyricists.size() == 0)
				wsprintf(temp, L"作词 未知");
			else
				wsprintf(temp, L"作词 %s", mapInfo.header.lyricists[0].c_str());
			graphics->drawTextW(temp, 200, h * 2 - font->getHeight() / 2);

			// Lyric
			if (mapInfo.header.composers.size() == 0)
				wsprintf(temp, L"编曲 未知");
			else
				wsprintf(temp, L"编曲 %s", mapInfo.header.composers[0].c_str());
			graphics->drawTextW(temp, 310, h * 2 - font->getHeight() / 2);

			// noter
			if (mapInfo.header.noters == L"NONE")
				wsprintf(temp, L"Noter 未知");
			else
				wsprintf(temp, L"Noter %s", mapInfo.header.noters.c_str());
			graphics->drawTextW(temp, 30, h * 3 - font->getHeight() / 2);

			// BPM
			wsprintf(temp, L"BPM %d", mapInfo.header.bpm);
			graphics->drawTextW(temp, 200, h * 3 - font->getHeight() / 2);

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
}
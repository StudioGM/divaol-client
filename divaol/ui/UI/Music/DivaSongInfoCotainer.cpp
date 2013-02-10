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
		
			int h = getHeight() / 9;
		
			int x = 40;
			int ydelta = 10;

			wchar_t temp[100];
			// Name
			wsprintf(temp, L"歌曲名 %s", mapInfo.header.name.c_str());
			graphics->drawTextW(temp, x, h * 1 + ydelta);

			// Artist
			if (mapInfo.header.artists.size() == 0)
				wsprintf(temp, L"作曲 未知");
			else
				wsprintf(temp, L"作曲 %s", mapInfo.header.artists[0].c_str());
			graphics->drawTextW(temp, x, h * 2 + ydelta);

			// Lyric
			if (mapInfo.header.lyricists.size() == 0)
				wsprintf(temp, L"作词 未知");
			else
				wsprintf(temp, L"作词 %s", mapInfo.header.lyricists[0].c_str());
			graphics->drawTextW(temp, x, h * 3 + ydelta);

			// Lyric
			if (mapInfo.header.composers.size() == 0)
				wsprintf(temp, L"编曲 未知");
			else
				wsprintf(temp, L"编曲 %s", mapInfo.header.composers[0].c_str());
			graphics->drawTextW(temp, x, h * 4 + ydelta);

			// noter
			if (mapInfo.header.noters.size() == 0)
				wsprintf(temp, L"Noter 未知");
			else
				wsprintf(temp, L"Noter %s", mapInfo.header.noters[0].c_str());
			graphics->drawTextW(temp, x, h * 5 + ydelta);

			// BPM
			wsprintf(temp, L"BPM %d", mapInfo.header.bpm);
			graphics->drawTextW(temp, x, h * 6 + ydelta);

			// VOCOLOID
			if (mapInfo.header.vocaloids.size() == 0)
				wsprintf(temp, L"使用VOCALOID  未知");
			else
			{
				std::wstring vocaloidStr = L"";
				for(int i=0;i<mapInfo.header.vocaloids.size();i++)
				{
					if(vocaloidStr!=L"")
						vocaloidStr += L"&";
					vocaloidStr += mapInfo.header.vocaloids[i];
				}
				wsprintf(temp, L"使用VOCALOID  %s", vocaloidStr.c_str());
			}
			graphics->drawTextW(temp, x, h * 7 + ydelta);

			////LINK
			//if (mapInfo.directLinkURLs.size() == 0)
			//	sprintf(temp, "Link  未知");
			//else
			//	sprintf(temp, "Link  %s", mapInfo.directLinkURLs[0].c_str());
			//graphics->drawText(temp, 30, h * 4 - font->getHeight() / 2);
		}
	}
	}
}
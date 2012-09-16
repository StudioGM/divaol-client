#include "DivaSongListItem.h"
#include "MusicUI.h"
#include <cstring>

namespace diva
{
	namespace MusicUI
	{

	using namespace gcn;

	SongListItem::SongListItem()
	{
		preview = NULL;
		image = NULL;
		difIndex = 0;
		artistFont = NULL;
	}

	SongListItem::SongListItem(Image* image, const divamap::DivaMap& m, int look, Font* artistFont)
	{
		this->look = look;
		this->image = image;
		mapInfo = m;
		difIndex = 0;
		this->artistFont = artistFont;

		if (look == SONG)
		{
			// @SonicMisora
			//if (mapInfo.mapThumbFileName != L"NONE")
			//{
			//	preview = Image::load(mapInfo.mapThumbFileName);
			//}
			//else
				preview = NULL;
		}
		else
			preview = NULL;
	}

	SongListItem::~SongListItem()
	{
	}

	void SongListItem::setImage(Image* image)
	{
		this->image = image;
	}

	Image* SongListItem::getImage() const
	{
		return image;
	}

	int SongListItem::getLook() const
	{
		return look;
	}

	int SongListItem::getDifIndex() const
	{
		return difIndex;
	}

	void SongListItem::nextDif()
	{
		difIndex++;
		if (difIndex >= mapInfo.levels.size())
			difIndex = 0;
	}

	void SongListItem::lastDif()
	{
		difIndex--;
		if (difIndex < 0)
			difIndex = mapInfo.levels.size() - 1;
	}

	const divamap::DivaMap& SongListItem::getMapInfo() const
	{
		return mapInfo;
	}

	void SongListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
	{
		// preview
		
		if (look == SONG && preview)
		{
			int desW = 160, desH = 90;
			(((SoraGUIImage*)preview)->getSprite())->setScale(double(desW) / preview->getWidth(), 
				double(desH) / preview->getHeight());
			graphics->drawImage(preview, 333, 25);
		}
		else
		{
			gcn::Rectangle rect;
			if (state == 0)
				rect = gcn::Rectangle(1432,412+95*2,210,95);
			else if (state == 1)
				rect = gcn::Rectangle(1432,412,210,95);
			else if (state == 2)
				rect = gcn::Rectangle(1432,412+95,210,95);

			int desW = 210, desH = 95;
			MusicUI* ui = MusicUI::Instance();
			(((SoraGUIImage*)ui->uiImage1)->getSprite())->setScale(double(desW) / rect.width, 
				double(desH) / rect.height);
			graphics->drawImage(ui->uiImage1, rect.x, rect.y, 299, 22, rect.width, rect.height);
		}

		if (image)
		{
			if (state == 0)
			{
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawImage(image, 0, 656, 0, 0, 571, 139);
			}
			else if (state == 1)
			{
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawImage(image, 0, 378, 0, 0, 571, 139);
			}
			else if (state == 2)
			{
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawImage(image, 0, 517, 0, 0, 571, 139);
			}
		}

		if (look == SONG)
		{
			std::wstring str;
			graphics->setColor(gcn::Color(255, 255, 255, alpha));

			graphics->setFont(font);

			// Artists
			if (mapInfo.header.artists.size() == 0)
			{
				str = L"未知艺术家";
			}
			else
			{
				str = mapInfo.header.artists[0];
				if (mapInfo.header.artists.size() > 1)
					str += L", ...";
			}

			if (mapInfo.header.vocaloids.size() == 0)
			{
				str += L" Feat. 未知";
			}
			else
			{
				str += L" Feat. " + mapInfo.header.vocaloids[0];
			}

			graphics->drawTextW(str, 30, 27);

			// Name
			str = mapInfo.header.name;
			graphics->drawTextW(str, 30, 47);

			// Length and bpm
			wchar_t temp[30];
			_swprintf(temp, L"%02d:%02d  BPM %d", mapInfo.header.songLength / 60, mapInfo.header.songLength % 60, mapInfo.header.bpm);
			str = temp;
			graphics->drawTextW(str, 30, 67);

			// player
			if (mapInfo.header.mapType == divamap::DivaMapHeader::Normal)
			{
				graphics->drawImage(image, 368, 198, 490, 60, 56, 40);
			}
			else
			{
				graphics->drawImage(image, 299, 198, 490, 60, 56, 40);
			}

			// dif
			graphics->drawTextW(config[L"difNames"][difIndex].asString(), 60, 104);

			// dif bar
			graphics->drawImage(image, 0, 847, 185, 85, 
				int((double(mapInfo.levels[(divamap::DivaMap::LevelType)difIndex].difficualty) / 100) * 368), 43);

		}
		else if (look == RANDOM)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			graphics->drawImage(image, 0, 795, 37, 35, 455, 52);

			// dif
			graphics->setFont(font);
			graphics->drawTextW(L"简单", 60, 104);
		}
		else if (look == ARTIST)
		{
			if (artistFont != NULL)
				graphics->setFont(artistFont);
			else
				graphics->setFont(font);
			graphics->setColor(gcn::Color(255, 255, 255, alpha)); 
			graphics->drawTextW(L"Artist: " + mapInfo.header.artists[0], 60, 40);
		}
	}
	}
}
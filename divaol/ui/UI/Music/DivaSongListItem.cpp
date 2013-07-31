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
			smallPreview = NULL;
			preview = NULL;
			image = NULL;
			difIndex = 0;
			artistFont = NULL;
			this->notDownloadImage = NULL;
			previewFilename = L"";
			listeningFilename  = L"";
			
			setDownloadFinished(false);
			setDownloadProgress(0);
		}

		SongListItem::SongListItem(Image* image, const divamap::DivaMap& m, int look, Font* artistFont, Image* notDownloadImage)
		{
			this->look = look;
			this->image = image;
			mapInfo = m;
			difIndex = 0;
			this->artistFont = artistFont;
			preview = NULL;
			smallPreview = NULL;
			previewFilename = L"";
			listeningFilename  = L"";
			this->notDownloadImage = notDownloadImage;

			setDownloadFinished(false);
			setDownloadProgress(0);
		}

		SongListItem::~SongListItem()
		{
			if (hasPreview())
			{
				delete preview;
			}
		}

		bool SongListItem::hasListening() const
		{
			return listeningFilename != L"";
		}

		const std::wstring& SongListItem::getListening() const
		{
			return listeningFilename;
		}

		void SongListItem::setListening(const std::wstring& filename)
		{
			listeningFilename = filename;
		}

		void SongListItem::setPreview(const std::wstring& filename)
		{
			if (preview)
			{
				delete preview;
			}
			if (filename != L"NONE")
			{
				preview = Image::load(filename);
				previewFilename = filename;
			}
			else
				preview = NULL;
		}

		const std::wstring& SongListItem::getPreviewFilename() const
		{
			return previewFilename;
		}

		bool SongListItem::hasPreview() const
		{
			return preview;
		}

		void SongListItem::setSmallPreview(const std::wstring& filename)
		{
			if (smallPreview)
			{
				delete smallPreview;
			}
			if (filename != L"NONE" && filename != L"")
			{
				smallPreview = Image::load(filename);
			}
			else
				smallPreview = NULL;
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

		double SongListItem::getDownloadProgress() const
		{
			return prog;
		}

		void SongListItem::setDownloadProgress(double p)
		{
			prog = p;
		}

		void SongListItem::setDownloadFinished(bool b)
		{
			downloadFinished = b;
		}

		bool SongListItem::getDownloadFinished() const
		{
			return downloadFinished;
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
		
			if (look == SONG && smallPreview)
			{
				int desW = 160, desH = 90;
				(((SoraGUIImage*)smallPreview)->getSprite())->setScale(double(desW) / smallPreview->getWidth(), 
					double(desH) / smallPreview->getHeight());
				graphics->drawImage(smallPreview, 333, 25);
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
				gcn::Color color;
				if (getDownloadFinished() || look == RANDOM)
					color = gcn::Color(255, 255, 255, alpha);
				else
					color = gcn::Color(80, 80, 80, alpha);
				if (state == 0)
				{
					graphics->setColor(color);
					graphics->drawImage(image, 0, 656, 0, 0, 571, 139);
				}
				else if (state == 1)
				{
					graphics->setColor(color);
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
					str = L"δ֪������";
				}
				else
				{
					str = mapInfo.header.artists[0];
					if (mapInfo.header.artists.size() > 1)
						str += L", ...";
				}

				if (mapInfo.header.vocaloids.size() == 0)
				{
					str += L" Feat. δ֪";
				}
				else
				{
					str += L" Feat. " + mapInfo.header.vocaloids[0];
					if(mapInfo.header.vocaloids.size()>1)
						str += L", ...";
				}

				graphics->drawTextW(str, 30, 27);

				// Name
				str = mapInfo.header.name;
				graphics->drawTextW(str, 30, 47);

				// Length and bpm
				wchar_t temp[30];
				_swprintf(temp, L"%02d:%02d  BPM %d  �Ѷ�:%d", mapInfo.header.songLength / 60, mapInfo.header.songLength % 60, mapInfo.header.bpm, mapInfo.levels[mapInfo.getLevel(difIndex)].difficulty);
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
				graphics->drawTextW(config[L"difNames"][mapInfo.getLevel(difIndex)].asString(), 60, 104);

				// download bar
				if (!getDownloadFinished())
				{
					graphics->drawImage(image, 0, 847, 185, 85, 
						int((prog / 1.0) * 368), 43);
					if (notDownloadImage)
					{
						int desWidth = 38, desHeight = 38;
						(((SoraGUIImage*)notDownloadImage)->getSprite())->setScale(double(desWidth) / notDownloadImage->getWidth(), 
							double(desHeight) / notDownloadImage->getHeight());
						graphics->drawImage(notDownloadImage, 0, 0, 200, 95, notDownloadImage->getWidth(), notDownloadImage->getHeight());
					}
				}

				// preview loading
				if (!smallPreview)
				{
					graphics->drawTextW(L"��ȡ��...", 400, 50);
				}
			}
			else if (look == RANDOM)
			{
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawImage(image, 0, 795, 37, 35, 455, 52);

				// dif
				graphics->setFont(font);
				graphics->drawTextW(config[L"difNames"][difIndex].asString(), 60, 104);
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
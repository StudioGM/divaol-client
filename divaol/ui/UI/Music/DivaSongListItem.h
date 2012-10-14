#pragma once

#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "soraguiimage.hpp"
#include "ui/structure.h"
#include "divasongmgr/DivaMapManager.h"

namespace diva
{
	namespace MusicUI
	{

		using namespace gcn;

		class SongListItem : public ListItemEx
		{
		private:
			double prog;
			bool downloadFinished;

			std::wstring previewFilename;
			std::wstring listeningFilename;
			Image* preview;
			Image* image;
			divamap::DivaMap mapInfo;
			int difIndex;
			int look;
			Font* artistFont;

		public:
			enum {SONG, ARTIST, RANDOM};

			SongListItem();
			SongListItem(Image* image, const divamap::DivaMap& m, int look = SONG, Font* artistFont = NULL);
			~SongListItem();

			void setPreview(const std::wstring& filename);
			bool hasPreview() const;
			const std::wstring& getPreviewFilename() const;

			bool hasListening() const;
			const std::wstring& getListening() const; 
			void setListening(const std::wstring& filename);

			void setImage(Image* image);
			virtual Image* getImage() const;

			void setDownloadProgress(double p);
			double getDownloadProgress() const;

			void setDownloadFinished(bool b);
			bool getDownloadFinished() const;

			int getLook() const;

			const divamap::DivaMap& getMapInfo() const;

			int getDifIndex() const;
			void nextDif();
			void lastDif();

			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);

		};

	}
}
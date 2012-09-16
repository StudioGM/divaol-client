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

			void setImage(Image* image);
			virtual Image* getImage() const;

			int getLook() const;

			const divamap::DivaMap& getMapInfo() const;

			int getDifIndex() const;
			void nextDif();
			void lastDif();

			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);

		};

	}
}
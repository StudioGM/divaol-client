#pragma once

#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "soraguiimage.hpp"
#include "ui/structure.h"

namespace diva
{
	using namespace gcn;

	class SongListItem : public ListItemEx
	{
	private:
		
		Image* preview;
		Image* image;
		Network::Map mapInfo;
		int difIndex;
		int look;
		Font* artistFont;

	public:
		enum {SONG, ARTIST, RANDOM};

		SongListItem();
		SongListItem(Image* image, const Network::Map& m, int look = SONG, Font* artistFont = NULL);
		~SongListItem();
		
		void setImage(Image* image);
		virtual Image* getImage() const;

		int getLook() const;

		const Network::Map& getMapInfo() const;

		int getDifIndex() const;
		void nextDif();
		void lastDif();

		virtual void draw(Graphics* graphics, Font* font, int state, int alpha);

	};
}
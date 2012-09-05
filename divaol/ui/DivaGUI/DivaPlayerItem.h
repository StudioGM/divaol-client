#pragma once

#include "ui/GUIChanEx/GUIChanListItemEx.h"

namespace diva
{
	using namespace gcn;

	class DivaPlayerItem :
		public ListItemEx
	{
	private:
		std::wstring name;
		Image* icon;
		Image* uiImage;
		gcn::Rectangle srcRect;
		bool isHost;
		bool isReady;
		bool isPlaying;
		bool isVoid;
		
	public:
		DivaPlayerItem();
		~DivaPlayerItem();

		void setInfo(const std::wstring name, const std::wstring iconFileName, gcn::Rectangle srcRect, const std::wstring uiFileName);
		void setHost(bool v);
		bool getHost() const;
		void setReady(bool v);
		bool getReady() const;
		void setPlaying(bool v);
		bool getPlaying() const;
		void setVoid(bool v);
		bool getVoid() const;

		virtual void draw(Graphics* graphics, Font* font, int state, int alpha);
	};

};
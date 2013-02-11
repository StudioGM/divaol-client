#include "AvatarListItem.h"
#include "soraguiimage.hpp"
#include "lib/Base/Random.h"
#include "ui/GUIChanEx/GUIChanJsonHelper.h"


namespace diva
{
	namespace HouseUI
	{
		AvatarListItem::AvatarListItem(AnimeBoxEx* animeBox, const gcn::Rectangle& avatarRect, const PointEx& playerNamePos, const std::wstring& name)
		{
			this->animeBox = animeBox;
			this->avatarRect = avatarRect;
			this->playerNamePos = playerNamePos;
			this->name = name;
			this->shadow = NULL;
		}

		void AvatarListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			//-----Shadow
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			shadow->draw(graphics, shadowDesRect);

			//-----Anime
			graphics->pushClipArea(avatarRect);
			animeBox->draw(graphics);
			graphics->popClipArea();

			//-----Text
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			if (name != L"")
			{
				graphics->setFont(font);
				graphics->setColor(gcn::Color(50, 50, 50, alpha));
				int textW = font->getWidthW(name);
				int textH = font->getHeight();
				playerNamePos.x = graphics->getCurrentClipArea().width / 2;
				graphics->fillRectangle(gcn::Rectangle(playerNamePos.x - textW / 2 - 2, playerNamePos.y - 2,  textW + 4, font->getHeight() + 4));
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawTextW(name, playerNamePos.x, playerNamePos.y, gcn::Graphics::CENTER);
			}
		}

		const std::wstring& AvatarListItem::getName() const
		{
			return name;
		}

		void AvatarListItem::update(float dt)
		{
			animeBox->update(dt);
		}

		AvatarListItem* AvatarListItem::FromJson(const WJson::Value& v, const std::wstring& name)
		{
			int size = v[L"avatarAnimes"].size();
			int index = Base::Random::RandomInt(0, size);
			AnimeBoxEx* animeBox = Helper::CreateAnimeBox(v[L"avatarAnimes"][index]);
			AvatarListItem* item = new AvatarListItem(animeBox, Helper::GetRect(v[L"avatarDesRect"]), Helper::GetPoint(v[L"playerNamePosition"]), name);
			item->shadow = Helper::CreateImageTile(v[L"shadow"]);
			item->shadowDesRect = Helper::GetRect(v[L"shadowDesRect"]);
			return item;
		}
	}

}
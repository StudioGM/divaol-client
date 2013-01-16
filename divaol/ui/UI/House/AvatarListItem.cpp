#include "AvatarListItem.h"
#include "soraguiimage.hpp"
#include "lib/Base/Random.h"
#include "ui/GUIChanEx/GUIChanJsonHelper.h"

namespace diva
{
	namespace HouseUI
	{
		AvatarListItem::AvatarListItem(const std::wstring& avatarFilename, const gcn::Rectangle& avatarRect, const PointEx& playerNamePos, const std::wstring& name)
		{
			avatarImage = Image::load(avatarFilename);
			this->avatarRect = avatarRect;
			this->playerNamePos = playerNamePos;
			this->name = name;
		}

		void AvatarListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			if (avatarImage)
			{
				(((SoraGUIImage*)avatarImage)->getSprite())->setScale(double(avatarRect.width) / avatarImage->getWidth(), 
					double(avatarRect.height) / avatarImage->getHeight());
				graphics->drawImage(avatarImage, 0, 0, avatarRect.x, avatarRect.y, avatarImage->getWidth(), avatarImage->getHeight());
			}
			if (name != L"")
			{
				graphics->setFont(font);
				graphics->setColor(gcn::Color(50, 50, 50, alpha));
				int textW = font->getWidthW(name);
				int textH = font->getHeight();
				graphics->fillRectangle(gcn::Rectangle(playerNamePos.x - textW / 2 - 2, playerNamePos.y - 2,  textW + 4, font->getHeight() + 4));
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawTextW(name, playerNamePos.x, playerNamePos.y, gcn::Graphics::CENTER);
			}
		}

		const std::wstring& AvatarListItem::getName() const
		{
			return name;
		}

		AvatarListItem* AvatarListItem::FromJson(const WJson::Value& v, const std::wstring& name)
		{
			int size = v[L"avatarFilenames"].size();
			int index = Base::Random::RandomInt(0, size);
			return new AvatarListItem(v[L"avatarFilenames"][index].asString(), Helper::GetRect(v[L"avatarDesRect"]), Helper::GetPoint(v[L"playerNamePosition"]), name);
		}
	}

}
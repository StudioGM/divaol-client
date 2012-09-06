#include "ui/DivaGUI/DivaPlayerItem.h"
#include "soraguiimage.hpp"
#include "ui/roomUI.h"

namespace diva
{
	DivaPlayerItem::DivaPlayerItem()
	{
		icon = NULL;
		uiImage = NULL;
		name = L"";
		isReady = false;
		isHost = false;
	}

	DivaPlayerItem::~DivaPlayerItem()
	{
		if (icon)
			delete icon;
		if (uiImage)
			delete uiImage;
	}

	void DivaPlayerItem::setHost(bool v)
	{
		isHost = v;
	}

	bool DivaPlayerItem::getHost() const
	{
		return isHost;
	}

	void DivaPlayerItem::setReady(bool v)
	{
		isReady = v;
	}

	bool DivaPlayerItem::getReady() const
	{
		return isReady;
	}

	void DivaPlayerItem::setVoid(bool v)
	{
		isVoid = v;
	}

	bool DivaPlayerItem::getVoid() const
	{
		return isVoid;
	}

	void DivaPlayerItem::setPlaying(bool v)
	{
		isPlaying = v;
	}

	bool DivaPlayerItem::getPlaying() const
	{
		return isPlaying;
	}

	void DivaPlayerItem::setInfo(const std::wstring name, const std::wstring iconFileName,
		gcn::Rectangle srcRect, const std::wstring uiFileName)
	{
		this->name = name;
		this->srcRect = srcRect;
		
		if (iconFileName != L"" && iconFileName!=L"NONE")
		{
			if (icon)
			{
				delete icon;
				icon = NULL;
			}
			icon = Image::load(iconFileName);
			if (srcRect.width == 0)
				this->srcRect = gcn::Rectangle(0, 0, icon->getWidth(), icon->getHeight());
		}

		if (uiFileName!= L"" && uiFileName != L"NONE")
		{
			if (uiImage)
			{
				delete uiImage;
				uiImage = NULL;
			}
			uiImage = Image::load(uiFileName);
		}
	}

	void DivaPlayerItem::draw(Graphics* graphics, Font* font, int state, int alpha)
	{
		// back
		if (uiImage)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			graphics->drawImage(uiImage, 1429,38, 0, 0, 462,116);
		}

		if (!isVoid)
		{
			// Name
			graphics->setFont(font);
			if (!isPlaying)
				graphics->drawTextW(name, 90, 50);
			else
				graphics->drawTextW(name + L"(ÓÎÏ·ÖÐ)", 90, 50);

			// icon
			if (icon)
			{
				gcn::Rectangle d(340, 10, 92, 92);
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				((gcn::SoraGUIImage*)icon)->getSprite()->setScale(
					double(d.width) / srcRect.width,
					double(d.height) / srcRect.height);
				graphics->drawImage(icon, srcRect.x, srcRect.y, d.x, d.y, srcRect.width, srcRect.height);
			}

			// host
			if (getHost())
			{
				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawImage(uiImage, 1365, 38, 420, 10, 36, 24);
			} 
			else if (getReady())
			{
				// room member ready
				UIScreenRoom* ui = UIScreenRoom::Instance();
				((gcn::SoraGUIImage*)ui->uiImage1)->getSprite()->setScale(1.0, 1.0);

				graphics->setColor(gcn::Color(255, 255, 255, alpha));
				graphics->drawImage(ui->uiImage1, 1222, 412, 1704 - 1385, 10, 54, 40);
			}
		}
		else
		{
			// no people
			graphics->setFont(font);
			graphics->drawTextW(L"ÀëÏß", 90, 50);
		}
	}

}
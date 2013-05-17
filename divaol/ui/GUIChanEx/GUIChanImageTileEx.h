#pragma once

#include "guichan.hpp"
#include "soraguiimage.hpp"
#include "SoraSprite.h"
#include "SoraCore.h"
#include <string>

namespace gcn
{
	// Extended in GUIChanEx.
	// By SonicMisora
	// wrapped to show a special part of an image.
	class ImageTileEx
	{
	private:
		std::wstring filename;
		gcn::Rectangle srcRect;
		Image* image;
		bool autoDelete;

	public:
		ImageTileEx()
		{
			filename = L"";
			srcRect = Rectangle(0, 0, 0, 0);
			image = NULL;
			autoDelete = false;
		}

		ImageTileEx(const std::wstring& filename)
		{
			this->filename = filename;
			image = Image::load(filename);
			if (image == NULL)
				throw "error";
			srcRect = gcn::Rectangle(0, 0, image->getWidth(), image->getHeight());
			autoDelete = true;
		}

		ImageTileEx(const std::wstring& filename, const gcn::Rectangle& srcRect)
		{
			this->filename = filename;
			image = Image::load(filename);
			if (image == NULL)
				throw "error";
			this->srcRect = srcRect;
			autoDelete = true;
		}

		ImageTileEx(Image* image)
		{
			this->filename = L"";
			this->image = image;
			this->srcRect = gcn::Rectangle(0, 0, image->getWidth(), image->getHeight());
			autoDelete = false;
		}

		ImageTileEx(Image* image, const gcn::Rectangle& srcRect)
		{
			this->filename = L"";
			this->image = image;
			this->srcRect = srcRect;
			autoDelete = false;
		}

		virtual ~ImageTileEx()
		{
			if (image && autoDelete)
				delete image;
		}

		bool hasImage() const
		{
			return image != NULL;
		}

		Image* getImage() const
		{
			return image;
		}

		const std::wstring& getFilename() const
		{
			return filename;
		}

		const gcn::Rectangle& getSrcRect() const
		{
			return srcRect;
		}

		void draw(Graphics *graphics, int desX, int desY)
		{
			if (image == NULL)
				return;
			(((SoraGUIImage*)image)->getSprite())->setScale(1.0, 1.0);
			graphics->drawImage(image, srcRect.x, srcRect.y, desX, desY, srcRect.width, srcRect.height);
		}

		void draw(Graphics* graphics, int desX, int desY, int desWidth, int desHeight)
		{
		if (image == NULL)
				return;
			(((SoraGUIImage*)image)->getSprite())->setScale(double(desWidth) / srcRect.width, double(desHeight) / srcRect.height);
			graphics->drawImage(image, srcRect.x, srcRect.y, desX, desY, srcRect.width, srcRect.height);
		}

		void draw(Graphics* graphics, const gcn::Rectangle& desRect)
		{
			if (image == NULL)
				return;
			(((SoraGUIImage*)image)->getSprite())->setScale(double(desRect.width) / srcRect.width, double(desRect.height) / srcRect.height);
			graphics->drawImage(image, srcRect.x, srcRect.y, desRect.x, desRect.y, srcRect.width, srcRect.height);
		}
	};

	typedef std::vector<ImageTileEx*> ImageTiles;
}
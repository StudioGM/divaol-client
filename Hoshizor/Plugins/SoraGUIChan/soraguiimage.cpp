#include "soraguiimage.hpp"

#include "guichan/exception.hpp"
#include "guichan/rectangle.hpp"

#include "SoraSprite.h"

namespace gcn
{

    SoraGUIImage::SoraGUIImage(sora::SoraTextureHandle texture)
        : mTexture(texture)
    {
        sora = sora::SoraCore::Instance();
    }

    SoraGUIImage::~SoraGUIImage()
    {
        free();
	}

    void SoraGUIImage::free()
    {
        if (mSoraSprite != NULL)
        {
            delete mSoraSprite;
            mSoraSprite =  NULL;
        }
	}

    sora::SoraSprite *SoraGUIImage::getSprite() const
    {
        return mSoraSprite;
    }

    int SoraGUIImage::getWidth() const
    {
		if(mSoraSprite)
			return mSoraSprite->getSpriteWidth();
		return 0;
    }

    int SoraGUIImage::getHeight() const
    {
		if(mSoraSprite)
			return mSoraSprite->getSpriteHeight();
		return 0;
    }
	
	void SoraGUIImage::setColor(const Color& col) {
		if(mSoraSprite)
			mSoraSprite->setColor(CARGB(col.a, col.r, col.g, col.b));
	}
	
	gcn::Color SoraGUIImage::getColor() const {
		if(mSoraSprite) {
			uint32 hardwareColor = mSoraSprite->getColor();
			return gcn::Color(CGETR(hardwareColor),
							  CGETG(hardwareColor),
							  CGETB(hardwareColor),
							  CGETA(hardwareColor));
		}
		return gcn::Color();
	}
	
	void SoraGUIImage::setRotation(float rot) {
		if(mSoraSprite)
			mSoraSprite->setRotation(rot);
	}

    Color SoraGUIImage::getPixel(int x, int y)
    {
        uint32 *pLockPtr = mSoraSprite->getPixelData();

        if (pLockPtr == NULL)
        {
            throw GCN_EXCEPTION("Locking of the texture failed. HGE only support locking of 32bit textures.");
        }

        uint32 color = pLockPtr[x + y * getWidth()];

        mSoraSprite->unlockPixelData();
        return Color(CGETR(color),
                     CGETG(color), 
                     CGETB(color), 
                     CGETA(color));
		return Color();
    }

    void  SoraGUIImage::putPixel(int x, int y, const Color& color)
    {
        uint32 hardwareColor = CARGB(color.a, color.r, color.g, color.b);

        uint32 *pLockPtr = mSoraSprite->getPixelData();

        if (pLockPtr == NULL) {
            throw GCN_EXCEPTION("Locking of the texture failed. HGE only support locking of 32bit textures.");
        }

        pLockPtr[x + y * getWidth()] = hardwareColor;

        mSoraSprite->unlockPixelData();
    }
	
	void SoraGUIImage::setTextureRect(const Rectangle& rect) {
		if(mSoraSprite)
			mSoraSprite->setTextureRect(static_cast<float>(rect.x), 
										static_cast<float>(rect.y), 
										static_cast<float>(rect.width), 
										static_cast<float>(rect.height));
	}

    void SoraGUIImage::convertToDisplayFormat()
    {
        /*DWORD *pLockPtr = sora->textureLock(mTexture);

        if (pLockPtr == NULL)
        {
            throw GCN_EXCEPTION("Locking of the texture failed. HGE only support locking of 32bit textures.");
        }

        int i;
        int end = ((sora::SoraTexture*)mTexture)->mOriginalWidth * ((sora::SoraTexture*)mTexture)->mOriginalHeight;

        for (i = 0; i < end; i++)
        {
            DWORD color = pLockPtr[i];
            if (CGETR(color) == 0xFF
                && CGETG(color) == 0x00
                && CGETB(color) == 0xFF)
            {
                pLockPtr[i] = CARGB(0x00, 0x00, 0x00, 0x00);
            }
        }

        sora->textureUnlock(mTexture);
		 */
        mSoraSprite = new sora::SoraSprite(mTexture);
	//	mSoraSprite->setZ(GUI_Z);
	//	mSoraSprite->setBlendMode(BLEND_DEFAULT_Z);
    }
	
	void SoraGUIImage::setName(const std::string& n) {
		name = n;
	}
	
	std::string SoraGUIImage::getName() {
		return name;
	}
}

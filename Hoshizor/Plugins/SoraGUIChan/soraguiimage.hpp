#ifndef GCN_SORAIMAGE_HPP
#define GCN_SORAIMAGE_HPP

#include <string>
#include "SoraCore.h"
#include "SoraColor.h"

#include "guichan/color.hpp"
#include "guichan/platform.hpp"
#include "guichan/image.hpp"


#define GUI_Z 0.f

namespace gcn
{
    /**
     * HGE implementation of Image.
     *
     * @author Kevin Lynx
     * @since  0.6.1
     */
    class GCN_EXTENSION_DECLSPEC SoraGUIImage : public Image
    {
    public:
        /**
         * Constructor. 
         * 
         * Creates an image.
         */
        SoraGUIImage(sora::SoraTextureHandle tex);

        /**
         * Destructor.
         *
         */
        ~SoraGUIImage();

      
        virtual sora::SoraSprite *getSprite() const;

    
        // Inherited from Image

        virtual void free();
    
        virtual int getWidth() const;
    
        virtual int getHeight() const;

        virtual Color getPixel(int x, int y);
    
        virtual void putPixel(int x, int y, const Color &color);

        virtual void convertToDisplayFormat();
		
		virtual void setTextureRect(const Rectangle& rect);
		
		virtual void setColor(const Color& color);
		virtual gcn::Color getColor() const;
		virtual void setRotation(float rot);
		
		virtual std::string getName();
		virtual void setName(const std::string& name);
		
    protected:
		std::string name;
		
		sora::SoraCore* sora;
        sora::SoraSprite *mSoraSprite;
		sora::SoraTextureHandle mTexture;
    };
}

#endif // end GCN_HGEIMAGE_HPP

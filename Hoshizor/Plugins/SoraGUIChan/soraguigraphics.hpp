#ifndef GCN_SORAGRAPHICS_HPP
#define GCN_SORAGRAPHICS_HPP

#include "guichan/graphics.hpp"
#include "guichan/platform.hpp"
#include "guichan/color.hpp"

#include "SoraCore.h"

namespace gcn
{
    class Color;

    /**
     * HGE implementation of Graphics.
     *
     * @author Kevin Lynx
     * @since 0.6.1
     */
    class GCN_EXTENSION_DECLSPEC SoraGUIGraphics : public Graphics
    {
    public:
        /**
         * Constructor.
         */
        SoraGUIGraphics();

        /**
         * Destructor.
         */
        ~SoraGUIGraphics();


        //Inherited from Graphics

        void _beginDraw();

        void _endDraw();

        void drawImage(const Image *image,
                       int srcX,
                       int srcY,
                       int dstX,
                       int dstY,
                       int width,
                       int height);

        void drawImage(const Image* image, int dstX, int dstY);

        void drawPoint(int x, int y);

        void drawLine(int x1, int y1, int x2, int y2);

        void drawRectangle(const Rectangle &rectangle);

        void fillRectangle(const Rectangle &rectangle);

        void setColor(const Color &color);

        const Color &getColor() const;

        bool pushClipArea(Rectangle area);

        void popClipArea();
        
        void setGlobalTransparency(float a);
        
        float getGlobalTransparency() const;
		
    protected:
        sora::SoraCore* sora;

		Color mColor;
        uint32 mHardwareColor;
        bool mClipNull;
        float mGlobalTransparency;
    };
}

#endif // end GCN_HGEGRAPHICS_HPP

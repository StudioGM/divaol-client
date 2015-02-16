/*
 *  mColor.h
 *  OpenGLES Study 1
 *
 *  Created by Robert Bu on 09.10.16
 *  Copyright 2009 ring0Dev. All rights reserved.
 *
 */
#ifndef _SORA_COLOR_H_
#define _SORA_COLOR_H_

#include "SoraPlatform.h"
#include "math/SoraMathCommon.h"
#include <algorithm>
#include <cmath>

namespace sora {

#define CARGB(a,r,g,b)	((uint32(a)<<24) + (uint32(r)<<16) + (uint32(g)<<8) + uint32(b))
#define CGETA(col)		(((col)>>24) & 0xFF)
#define CGETR(col)		(((col)>>16) & 0xFF)
#define CGETG(col)		(((col)>>8) & 0xFF)
#define CGETB(col)		((col) & 0xFF)
#define CSETA(col,a)		(((col) & 0x00FFFFFF) + (uint32(a)<<24))
#define CSETR(col,r)		(((col) & 0xFF00FFFF) + (uint32(r)<<16))
#define CSETG(col,g)		(((col) & 0xFFFF00FF) + (uint32(g)<<8))
#define CSETB(col,b)		(((col) & 0xFFFFFF00) + uint32(b))

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
    
    class SoraColorRGBA {
    public:
        SoraColorRGBA():
        r(1.f),
        g(1.f),
        b(1.f),
        a(1.f) {}
        
        SoraColorRGBA(int32 _r, int32 _g, int32 _b):
        r(_r / 255.0f),
        g(_g / 255.0f),
        b(_b / 255.0f),
        a(1.0f) {}
        
        SoraColorRGBA(float _r, float _g, float _b, float _a):
        r(_r), 
        g(_g), 
        b(_b), 
        a(_a) {}
        
        SoraColorRGBA(uint32 col) { 
            *this = col;
        }
        
        void Set(float _r, float _g, float _b, float _a) { 
            this->r = _r; 
            this->g = _g; 
            this->b = _b; 
            this->a = _a;
        }
        
        SoraColorRGBA operator = (const SoraColorRGBA& rhs) {
            this->r = rhs.r; 
            this->g = rhs.g; 
            this->b = rhs.b; 
            this->a = rhs.a; 
            return *this;
        }
        
        SoraColorRGBA operator = (uint32 col) {
            this->a = (col>>24)/255.0f; 
            this->r = ((col>>16) & 0xFF)/255.0f; 
            this->g = ((col>>8) & 0xFF)/255.0f; 
            this->b = (col & 0xFF)/255.0f; 
            return *this;
        }
        
        SoraColorRGBA& operator += (const SoraColorRGBA& rhs) {
            this->r += rhs.r; 
            this->g += rhs.g; 
            this->b += rhs.b; 
            this->a += rhs.a; 
            return *this;
        }
        
        SoraColorRGBA& operator -= (const SoraColorRGBA& rhs) {
            this->r -= rhs.r; 
            this->g -= rhs.g; 
            this->b -= rhs.b; 
            this->a -= rhs.a; 
            return *this;
        }
        SoraColorRGBA& operator *= (const SoraColorRGBA& rhs) {
            this->r *= rhs.r; 
            this->g *= rhs.g; 
            this->b *= rhs.b; 
            this->a *= rhs.a; 
            return *this;
        }
        SoraColorRGBA& operator /= (float s) {
            this->r /= s; 
            this->g /= s; 
            this->b /= s; 
            this->a /= s; 
            return *this;
        }
        SoraColorRGBA& operator *= (float s) {
            this->r *= s; 
            this->g *= s; 
            this->b *= s; 
            this->a *= s; 
            return *this; 
        }
        
        SoraColorRGBA operator - (const SoraColorRGBA& rhs)	const {
            return SoraColorRGBA(this->r-rhs.r, this->g-rhs.g, this->b-rhs.b, this->a-rhs.a);
        }
        
        SoraColorRGBA operator + (float s)	const {
            return SoraColorRGBA(this->r+s, this->g+s, this->b+s, this->a+s);
        }
        SoraColorRGBA operator - (float s)	const {
            return SoraColorRGBA(this->r-s, this->g-s, this->b-s, this->a-s);
        }
        SoraColorRGBA operator * (float s)	const {
            return SoraColorRGBA(this->r*s, this->g*s, this->b*s, this->a*s);
        }
        SoraColorRGBA operator / (float s)	const {
            sora_assert(s != 0.f);
            return SoraColorRGBA(this->r/s, this->g/s, this->b/s, this->a/s);
        }
        SoraColorRGBA operator * (const SoraColorRGBA& rhs) const {
            return SoraColorRGBA(this->r * rhs.r, 
                                 this->g * rhs.g, 
                                 this->b * rhs.b, 
                                 this->a * rhs.a);
        }
        
        
        SoraColorRGBA operator + (const SoraColorRGBA& rhs) const {
            return SoraColorRGBA(this->r+rhs.r, 
                                 this->g+rhs.g, 
                                 this->b+rhs.b, 
                                 this->a+rhs.a);
        }
        
        bool operator == (const SoraColorRGBA& rhs)	{
            return (this->r==rhs.r && this->g==rhs.g && this->b==rhs.b && this->a==rhs.a);
        }
        bool operator != (const SoraColorRGBA& rhs)	{
            return !(*this == rhs);
        }
        
        unsigned int getHWColor() { 
            normalize();
            return ((unsigned int)(this->a*255.0f)<<24) + 
                    ((unsigned int)(this->r*255.0f)<<16) +
                    ((unsigned int)(this->g*255.0f)<<8) + 
                    (unsigned int)(this->b*255.0f);
        }
        static uint32 GetHWColor(float oR, float og, float ob, float oa)  { 
            return ((unsigned int)(oa*255.0f)<<24) + 
                    ((unsigned int)(oR*255.0f)<<16) +
                    ((unsigned int)(og*255.0f)<<8) + 
                    (unsigned int)(ob*255.0f);
        }
        
        operator uint32() {
            return getHWColor();
        }
        
        
        SoraColorRGBA normalize() {
            this->r = clamp(0.f, 1.f, r);
            this->g = clamp(0.f, 1.f, g);
            this->b = clamp(0.f, 1.f, b);
            this->a = clamp(0.f, 1.f, a);
            return *this;
        }
        
        float r;
        float g;
        float b;
        float a;
    };
	
    namespace Color {
        static SoraColorRGBA White          = SoraColorRGBA(1.f, 1.f, 1.f, 1.f);
        static SoraColorRGBA Black          = SoraColorRGBA(0.f, 0.f, 0.f, 1.f);
        static SoraColorRGBA Red            = SoraColorRGBA(1.f, 0.f, 0.f, 1.f);
        static SoraColorRGBA Blue           = SoraColorRGBA(0.f, 0.f, 1.f, 1.f);
        static SoraColorRGBA Lime           = SoraColorRGBA(0.f, 1.f, 0.f, 1.f);
        static SoraColorRGBA Marron         = SoraColorRGBA(0xFF800000);
        static SoraColorRGBA Darkred        = SoraColorRGBA(0xFF8B0000);
        static SoraColorRGBA Crimson        = SoraColorRGBA(0xFFDC143C);
        static SoraColorRGBA Darkmagenta    = SoraColorRGBA(0xFF8B008B);
        static SoraColorRGBA Orchid         = SoraColorRGBA(0xFFDA70D6);
        static SoraColorRGBA Thistle        = SoraColorRGBA(0xFFD8BFD8);
        static SoraColorRGBA Plum           = SoraColorRGBA(0xFFDDA0DD);
        static SoraColorRGBA Violet         = SoraColorRGBA(0xFFEE82EE);
        static SoraColorRGBA Slateblue      = SoraColorRGBA(0xFF6A5ACD);
        static SoraColorRGBA Mediumslateblue= SoraColorRGBA(0xFF7B68EE);
        static SoraColorRGBA Darkblue       = SoraColorRGBA(0xFF00008B);
        static SoraColorRGBA Mediumblue     = SoraColorRGBA(0xFF0000CD);
        static SoraColorRGBA Navy           = SoraColorRGBA(0xFF000080);
        static SoraColorRGBA Royalblue      = SoraColorRGBA(0xFF4169E1);
        static SoraColorRGBA Lightsteelblue = SoraColorRGBA(0xFFB0C4DE);
        static SoraColorRGBA Aliceblue      = SoraColorRGBA(0xFFF0F8FF);
        static SoraColorRGBA Ghostblue      = SoraColorRGBA(0xFFF8F8FF);
        static SoraColorRGBA Lavender       = SoraColorRGBA(0xFFE6E6FA);
        static SoraColorRGBA Dodgerblue     = SoraColorRGBA(0xFF1E90FF);
        static SoraColorRGBA Steelblue      = SoraColorRGBA(0xFF00BFFF);
        static SoraColorRGBA Deepskyblue    = SoraColorRGBA(0xFF4169E1);
        static SoraColorRGBA Slategray      = SoraColorRGBA(0xFF808090);
        static SoraColorRGBA Lightskyblue   = SoraColorRGBA(0xFF87CEFA);
        static SoraColorRGBA Skyblue        = SoraColorRGBA(0xFF87CEEB);
        static SoraColorRGBA Lightblue      = SoraColorRGBA(0xFFADD8E6);
        static SoraColorRGBA Teal           = SoraColorRGBA(0xFF008080);
        static SoraColorRGBA Darkturquoise  = SoraColorRGBA(0xFF00CED1);
        static SoraColorRGBA Aqua           = SoraColorRGBA(0x0000FFFF);
        static SoraColorRGBA Mediumturquoise= SoraColorRGBA(0xFF48D1CC);
        static SoraColorRGBA Cadetblue      = SoraColorRGBA(0xFF5F9EA0);
        static SoraColorRGBA Paleturquoise  = SoraColorRGBA(0xFFAFEEEE);
        static SoraColorRGBA Lightcyan      = SoraColorRGBA(0xFFE0FFFF);
        static SoraColorRGBA Azure          = SoraColorRGBA(0xFFF0FFFF);
        static SoraColorRGBA Lightseagreen  = SoraColorRGBA(0xFF20B2AA);
        static SoraColorRGBA Turquoise      = SoraColorRGBA(0xFFAFEEEE);
        static SoraColorRGBA Powderblue     = SoraColorRGBA(0xFF40E0D0);
        static SoraColorRGBA Darkslategray  = SoraColorRGBA(0xFF2F4F4F);
        static SoraColorRGBA Aquamarine     = SoraColorRGBA(0xFF7FFFD4);
        static SoraColorRGBA Springgreen    = SoraColorRGBA(0xFF00FF7F);
        static SoraColorRGBA Mediumseagreen = SoraColorRGBA(0xFF3CB371);
        static SoraColorRGBA Seagreen       = SoraColorRGBA(0xFF2E8B57);
        static SoraColorRGBA Limegreen      = SoraColorRGBA(0xFF32CD32);
        static SoraColorRGBA Darkgreen      = SoraColorRGBA(0xFF006400);
        static SoraColorRGBA Green          = SoraColorRGBA(0xFF008000);
        static SoraColorRGBA Forestgreen    = SoraColorRGBA(0xFF228B22);
        static SoraColorRGBA Darkseagreen   = SoraColorRGBA(0xFF8FBC8F);
        static SoraColorRGBA Lightgreen     = SoraColorRGBA(0xFF90EE90);
        static SoraColorRGBA Palegreen      = SoraColorRGBA(0xFF98FB98);
        static SoraColorRGBA Mintcream      = SoraColorRGBA(0xFFF5FFFA);
        static SoraColorRGBA Honeydew       = SoraColorRGBA(0xFFF0FFF0);
        static SoraColorRGBA Chartreuse     = SoraColorRGBA(0xFF7FFF00);
        static SoraColorRGBA Olivedrab      = SoraColorRGBA(0xFF6B8E23);
        static SoraColorRGBA Darkolivegreen = SoraColorRGBA(0xFF556B2F);
        static SoraColorRGBA Yellowgreen    = SoraColorRGBA(0xFF9ACD32);
        static SoraColorRGBA Greenyellow    = SoraColorRGBA(0xFFADFF2F);
        static SoraColorRGBA Beige          = SoraColorRGBA(0xFFF5F5DC);
        static SoraColorRGBA Linen          = SoraColorRGBA(0xFFFAF0E6);
        static SoraColorRGBA Olive          = SoraColorRGBA(0xFF808000);
        static SoraColorRGBA Yellow         = SoraColorRGBA(0xFFFFFF00);
        static SoraColorRGBA Lightyellow    = SoraColorRGBA(0xFFFFFFE0);
        static SoraColorRGBA Ivory          = SoraColorRGBA(0xFFFFFFF0);
        static SoraColorRGBA Darkkhaki      = SoraColorRGBA(0xFFBDB76B);
        static SoraColorRGBA Khaki          = SoraColorRGBA(0xFFF0E68C);
        static SoraColorRGBA Palegold       = SoraColorRGBA(0xFFEEE8AA);
        static SoraColorRGBA Wheat          = SoraColorRGBA(0xFFF6DEB3);
        static SoraColorRGBA Gold           = SoraColorRGBA(0xFFFFD700);
        static SoraColorRGBA Lemonchiffon   = SoraColorRGBA(0xFFFFFACD);
        static SoraColorRGBA Papayawhip     = SoraColorRGBA(0xFFFFEFD5);
        static SoraColorRGBA Darkgold       = SoraColorRGBA(0xFFB8860B);
        static SoraColorRGBA Goldenrod      = SoraColorRGBA(0xFFDAA520);
        static SoraColorRGBA Antiquewhite   = SoraColorRGBA(0xFFFAEBD7);
        static SoraColorRGBA Cornsilk       = SoraColorRGBA(0xFFFFF8DC);
        static SoraColorRGBA Oldlace        = SoraColorRGBA(0xFFFDF5E6);
        static SoraColorRGBA Moccasin       = SoraColorRGBA(0xFFFFE4B5);
        static SoraColorRGBA Navajowhite    = SoraColorRGBA(0xFFFFDEAD);
        static SoraColorRGBA Orange         = SoraColorRGBA(0xFFFFA500);
        static SoraColorRGBA Bisque         = SoraColorRGBA(0xFFFFE4CE);
        static SoraColorRGBA Tan            = SoraColorRGBA(0xFFD2B48C);
        static SoraColorRGBA Darkorange     = SoraColorRGBA(0xFFFF8C00);
        static SoraColorRGBA Burlywood      = SoraColorRGBA(0xFFDEB887);
        static SoraColorRGBA Saddlebrown    = SoraColorRGBA(0xFF8B4513);
        static SoraColorRGBA Sandybrown     = SoraColorRGBA(0xFFF4A460);
        static SoraColorRGBA Blanchedalmond = SoraColorRGBA(0xFFFFEBCD);
        static SoraColorRGBA Lavenderblush  = SoraColorRGBA(0xFFFFF0F5);
        static SoraColorRGBA Seashell       = SoraColorRGBA(0xFFFFF5EE);
        static SoraColorRGBA Floralwhite    = SoraColorRGBA(0xFFFFFAF0);
        static SoraColorRGBA Snow           = SoraColorRGBA(0xFFFFFAFA);
        static SoraColorRGBA Peru           = SoraColorRGBA(0xFFCD853F);
        static SoraColorRGBA Peachpuff      = SoraColorRGBA(0xFFFFDAB9);
        static SoraColorRGBA Chocolate      = SoraColorRGBA(0xFFD2691E);
        static SoraColorRGBA Sienna         = SoraColorRGBA(0xFFA0522D);
        static SoraColorRGBA Lightsalmon    = SoraColorRGBA(0xFFFFA07A);
        static SoraColorRGBA Coral          = SoraColorRGBA(0xFFFF7F50);
        static SoraColorRGBA Darksalm       = SoraColorRGBA(0xFFE9967A);
        static SoraColorRGBA Mistyrose      = SoraColorRGBA(0xFFFFE4E1);
        static SoraColorRGBA Orangered      = SoraColorRGBA(0xFFFF4500);
        static SoraColorRGBA Salmon         = SoraColorRGBA(0xFFFA8072);
        static SoraColorRGBA Tomato         = SoraColorRGBA(0xFFFF6347);
        static SoraColorRGBA Rosybrow       = SoraColorRGBA(0xFFBC8F8F);
        static SoraColorRGBA Pink           = SoraColorRGBA(0xFFFFC0CB);
        static SoraColorRGBA Indianred      = SoraColorRGBA(0xFFCD5C5C);
        static SoraColorRGBA Lightcoral     = SoraColorRGBA(0xFFF08080);
        static SoraColorRGBA Brown          = SoraColorRGBA(0xFFA52A2A);
        static SoraColorRGBA Firebrik       = SoraColorRGBA(0xFFB22222);
        static SoraColorRGBA Dimgray        = SoraColorRGBA(0xFF696969);
        static SoraColorRGBA Gray           = SoraColorRGBA(0xFF808080);
        static SoraColorRGBA Darkgray       = SoraColorRGBA(0xFFA9A9A9);
        static SoraColorRGBA Silver         = SoraColorRGBA(0xFFC0C0C0);
        static SoraColorRGBA Lightgrey      = SoraColorRGBA(0xFFD3D3D3);
        static SoraColorRGBA Gainsboro      = SoraColorRGBA(0xFFDCDCDC);
        static SoraColorRGBA Whitesmoke     = SoraColorRGBA(0xFFF5F5F5);        
    } // namespace color
    
    class SoraColorHSV {
    public:
        float h, s, v, a;
        
        SoraColorHSV(float _h, float _s, float _v, float _a):
        h(_h), 
        s(_s), 
        v(_v), 
        a(_a) {}
        
        SoraColorHSV(uint32 col) {
            setHWColor(col);
        }
        
        bool operator ==(const SoraColorHSV& c) const;
        bool operator !=(const SoraColorHSV& c) const;
        
        void setHWColor(uint32 col);
        uint32 getHWColor() const;
    };
    
    inline void SoraColorHSV::setHWColor(uint32 col) {
        float r, g, b;
        float minv, maxv, delta;
        float del_R, del_G, del_B;
        
        this->a = (col>>24) / 255.0f;
        r = ((col>>16) & 0xFF) / 255.0f;
        g = ((col>>8)  & 0xFF) / 255.0f;
        b = (col       & 0xFF) / 255.0f;
        
        minv = std::min(std::min(r, g), b);
        maxv = std::max(std::max(r, g), b);
        delta = maxv - minv;
        
        this->v = maxv;
        
        if (delta == 0) {
            this->h = 0;
            this->s = 0;
        }
        else {
            this->s = delta / maxv;
            del_R = (((maxv - r) / 6) + (delta / 2)) / delta;
            del_G = (((maxv - g) / 6) + (delta / 2)) / delta;
            del_B = (((maxv - b) / 6) + (delta / 2)) / delta;
            
            if      (r == maxv) {this->h = del_B - del_G;}
            else if (g == maxv) {this->h = (1 / 3) + del_R - del_B;}
            else if (b == maxv) {this->h = (2 / 3) + del_G - del_R;}
            
            if (this->h < 0) this->h += 1;
            if (this->h > 1) this->h -= 1;
        }
    }
    
    inline uint32 SoraColorHSV::getHWColor() const {
        float r, g, b;
        float xh, i, p1, p2, p3;
        
        if (this->s == 0) {
            r = v;
            g = v;
            b = v;
        }
        else {
            xh = h * 6;
            if(xh == 6) xh=0;
            i = floorf(xh);
            p1 = this->v * (1 - this->s);
            p2 = this->v * (1 - this->s * (xh - i));
            p3 = this->v * (1 - this->s * (1 - (xh - i)));
            
            if      (i == 0) {r = v;  g = p3; b = p1;}
            else if (i == 1) {r = p2; g = v;  b = p1;}
            else if (i == 2) {r = p1; g = v;  b = p3;}
            else if (i == 3) {r = p1; g = p2; b = v; }
            else if (i == 4) {r = p3; g = p1; b = v; }
            else			 {r = v;  g = p1; b = p2;}
        }
        
        return (uint32(a*255.0f)<<24) + (uint32(r*255.0f)<<16) + (uint32(g*255.0f)<<8) + uint32(b*255.0f);
    }
    

} // namespace sora

#endif
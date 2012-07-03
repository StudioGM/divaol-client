/*
 *  SoraRect.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/27/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */
#ifndef SORA_RECT_H
#define SORA_RECT_H

#include "SoraVector.h"

namespace sora {
    
    struct SORA_API SoraRect {
        
        real x1, y1;
        real x2, y2;
        
        SoraRect(real _x1, real _y1, real _x2, real _y2, bool asWH=false):
        x1(_x1), y1(_y1),
        x2(_x2), y2(_y2) {
            
        }
        
        SoraRect() {   
        }
        
        SoraRect(const SoraVector& upper, const SoraVector& lower):
        x1(upper.x), y1(upper.y),
        x2(lower.x), y2(lower.y) {
            
        }
        
        SoraRect(const SoraVector& pos, real r) {
            setAsBox(pos.x, pos.y, r);
        }
        
        void set(const SoraVector& upper, const SoraVector& lower) {
            this->x1 = upper.x;
            this->y1 = upper.y;
            this->x2 = upper.x;
            this->y2 = upper.y;
        }
        
        void set(real _x1, real _y1, real _x2, real _y2, bool asWH=false) {
            this->x1 = _x1; 
            this->y1 = _y1;
            this->x2 = asWH ? _x1 + _x2 : _x2;
            this->y2 = asWH ? _y1 + _y2 : _y2;
        }
        
        void setAsBox(real x, real y, real r) {
            this->x1 = x - r;
            this->y1 = y - r;
            this->x2 = x + r;
            this->y2 = y + r;
        }
        
        real getWidth() const {
            return this->x2 - this->x1;
        }
        
        real getHeight() const {
            return this->y2 - this->y1;
        }
        
        SoraVector getUpper() const {
            return SoraVector(this->x1, this->y1);
        }
        
        SoraVector getLower() const {
            return SoraVector(this->x2, this->y2);
        }
        
        void encapsulate(real x, real y) {
            if(x<this->x1) this->x1 = x;
            if(x>this->x2) this->x2 = x;
            if(y<this->y1) this->y1 = y;
            if(y>this->y2) this->y2 = y;
        }
        
        bool testPoint(real x, real y) const {
            return x>=x1 && x<=x2 && y>=y1 && y<=y2;
        }
        
        bool testPoint(const SoraVector& pos) const {
            return testPoint(pos.x, pos.y);
        }
        
        bool intersect(const SoraRect& rhs) const {
            if(fabs(this->x1 + this->x2 - rhs.x1 - rhs.x2) < (this->x2 - this->x1 + rhs.x2 - rhs.x1)) {
                if(fabs(this->y1 + this->y2 - rhs.y1 - rhs.y2) < (this->y2 - this->y1 + rhs.y2 - rhs.y1)) {
                    return true;
                }
            }
            return false;
        }
        
        SoraVector getPosition() const {
            return SoraVector((this->x1 + this->x2)/2,
                              (this->y1 + this->y2)/2);
        }
    };
    
    typedef SoraRect SoraAABB;
}



#endif

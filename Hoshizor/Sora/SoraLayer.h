//
//  SoraLayer.h
//  Sora
//
//  Created by Robert Bu on 7/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraLayer_h
#define Sora_SoraLayer_h

#include "SoraPlatform.h"
#include "SoraObject.h"

namespace sora {
    
    /**
     *  Base class for layer
     *  Used by SoraScene
     **/
        
    class SORA_API SoraLayer: public SoraObject {
    public:
        SoraLayer(int32 layerDepth);
        virtual ~SoraLayer();
        
        bool operator < (const SoraLayer& rhs);
        
        void    setLayerDepth(int32 depth);
        int32   getLayerDepth() const;
        
        /**
         *  Combine two layers together
         **/
        SoraLayer& combine(const SoraLayer& layer);
        
        /**
         *  Remove all duplicate objects among the two layers
         **/
        SoraLayer& removeDuplicate(const SoraLayer& layer);
        
        /**
         *  Swap this layer with another
         **/
        void swap(SoraLayer& layer);
        
        /**
         *  Clone a layer
         **/
        SoraLayer* clone();
      
    private:
        int32 mLayerDepth;
    };
    
    inline bool SoraLayer::operator <(const SoraLayer& rhs) {
        return mLayerDepth < rhs.mLayerDepth;
    }

    inline int32 SoraLayer::getLayerDepth() const {
        return mLayerDepth;
    }
} // namespace sora


#endif

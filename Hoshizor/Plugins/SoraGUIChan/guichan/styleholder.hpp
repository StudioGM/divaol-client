//
//  styleholder.h
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) on 8/26/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef gcn_styleholder_h
#define gcn_styleholder_h

#include <string>
#include <map>

namespace gcn {
    
    class Style;
    
    /**
     * Class that manages the style with a string tag
     * When destroys, all style that haven't been removed would be deleted
     *
     * @author Robert Bu(darkfall)
     * @since Guichan for Hoshizora
     **/
    
    class StyleHolder {
    public:
        StyleHolder();
        ~StyleHolder();
        
        /**
         * Global entry
         **/
        static StyleHolder& Instance();
        
        /**
         * Add a style with tag
         * If the style already exist, will throw a exception
         * @param the tag of the style
         * @retval the added style
         **/
        Style* addStyle(const std::string& tag);
        
        /**
         * Add a custom style with tag
         * If the style already exist, will throw a exception
         * @param the tag of the style
         * @param the style to be added
         **/
        void addStyle(const std::string& tag, Style* style);
        
        /**
         * Get a style with tag
         * If the style does not exist, will return NULL
         * @param the tag of the style
         * @retval the style
         **/
        Style* getStyle(const std::string& tag) const;
        
        /**
         * Remove a style from the holder
         * Would NOT free the memory
         * @param the tag of the style
         * @retval the removed style
         **/
        Style* removeStyle(const std::string& tag);
        
    private:
        typedef std::map<std::string, Style*> StyleMap;
       
        StyleMap mStyles;
    };
    
    /**
     * Global entries
     **/
    static Style* GetStyle(const std::string& tag) {
        return StyleHolder::Instance().getStyle(tag);
    }
    
    static Style* RemoveStyle(const std::string& tag) {
        return StyleHolder::Instance().removeStyle(tag);
    }
    
    static Style* AddStyle(const std::string& tag) {
        return StyleHolder::Instance().addStyle(tag);
    }
    
    static void AddStyle(const std::string& tag, Style* style) {
        StyleHolder::Instance().addStyle(tag, style);
    }
    
} // namespace sora



#endif

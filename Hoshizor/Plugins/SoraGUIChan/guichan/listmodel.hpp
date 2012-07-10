/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GCN_LISTMODEL_HPP
#define GCN_LISTMODEL_HPP

#include <string>
#include <vector>

#include "guichan/platform.hpp"


namespace gcn
{
    /**
     * An interface for a model that represents a list. It is 
     * used in certain widgets, like the ListBox, to handle a 
     * lists with string elements. If you want to use widgets 
     * like ListBox, make a derived class from this class that 
     * represents your list.
     */
    class GCN_CORE_DECLSPEC ListModel
    {

    public:
        /**
         * Destructor.
         */
        virtual ~ListModel() { }

        /**
         * Gets the number of elements in the list.
         *
         * @return The number of elements in the list
         */
        virtual int getNumberOfElements() {
            return static_cast<int>(mElements.size());
        }
		
        /**
         * Gets an element at a certain index in the list.
         *
         * @param i An index in the list.
         * @return An element as a string at the a certain index.
         */
        virtual std::string getElementAt(int i) {
            if(i >= 0 && i <= (int)mElements.size())
                return mElements[i];
            return std::string();
        }
        
        /**
         * Push a element into the tail of the list
         *
         * @param the element to add
         * @author Robert Bu(darkfall)
         * @since GuiChan for Hoshizora
         */
        virtual void pushElement(const std::string& elmnt) {
            mElements.push_back(elmnt);
        }
        
        /**
         * Push a element into the tail of the list
         *
         * @param the element to add
         * @author Robert Bu(darkfall)
         * @since GuiChan for Hoshizora
         */
        virtual void setElementAt(int i, const std::string& elmnt) {
            if(i >= 0 && i <= (int)mElements.size())
                mElements[i] = elmnt;
        }
        
        /**
         * Remove a element at
         *
         * @parma the element index
         * @author Robert Bu(darkfall)
         * @since Guichan for Hoshizora
         */
        virtual void removeElementAt(int i)  {
            if(i >= 0 && i <= (int)mElements.size())
                mElements.erase(mElements.begin()+i);
        }

		virtual void clearElements() {
			mElements.clear();
		}
        
    protected:
        typedef std::vector<std::string> StringList;
        StringList mElements;
    };
}

#endif // end GCN_LISTMODEL_HPP

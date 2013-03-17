#ifndef WListModel_H
#define WListModel_H

#include <string>
#include <vector>

#include "guichan/platform.hpp"


namespace gcn
{
    class GCN_CORE_DECLSPEC WListModel
    {

    public:
        /**
         * Destructor.
         */
        virtual ~WListModel() { }

        virtual int getNumberOfElements() {
            return mElements.size();
        }
		
        virtual std::wstring getElementAt(int i) {
            if(i >= 0 && i <= mElements.size())
                return mElements[i];
            return std::wstring();
        }
		virtual int getElementIndex(std::wstring element){
			for(int i=0;i<mElements.size();i++)
				if(mElements[i] == element)
					return i;
			return -1;
		}

		virtual void insertElement(int i, const std::wstring& elmnt)
		{
			if(i<0)
				i=0;
			if(i>=mElements.size())
				pushElement(elmnt);
			else
				mElements.insert(mElements.begin()+i, elmnt);
		}

        virtual void pushElement(const std::wstring& elmnt) {
            mElements.push_back(elmnt);
        }
        
        virtual void setElementAt(int i, const std::wstring& elmnt) {
            if(i >= 0 && i < mElements.size())
                mElements[i] = elmnt;
        }
        virtual void removeElementAt(int i)  {
            if(i >= 0 && i < mElements.size())
                mElements.erase(mElements.begin()+i);
        }

		virtual void clearElements() {
			mElements.clear();
		}
        
    protected:
        typedef std::vector<std::wstring> WStringList;
        WStringList mElements;
    };
}

#endif // end GCN_LISTMODEL_HPP

#include "divaeditor/DivaEditorCommon.h"
#include "divacore/Core/DivaCore.h"


namespace divaeditor
{

	EditorConfig* EditorConfig::Ptr = NULL;


	void EditorConfig::ChangeEditState()
	{
		if(EDITSTATE_NOTESTATE==NOTESTATE::NORMAL)
			EDITSTATE_NOTESTATE=NOTESTATE::LONG;
		else if(EDITSTATE_NOTESTATE==NOTESTATE::LONG)
			EDITSTATE_NOTESTATE=NOTESTATE::COMBO;
		else
			EDITSTATE_NOTESTATE=NOTESTATE::NORMAL;
	}


	void EditorConfig::noteIndexChanged(int oldIndex, int newIndex)
	{
		for(int i=0;i<noteSelected.size();i++)
			if(noteSelected[i]==oldIndex)
				if(newIndex!=-1)
					noteSelected[i]=newIndex;
				else
				{
					noteSelected.erase(noteSelected.begin()+i);
					i--;
				}
	}

	void EditorConfig::noteIndexSwaped(int indexL,int indexR)
	{
		for(int i=0;i<noteSelected.size();i++)
			if(noteSelected[i]==indexL)
				noteSelected[i]=indexR;
			else if(noteSelected[i]==indexR)
				noteSelected[i]=indexL;
	}

	void EditorConfig::addSelectedNote(int index)
	{
		//check exist
		for(int i=0;i<noteSelected.size();i++)
			if(noteSelected[i]==index)
			{
				noteSelected.erase(noteSelected.begin()+i);
				return;
			}
		noteSelected.push_back(index);
	}

	void EditorConfig::deleteSelectedNote(int index)
	{
		for(int i=0;i<noteSelected.size();i++)
			if(noteSelected[i]==index)
			{
				noteSelected.erase(noteSelected.begin()+i);
				i--;
			}
			else if(noteSelected[i]>index)
				noteSelected[i]--;
	}

	void EditorConfig::clearSelectedNote()
	{
		noteSelected.clear();
	}

	bool EditorConfig::isNoteSelected(int index)
	{
		for(int i=0;i<noteSelected.size();i++)
			if(noteSelected[i]==index)
				return true;
		return false;
	}


	//Utility
	//int to Wstring()
	std::wstring iToWS(int n) 
	{
		std::wostringstream strStream;
		strStream<<n;
		return strStream.str();
	}

	//Float to String
	std::string fTos(float f,int bit)
	{
		std::string ret;
		if(f<0)
		{
			ret += "-";
			f=-f;
		}

		ret += divacore::iToS(int(f));

		if(bit>0)
		{
			ret += '.';

			float tail = f - int(f);
			for(int i=0;i<bit;i++)
			{
				tail*=10;
				ret += int(tail) + '0';
			}
		}
		return ret;
	}

	//Float to WString
	std::wstring fTows(float f,int bit)
	{
		std::wstring ret;
		if(f<0)
		{
			ret += L"-";
			f=-f;
		}

		ret += iToWS(int(f));

		if(bit>0)
		{
			ret += L".";

			float tail = f - int(f);
			for(int i=0;i<bit;i++)
			{
				tail*=10;
				ret += (wchar_t)(int(tail) + '0');
			}
		}
		return ret;
	}

	//WString to float
	float wsTof(std::wstring &s)
	{
		//Check validate
		int dotPosCount=s.length();
		bool negative=0;
		for(int i=0;i<s.length();i++)
		{
			if(s[i]==(wchar_t)'.')
			{
				if(dotPosCount!=s.length()) //too many dots
					return 0;
				else
					dotPosCount = i;
			}
			else if((s[i]==(wchar_t)'-'&&!negative))
				negative=true;
			else if(s[i]<(wchar_t)'0'||s[i]>(wchar_t)'9')
				return 0;

		}

		float leftPart = 0;
		for(int i=negative?1:0;i<dotPosCount;i++)
		{
			leftPart*=10;
			leftPart+=s[i]-(wchar_t)'0';
		}

		float rightPart=0;
		for(int i=s.length()-1;i>dotPosCount;i--)
		{
			rightPart+=s[i]-(wchar_t)'0';
			rightPart/=10;
		}

		return negative?(-((leftPart+rightPart))):(leftPart+rightPart);
	}

	//round to integer
	double round(double r) {
		return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
	}
}

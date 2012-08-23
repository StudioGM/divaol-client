#ifndef DivaEditorLocalization
#define DivaEditorLocalization

#include <cstdarg>
#include <string>
#include <map>

namespace divaeditor
{

	class Localization
	{
	private:
		static void InitFromLocalizationJsonFile(Localization* local);

		std::map<std::wstring, std::wstring> strMap;
		std::wstring font;
		std::wstring language;

	public:
		static Localization* Ptr;
		static Localization* instance() {static Localization instance; InitFromLocalizationJsonFile(&instance); return (Ptr=&instance);}

		std::wstring getLocalStr(std::wstring strID,...);
		std::wstring getLocalFontPath();
	};

#define LOCALIZATION divaeditor::Localization::instance()


}



#endif
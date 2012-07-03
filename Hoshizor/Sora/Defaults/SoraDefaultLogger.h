#ifndef SORA_DEFAULT_LOGGER_H_
#define SORA_DEFAULT_LOGGER_H_

#include <fstream>

#include "SoraPlatform.h"

namespace sora {

	class SoraDefaultLogger {
	public:
		SoraDefaultLogger(): logfilepath("SoraLog.txt") {
			file.open(logfilepath.c_str());
			if(file.is_open())
				file<<"============================Hoshi no Sora==================================\n";
		//	file.close();
		}
		inline ~SoraDefaultLogger() {  file.close(); };

		inline void setFile(const SoraString& file) { logfilepath = file; }

		inline void log(const SoraString& str) {
		//	std::fstream file(logfile.c_str(), std::ios::app);
			if(!file.is_open()) return;

			file<<str;
		//	file.close();
		}
		inline void log(const SoraWString& str) {
		/*	std::fstream file;
#if defined(__APPLE_CC__) || defined(__GNUC__)
			file.open(logfile.c_str(), (std::_Ios_Openmode)(std::ios::app || std::ios::out));
#else
			file.open(logfile.c_str(), (std::ios::app || std::ios::out));
#endif*/
			if(!file.is_open()) return;

			file<<str.c_str();
		//	file.close();
		}

	private:
		SoraString logfilepath;
        std::ofstream file;
	};

} // namespace sora


#endif // SORA_DEFAULT_LOGGER_H_

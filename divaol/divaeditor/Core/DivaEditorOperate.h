#ifndef DivaEditorOperate_H
#define DivaEditorOperate_H


#include <vector>
//#include "SoraAny.h"

namespace divaeditor
{

	class DivaEditorOperation
	{
	protected:
		std::string operation_type;

	public:
		bool needToRefreshAll,needToRecalcTime;
		bool isTemp;

		DivaEditorOperation(){needToRefreshAll=true;needToRecalcTime=false;isTemp=false;operation_type="ERROR";}

		virtual void doOperation() = 0;
		virtual void undoOperation() = 0;

		std::string getOperationType(){return operation_type;}

		virtual std::wstring ToString() = 0;
	};


}




#endif
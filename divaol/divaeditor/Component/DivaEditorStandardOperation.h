#ifndef DivaEditorStandardOperation_H
#define DivaEditorStandardOperation_H

#include "divaeditor/Core/DivaEditorMapData.h"
#include "divaeditor/Core/DivaEditorOperate.h"

#include "divacore/Core/DivaMapInfo.h"

#include <vector>

namespace divaeditor
{


	class DivaEditorOperationSet : public DivaEditorOperation
	{
	public:
		std::vector<DivaEditorOperation*> operations;

		DivaEditorOperationSet();
		~DivaEditorOperationSet();

		virtual void doOperation();
		virtual void undoOperation();

		void clearOperation();
		void addOperation(DivaEditorOperation* newOperation);
		void merge(DivaEditorOperationSet* tomerge);

		virtual std::wstring ToString();
	};



	class DivaEditorOperation_BPM : public DivaEditorOperation
	{
	private:
		float bpmOld, bpmNew;
		float position;

	public:
		enum Type{CHANGEBPM,INSERTBPM,DELETEBPM} bpmOperation_Type;

		DivaEditorOperation_BPM(float bpmOld, float bpmNew, float position, Type bpmOperation_Type);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};
	class DivaEditorOperation_GridOffset : public DivaEditorOperation
	{
	private:
		int offsetOld,offsetNew;

	public:
		DivaEditorOperation_GridOffset(int offsetOld, int offsetNew);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};
	class DivaEditorOperation_STOP : public DivaEditorOperation
	{
	private:
		int stopOld, stopNew;
		float position;

	public:
		enum Type{CHANGESTOP, INSERTSTOP, DELETESTOP} stopOperation_Type;

		DivaEditorOperation_STOP(int stopOld, int stopNew, float position, Type stopOperation_Type);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};
	class DivaEditorOperation_BeatNum : public DivaEditorOperation
	{
	private:
		int beatNumOld, beatNumNew;
		int period;

	public:
		enum Type{INSERTBEATNUM, CHANGEBEATNUM, DELETEBEATNUM} beatNumOperation_Type;

		DivaEditorOperation_BeatNum(int beatNumOld, int beatNumNew,int period, Type beatNumOperation_Type);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};
	class DivaEditorOperation_TailSpeed : public DivaEditorOperation
	{
	private:
		float tailSpeedOld,tailSpeedNew;

	public:
		DivaEditorOperation_TailSpeed(float tailSpeedOld, float tailSpeedNew);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};





	class DivaEditorOperation_AddNormalNote : public DivaEditorOperation
	{
	private:
		divacore::MapNote addedNote;
		int addedIndex;

	public:
		DivaEditorOperation_AddNormalNote(divacore::MapNote& addedNote);
		DivaEditorOperation_AddNormalNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, int key=-1);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};

	class DivaEditorOperation_AddLongNote : public DivaEditorOperation
	{
	private:
		divacore::MapNote addedNote;
		int addedIndex;

	public:
		DivaEditorOperation_AddLongNote(divacore::MapNote& addedNote);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};

	class DivaEditorOperation_DeleteNote : public DivaEditorOperation
	{
	private:
		divacore::MapNote deletedNote;

	public:
		DivaEditorOperation_DeleteNote(int deletedIndex);

		virtual void doOperation();
		virtual void undoOperation();

		virtual std::wstring ToString();
	};

	class DivaEditorOperation_ModifyNote : public DivaEditorOperation
	{
	private:

		std::string key;
		int tailX,tailY;
		int x,y;
		bool isDelta;
		int pos;
		int type;
		bool needDecode;
		char keyPress;
		bool arrow;

	public:

		bool calculated;
		divacore::MapNote oldNote,newNote;

		enum Type{KEY,TAIL,POS,TIMEPOS,TYPEBYTYPE,TYPE} noteModifyType;

		DivaEditorOperation_ModifyNote(int index,	std::string key);									//note_modifyKey
		DivaEditorOperation_ModifyNote(int index,	int tailX,		int tailY);							//note_modifyTail
		DivaEditorOperation_ModifyNote(int index,	int x,			int y,         bool isDelta);		//note_modifyPos
		DivaEditorOperation_ModifyNote(int index,	int pos,		bool isDelta);						//note_modifyTimePos
		DivaEditorOperation_ModifyNote(int index,	int type,		bool isDelta,    bool needDecode);	//note_modifyTypeByType
		DivaEditorOperation_ModifyNote(int index,	char keyPress,	bool arrow);						//note_modifyType
		
		virtual void doOperation();
		virtual void undoOperation();

		void merge(DivaEditorOperation_ModifyNote* tomerge);

		virtual std::wstring ToString();
	};
}





#endif
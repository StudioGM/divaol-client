///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2010 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://luaplus.org/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifndef BUILDING_LUAPLUS
#define BUILDING_LUAPLUS
#endif
#include "LuaLink.h"
LUA_EXTERN_C_BEGIN
#include "src/lobject.h"
LUA_EXTERN_C_END
#include "LuaPlus.h"

#if defined(WIN32) && !defined(_XBOX) && !defined(_XBOX_VER)
#include <windows.h>
#undef GetObject
#undef LoadString
#elif defined(_XBOX) || defined(_XBOX_VER)
#include <xtl.h>
#endif // WIN32

#include <ctype.h>

NAMESPACE_LUA_BEGIN
LUA_EXTERN_C int str_format_helper (luaL_Buffer* b, lua_State *L, int arg);
NAMESPACE_LUA_END

namespace LuaPlus {

class LuaStateOutputDebugStringFile : public LuaStateOutFile
{
public:
	LuaStateOutputDebugStringFile() {}
	virtual ~LuaStateOutputDebugStringFile() {}

	virtual bool Open(const char* fileName) {
		(void)fileName;
		return true;
	}

	virtual void Close() {
	}

	virtual void Print(const char* str, ...) {
		char message[800];
		va_list arglist;

		va_start(arglist, str);
		vsprintf(message, str, arglist);
		va_end(arglist);

#if defined(WIN32) || defined(_XBOX) || defined(_XBOX_VER)
		OutputDebugStringA(message);
#else // !WIN32
		puts(message);
#endif // WIN32
	}

protected:
};


static void luaI_addquotedbinary (LuaStateOutFile& file, const char* s, size_t l) {
	file.Print("%c", '"');
	while (l--) {
		switch (*s) {
			case '"':  case '\\':
				file.Print("\\%c", *s);
				break;
			case '\a':		file.Print("\\a");		break;
			case '\b':		file.Print("\\b");		break;
			case '\f':		file.Print("\\f");		break;
			case '\n':		file.Print("\\n");		break;
			case '\r':		file.Print("\\r");		break;
			case '\t':		file.Print("\\t");		break;
			case '\v':		file.Print("\\v");		break;
			default:
				if (isprint((unsigned char)*s))
					file.Print("%c", *s);
				else
					file.Print("\\x%02x", (unsigned int)(unsigned char)*s);
		}
		s++;
	}
	file.Print("%c", '"');
}


#if LUA_WIDESTRING

static void luaI_addquotedwidebinary (LuaStateOutFile& file, const lua_WChar* s, int l) {
	file.Print("L\"");
	while (l--) {
		switch (*s) {
			case '"':
			case '\\':
				file.Print("\\%c", *s);
				break;
			case '\a':		file.Print("\\a");		break;
			case '\b':		file.Print("\\b");		break;
			case '\f':		file.Print("\\f");		break;
			case '\n':		file.Print("\\n");		break;
			case '\r':		file.Print("\\r");		break;
			case '\t':		file.Print("\\t");		break;
			case '\v':		file.Print("\\v");		break;
			default:
				if (*s < 256  &&  isprint((unsigned char)*s)) {
					file.Print("%c", *s);
				} else {
					file.Print("\\x%04x", (unsigned int)*s);
				}
		}
		s++;
	}
	file.Print("\"");
}

#endif /* LUA_WIDESTRING */

#define bufflen(B)	((B)->p - (B)->buffer)

static int LS_LuaFilePrint(LuaState* state) {
	LuaStateOutFile* file = (LuaStateOutFile*)state->Stack(1).GetUserData();

	luaL_Buffer b;
	str_format_helper(&b, *state, 2);

	size_t l = bufflen(&b);
	if (l != 0) {
#if LUA_WIDESTRING
		if (b.isWide) {
			luaplus_assert(0);
		}
		else
#endif /* LUA_WIDESTRING */
		{
			luaL_addchar(&b, 0);
			file->Print(b.buffer);
		}
	}

	return 0;
}


static int LS_LuaFileIndent(LuaState* state) {
	LuaStateOutFile* file = (LuaStateOutFile*)state->Stack(1).GetUserData();
	int indentLevel = (int)state->Stack(2).GetInteger();
	file->Indent((unsigned int)indentLevel);

	return 0;
}


#if LUAPLUS_DUMPOBJECT

bool LuaState::CallFormatting(LuaObject& tableObj, LuaStateOutFile& file, int indentLevel,
		bool writeAll, bool alphabetical, bool writeTablePointers, unsigned int maxIndentLevel) {
	LuaObject metaTableObj = tableObj.GetMetaTable();
	if (metaTableObj.IsNil())
		return false;

	LuaObject formattedWriteObj = metaTableObj["FormattedWrite"];
	if (!formattedWriteObj.IsFunction())
		return false;

	LuaState* state = tableObj.GetState();

	{
		LuaObject funcObj = state->GetGlobals()["LuaFilePrint"];
		if (funcObj.IsNil()) {
			state->GetGlobals().Register("LuaFilePrint", LS_LuaFilePrint);
		}

		funcObj = state->GetGlobals()["LuaFileIndent"];
		if (funcObj.IsNil()) {
			state->GetGlobals().Register("LuaFileIndent", LS_LuaFileIndent);
		}
	}

	LuaCall call = formattedWriteObj;
	call << &file << tableObj << alphabetical << indentLevel << maxIndentLevel << writeAll << writeTablePointers << LuaRun();

	return true;
}


struct KeyValue {
	LuaObject key;
	LuaObject value;

	inline bool operator<(const KeyValue& right) const {
		if (key.Type() == right.key.Type()) {
			if (key.IsBoolean()  &&  right.key.IsBoolean())
				return !key.GetBoolean();
			return key < right.key;
		}
		if (key.IsNumber())
			return true;
		if (key.IsString()  &&  !right.key.IsNumber())
			return true;
		return false;
	}
};


static void WriteKey(LuaStateOutFile& file, LuaObject& key) {
	if (key.IsNumber()) {
		char keyName[255];
		sprintf(keyName, "[%.16g]", key.GetNumber());
		file.Print("%s", keyName);
	} else if (key.IsString()) {
		const char* ptr = key.GetString();
		bool isAlphaNumeric = true;
		if (isdigit(*ptr))
			isAlphaNumeric = false;
		while (*ptr) {
			if (!isalnum(*ptr)  &&  *ptr != '_') {
				isAlphaNumeric = false;
				break;
			}
			ptr++;
		}

		if (isAlphaNumeric)
			file.Print("%s", key.GetString());
		else {
			file.Print("[");
			luaI_addquotedbinary(file, key.GetString(), key.StrLen());
			file.Print("]");
		}
	} else if (key.IsBoolean()) {
		file.Print(key.GetBoolean() ? "[true]" : "[false]");
	}
}


static bool KeyValueCompare(const KeyValue& left, const KeyValue &right) {
	return left < right;
}


template<typename E>
class SimpleList
{
public:
	SimpleList()
		: m_pHead(NULL)
		, m_pTail(NULL)
	{
	}

	~SimpleList() throw()
	{
		while (m_pHead)
		{
			CNode* pKill = m_pHead;
			m_pHead = m_pHead->m_pNext;
			delete pKill;
		}

		m_pHead = NULL;
		m_pTail = NULL;
	}

	void AddTail(E& element)
	{
		CNode* pNewNode = new CNode(element);
		pNewNode->m_pPrev = m_pTail;
		pNewNode->m_pNext = NULL;

		if (m_pTail)
			m_pTail->m_pNext = pNewNode;
		else
			m_pHead = pNewNode;

		m_pTail = pNewNode;
	}

	void* GetHeadPosition() const throw()
	{
		return m_pHead;
	}

	E& GetNext( void*& pos ) throw()
	{
		CNode* pNode = (CNode*)pos;
		pos = (void*)pNode->m_pNext;
		return pNode->m_element;
	}

	// Algorithm taken from http://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.html by Simon Tatham.
	template <typename CompareT>
	void Sort(CompareT Compare)
	{
		CNode *q, *e;

		if (m_pHead == NULL)
			return;

		int insize = 1;

		while (1)
		{
			CNode* p = m_pHead;
			m_pHead = NULL;
			m_pTail = NULL;

			int nmerges = 0;  /* count number of merges we do in this pass */

			while (p) {
				nmerges++;  /* there exists a merge to be done */
				/* step `insize' places along from p */
				q = p;
				int psize = 0;
				for (int i = 0; i < insize; i++)
				{
					psize++;
					q = q->m_pNext;
					if (!q)
						break;
				}

				/* if q hasn't fallen off end, we have two lists to merge */
				int qsize = insize;

				/* now we have two lists; merge them */
				while (psize > 0 || (qsize > 0 && q))
				{
					/* decide whether next element of merge comes from p or q */
					if (psize == 0)
					{
						/* p is empty; e must come from q. */
						e = q;
						q = q->m_pNext;
						qsize--;
					}
					else if (qsize == 0 || !q)
					{
						/* q is empty; e must come from p. */
						e = p;
						p = p->m_pNext;
						psize--;
					}
					else if (Compare(p->m_element, q->m_element))
					{	// p < q
						/* First element of p is lower (or same);
						* e must come from p. */
						e = p;
						p = p->m_pNext;
						psize--;
					}
					else
					{
						/* First element of q is lower; e must come from q. */
						e = q;
						q = q->m_pNext;
						qsize--;
					}

					/* add the next element to the merged list */
					if (m_pTail)
						m_pTail->m_pNext = e;
					else
						m_pHead = e;
					e->m_pPrev = m_pTail;
					m_pTail = e;
				}

				/* now p has stepped `insize' places along, and q has too */
				p = q;
			}

			m_pTail->m_pNext = NULL;

			/* If we have done only one merge, we're finished. */
			if (nmerges <= 1)   /* allow for nmerges==0, the empty list case */
			{
				return;
			}

			/* Otherwise repeat, merging lists twice the size */
			insize *= 2;
		}
	}

private:
	class CNode
	{
	public:
		CNode( E& element ) : m_element( element )  { }

		CNode* m_pNext;
		CNode* m_pPrev;
		E m_element;

	private:
		CNode( const CNode& ) throw();
	};

	CNode* m_pHead;
	CNode* m_pTail;

	SimpleList( const SimpleList& ) throw();
	SimpleList& operator=( const SimpleList& ) throw();
};


/**
	Writes a Lua object to a text file.
**/
bool LuaState::DumpObject(LuaStateOutFile& file, LuaObject& key, LuaObject& value,
						 unsigned int flags, int indentLevel, unsigned int maxIndentLevel)
{
	bool alreadyDumpedKey = (flags & 0xF0000000) != 0;
	flags &= ~0xF0000000;

	// If the value is nil, don't write it.
	if (value.IsNil())
		return false;

	// Indent the line the number of spaces for the current indentation level.
	const unsigned int INDENT_SIZE = 1;
	const unsigned int indentSpaces = (indentLevel == -1 ? 0 : indentLevel) * INDENT_SIZE;

	// If the variable is user data or a function...
	if (!alreadyDumpedKey  &&  value.IsUserData()  ||  value.IsFunction()  ||  value.IsCFunction())
	{
		// ... only write it if they requested it be written.  But only do
		// it as a comment.
		if (flags & DUMP_WRITEALL)
		{
			if ((unsigned int)indentLevel < maxIndentLevel)
				file.Indent(indentSpaces);
			else
				file.Print(" ");

			if (value.IsUserData())
			{
				file.Print("-- ");
				if (!key.IsNil())
				{
					WriteKey(file, key);
					file.Print(" = ");
				}
				file.Print("'userdata: %p'", value.GetUserData());
			}
			else if (value.IsCFunction())
			{
				file.Print("-- ");
				if (!key.IsNil())
				{
					WriteKey(file, key);
					file.Print(" = ");
				}
				file.Print("'cfunction: %p'", value.GetCFunction());
			}
			else if (value.IsFunction())
			{
				lua_Debug ar;
				value.Push();
				lua_getinfo(*this, ">S", &ar);
//				printf("%d\r\n", ar.linedefined);
				file.Print("-- ");
				if (!key.IsNil())
				{
					WriteKey(file, key);
					file.Print(" = ");
				}
				file.Print("'function: %s %d'", ar.source, ar.linedefined);
			}

			return true;
		}

		return false;
	}

	if (!alreadyDumpedKey)
	{
		if ((unsigned int)indentLevel < maxIndentLevel)
			file.Indent(indentSpaces);
		else
			file.Print(" ");

		// If the object has a name, write it out.
		if (!key.IsNil())
		{
			WriteKey(file, key);

			file.Print(" = ");
		}
	}

	// If the object's value is a number, write it as a number.
	if (value.IsBoolean())
		file.Print("%s", value.GetBoolean() ? "true" : "false");

	else if (value.IsNumber())
		file.Print(LUA_NUMBER_FMT, value.GetNumber());

	// Or if the object's value is a string, write it as a quoted string.
	else if (value.IsString())
	{
		luaI_addquotedbinary(file, value.GetString(), value.StrLen());
	}

#if LUA_WIDESTRING
	// Or if the object's value is a string, write it as a quoted string.
	else if (value.IsWString())
	{
		luaI_addquotedwidebinary(file, value.GetWString(), value.StrLen());
	}
#endif /* LUA_WIDESTRING */

	// Otherwise, see if the object's value is a table.
	else if (value.IsTable())
	{
		bool calledFormatting =
				CallFormatting(value, file, indentLevel, (flags & DUMP_WRITEALL) != 0,
					(flags & DUMP_ALPHABETICAL) != 0, (flags & DUMP_WRITETABLEPOINTERS) != 0, maxIndentLevel);
		if (!calledFormatting)
		{
			// Write the table header.
			if (indentLevel != -1)
			{
				if ((unsigned int)indentLevel + 1 < maxIndentLevel)
				{
					file.Print("\r\n");
					file.Indent(indentSpaces);
				}
				if (flags & DUMP_WRITETABLEPOINTERS)
					file.Print("{ --%8x\r\n", value.GetLuaPointer());
				else
					file.Print("{");
				if ((unsigned int)indentLevel + 1 < maxIndentLevel)
				{
					file.Print("\r\n");
				}
			}

			// Rename, just for ease of reading.
			LuaObject& table = value;

			// upperIndex is the upper index value of a sequential numerical array
			// items.
			int upperIndex = 1;
			bool wroteSemi = false;
			bool hasSequential = false;

			// Block to search for array items.
			{
				// Grab index 1 and index 2 of the table.
				LuaObject value1 = table[1];
				LuaObject value2 = table[2];

				// If they both exist, then there is a sequential list.
				if (!value1.IsNil())
				{
					// Cycle through the list.
					bool headSequential = true;
					for (; ; ++upperIndex)
					{
						// Try retrieving the table entry at upperIndex.
						LuaObject value = table[upperIndex];

						// If it doesn't exist, then exit the loop.
						if (value.IsNil())
							break;

						// Only add the comma and return if not on the head item.
						if (!headSequential  &&  indentLevel != -1)
						{
							file.Print(",");
							if ((unsigned int)indentLevel + 1 < maxIndentLevel)
							{
								file.Print("\r\n");
							}
						}

						// Write the object as an unnamed entry.
						LuaObject nilObj(this);
						DumpObject(file, nilObj, value, flags, indentLevel + 1, maxIndentLevel);

						// We've definitely passed the head item now.
						headSequential = false;
					}
				}
			}

			// Did we find any sequential table values?
			if (upperIndex > 1)
			{
				hasSequential = true;
			}

			if (flags & DUMP_ALPHABETICAL)
			{
				SimpleList<KeyValue> keys;

				// Cycle through the table.
				for (LuaTableIterator it(table); it; ++it)
				{
					// Retrieve the table entry's key and value.
					LuaObject& key = it.GetKey();

					// Is the key a number?
					if (key.IsNumber())
					{
						// Yes, were there sequential array items in this table?
						if (hasSequential)
						{
							// Is the array item's key an integer?
							lua_Number realNum = key.GetNumber();
							int intNum = (int)realNum;
							if (realNum == (lua_Number)intNum)
							{
								// Yes.  Is it between 1 and upperIndex?
								if (intNum >= 1  &&  intNum < upperIndex)
								{
									// We already wrote it as part of the sequential
									// list.
									continue;
								}
							}
						}
					}

					KeyValue info;
					info.key = key;
					info.value = it.GetValue();
					keys.AddTail(info);
				}

				keys.Sort(KeyValueCompare);

				if (keys.GetHeadPosition() != NULL)
				{
					// If we wrote a sequential list, the value we're about to write
					// is not nil, and we haven't written the semicolon to separate
					// the sequential table entries from the keyed table entries...
					if (hasSequential  &&  indentLevel != -1)
					{
						// Then add a comma (for good measure).
						file.Print(", ");
						if ((unsigned int)indentLevel + 1 < maxIndentLevel)
						{
							file.Print("\r\n");
						}
						wroteSemi = true;
					}
				}

				for (void* keysIt = keys.GetHeadPosition(); keysIt; )
				{
					KeyValue& info = keys.GetNext(keysIt);

					// Write the table entry.
					bool ret = DumpObject(file, info.key, info.value, flags,
							indentLevel + 1, maxIndentLevel);

					// Add a comma after the table entry.
					if (indentLevel != -1  &&  ret)
					{
						file.Print(",");
						if ((unsigned int)indentLevel + 1 < maxIndentLevel)
						{
							file.Print("\r\n");
						}
					}
				}
			}
			else
			{
				// Cycle through the table.
				for (LuaTableIterator it(table); it; ++it)
				{
					// Retrieve the table entry's key and value.
					LuaObject& key = it.GetKey();

					// Is the key a number?
					if (key.IsNumber())
					{
						// Yes, were there sequential array items in this table?
						if (hasSequential)
						{
							// Is the array item's key an integer?
							lua_Number realNum = key.GetNumber();
							int intNum = (int)realNum;
							if (realNum == (lua_Number)intNum)
							{
								// Yes.  Is it between 1 and upperIndex?
								if (intNum >= 1  &&  intNum < upperIndex)
								{
									// We already wrote it as part of the sequential
									// list.
									continue;
								}
							}
						}
					}

					// If we wrote a sequential list, the value we're about to write
					// is not nil, and we haven't written the semicolon to separate
					// the sequential table entries from the keyed table entries...
					if (hasSequential  &&  !value.IsNil()  &&  !wroteSemi)
					{
						// Then add a comma (for good measure).
						if (indentLevel != -1)
						{
							file.Print(", ");
							if ((unsigned int)indentLevel + 1 < maxIndentLevel)
							{
								file.Print("\r\n");
							}
						}
						wroteSemi = true;
					}

					// Write the table entry.
					bool ret = DumpObject(file, key, it.GetValue(), flags,
						indentLevel + 1, maxIndentLevel);

					// Add a comma after the table entry.
					if (ret  &&  indentLevel != -1)
					{
						file.Print(",");
						if ((unsigned int)indentLevel + 1 < maxIndentLevel)
						{
							file.Print("\r\n");
						}
					}
				}
			}

			// If we wrote a sequential list and haven't written a semicolon, then
			// there were no keyed table entries.  Just write the final comma.
			if (hasSequential  &&  !wroteSemi  &&  indentLevel != -1)
			{
				file.Print(",");
				if ((unsigned int)indentLevel + 1 < maxIndentLevel)
				{
					file.Print("\r\n");
				}
			}

			// Indent, with the intent of closing up the table.
			file.Indent(indentSpaces);

			// If the indentation level is 0, then we're at the root position.
			if (indentLevel == 0)
			{
				// Add a couple extra returns for readability's sake.
				file.Print("}");
				if ((unsigned int)indentLevel + 1 < maxIndentLevel)
				{
					file.Print("\r\n\r\n");
				}
			}
			else if (indentLevel > 0)
			{
				// Close the table.  The comma is written when WriteObject()
				// returns from the recursive call.
				file.Print("}");
			}
		}
	}

	// If the indentation level is at the root, then add a return to separate
	// the lines.
	if (indentLevel == 0)
	{
		if ((unsigned int)indentLevel < maxIndentLevel)
		{
			file.Print("\r\n");
		}
	}

	return true;
}


/**
	Writes a Lua object to a text file.
**/
bool LuaState::DumpObject(LuaStateOutFile& file, const char* name, LuaObject& value,
						 unsigned int flags, int indentLevel, unsigned int maxIndentLevel)
{
	// Yes, this is hack-ish.

	// If the value is nil, don't write it.
	if (value.IsNil())
		return false;

	const unsigned int INDENT_SIZE = 1;
	const unsigned int indentSpaces = (indentLevel == -1 ? 0 : indentLevel) * INDENT_SIZE;

	// If the variable is user data or a function...
	if (value.IsUserData()  ||  value.IsFunction()  ||  value.IsCFunction())
	{
		// ... only write it if they requested it be written.  But only do
		// it as a comment.
		if ((flags & DUMP_WRITEALL)  &&  name)
		{
			if (value.IsUserData())
			{
				file.Print("-- %s", name);
				file.Print(" = '!!!USERDATA!!!'\r\n");
			}
			else if (value.IsFunction())
			{
				lua_Debug ar;
				value.Push();
				lua_getinfo(*this, ">S", &ar);
//				printf("%d\r\n", ar.linedefined);
				file.Print("-- %s", name);
				file.Print(" = '!!!FUNCTION!!! %s %d'\r\n", ar.source, ar.linedefined);
			}
			else
			{
				file.Print("-- %s", name);
				file.Print(" = '!!!CFUNCTION!!!'\r\n");
			}

			return true;
		}

		return false;
	}

	if ((unsigned int)indentLevel < maxIndentLevel)
		file.Indent(indentSpaces);
	else
		file.Print(" ");

	// If the object has a name, write it out.
	if (name)
	{
		file.Print("%s = ", name);
	}

	LuaObject key(this);
	bool ret = DumpObject(file, key, value, flags | 0xF0000000, indentLevel, maxIndentLevel);
	file.Print("\r\n");
	return ret;
}


/**
	Save the complete script state.
**/
bool LuaState::DumpObject(const char* filename, LuaObject& key, LuaObject& value,
						 unsigned int flags, int indentLevel, unsigned int maxIndentLevel)
{
	if (!key.IsString())
	{
		// Open the text file to write the script state to.
		LuaStateOutFile regFile;
		LuaStateOutputDebugStringFile odsFile;

		LuaStateOutFile* file;

		if (strcmp(filename, "@") == 0)
		{
			file = &odsFile;
		}
		else
		{
			file = &regFile;
			if (!file->Open(filename))
				return false;
		}

		return DumpObject(*file, key, value, flags, indentLevel, maxIndentLevel);
	}
	else
	{
		return DumpObject(filename, key.GetString(), value, flags, indentLevel, maxIndentLevel);
	}
}


/**
	Save the complete script state.
**/
bool LuaState::DumpObject(const char* filename, const char* name, LuaObject& value,
						 unsigned int flags, int indentLevel, unsigned int maxIndentLevel)
{
	// Open the text file to write the script state to.
	LuaStateOutFile regFile;
	LuaStateOutputDebugStringFile odsFile;

	LuaStateOutFile* file;

	if (strcmp(filename, "@") == 0)
	{
		file = &odsFile;
	}
	else
	{
		file = &regFile;
		if (!file->Open(filename))
			return false;
	}

	// Yes, this is hack-ish.

	// If the value is nil, don't write it.
	if (value.IsNil())
		return false;

	// If the variable is user data or a function...
	if (value.IsUserData()  ||  value.IsFunction()  ||  value.IsCFunction())
	{
		// ... only write it if they requested it be written.  But only do
		// it as a comment.
		if ((flags & DUMP_WRITEALL)  &&  name)
		{
			if (value.IsUserData())
			{
				file->Print("-- %s", name);
				file->Print(" = '!!!USERDATA!!!'\r\n");
			}
			else if (value.IsFunction())
			{
				lua_Debug ar;
				value.Push();
				lua_getinfo(*this, ">S", &ar);
//				printf("%d\r\n", ar.linedefined);
				file->Print("-- %s", name);
				file->Print(" = '!!!FUNCTION!!! %s %d'\r\n", ar.source, ar.linedefined);
			}
			else
			{
				file->Print("-- %s", name);
				file->Print(" = '!!!CFUNCTION!!!'\r\n");
			}

			return true;
		}

		return false;
	}

	// Indent the line the number of spaces for the current indentation level.
	const unsigned int INDENT_SIZE = 1;
	const unsigned int indentSpaces = (indentLevel == -1 ? 0 : indentLevel) * INDENT_SIZE;
	if ((unsigned int)indentLevel < maxIndentLevel)
		file->Indent(indentSpaces);
	else
		file->Print(" ");

	// If the object has a name, write it out.
	if (name)
	{
		file->Print("%s = ", name);
	}

	LuaObject key(this);
	bool ret = DumpObject(*file, key, value, flags | 0xF0000000, indentLevel, maxIndentLevel);
	file->Print("\n");
	return ret;
}


/**
	Adds [indentLevel] number of spaces to the file.
**/
void LuaStateOutFile::Indent(unsigned int indentLevel)
{
	// Write out indentation.
	char spaces[500];
	unsigned int i;
	for (i = 0; i < indentLevel; ++i)
		spaces[i] = '\t';
	spaces[i] = 0;
	Print(spaces);
}


/**
	Save the complete script state.
**/
bool LuaState::DumpGlobals(const char* filename, unsigned int flags, unsigned int maxIndentLevel)
{
	// Open the text file to write the script state to.
	LuaStateOutFile file;
	if (!file.Open(filename))
		return false;

	return DumpGlobals(file, flags, maxIndentLevel);
}


/**
	Save the complete script state.
**/
bool LuaState::DumpGlobals(LuaStateOutFile& file, unsigned int flags, unsigned int maxIndentLevel)
{
	LuaObject globalsObj = GetGlobals();

	for (LuaTableIterator it(globalsObj); it; ++it)
	{
		// Don't try and dump the globals table.
		if (!(it.GetValue() == globalsObj))
		{
			// Run through all the globals.
			DumpObject(file, it.GetKey(), it.GetValue(), flags, 0, maxIndentLevel);
		}
	}

	return true;
}

#endif // LUAPLUS_EXTENSIONS

} // namespace LuaPlus

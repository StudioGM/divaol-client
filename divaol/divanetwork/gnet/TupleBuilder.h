/*
 *  gnet.h
 *
 *  Created by tempbuffer on 2/14/13.
 *  Copyright 2013 tempbuffer. All rights reserved.
 *
 */
#ifndef _TUPLE_BUILDER_H_
#define _TUPLE_BUILDER_H_

#include "gnet.h"

namespace gnet
{
	class TupleBuilder
	{
		Item<Tuple> *tuple;
	public:
		TupleBuilder()
		{
			tuple = new Item<Tuple>();
		}
		Item<Tuple>* Get()
		{
			return tuple;
		}
		void Clear() {tuple->clear();}
		TupleBuilder& AddAtom(const Atom &rhs) {*tuple += rhs; return *this;}
		TupleBuilder& AddInt(int32 rhs) {*tuple += rhs; return *this;}
		TupleBuilder& AddInt(uint32 rhs) {*tuple += rhs; return *this;}
		TupleBuilder& AddInt(int64 rhs) {*tuple += rhs; return *this;}
		TupleBuilder& AddInt(uint64 rhs) {*tuple += rhs; return *this;}
		TupleBuilder& AddString(const char* rhs) {*tuple += (Binary)rhs; return *this;}
		TupleBuilder& AddString(const std::string &rhs) {*tuple += (ItemBase*)new Item<Binary>(rhs); return *this;}
		TupleBuilder& AddString(const std::wstring &rhs) {*tuple += (ItemBase*)new Item<Binary>(ItemUtility::ToBytes(rhs), true); return *this;}
		TupleBuilder& AddDouble(double rhs) {*tuple += (double)rhs; return *this;}
		TupleBuilder& AddChar(char rhs) {*tuple += (int8)rhs; return *this;}
		TupleBuilder& AddBoolean(bool rhs) {*tuple += (int8)rhs; return *this;}
		TupleBuilder& AddBytes(Bytes &rhs) {*tuple += (ItemBase*)new Item<Binary>(rhs,true); return *this;}
	};
}

#endif
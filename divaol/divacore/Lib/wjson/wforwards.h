#ifndef WJSON_FORWARDS_H_INCLUDED
# define WJSON_FORWARDS_H_INCLUDED

# include "wconfig.h"

namespace WJson {

   // writer.h
   class FastWriter;
   class StyledWriter;

   // reader.h
   class Reader;

   // features.h
   class Features;

   // value.h
   typedef int Int;
   typedef unsigned int UInt;
   class StaticString;
   class Path;
   class PathArgument;
   class Value;
   class ValueIteratorBase;
   class ValueIterator;
   class ValueConstIterator;
#ifdef WJSON_VALUE_USE_INTERNAL_MAP
   class ValueAllocator;
   class ValueMapAllocator;
   class ValueInternalLink;
   class ValueInternalArray;
   class ValueInternalMap;
#endif // #ifdef WJSON_VALUE_USE_INTERNAL_MAP

} // namespace WJson


#endif // WJSON_FORWARDS_H_INCLUDED

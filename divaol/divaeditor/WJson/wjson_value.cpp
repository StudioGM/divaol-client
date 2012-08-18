#include <iostream>
#include "wvalue.h"
#include "wwriter.h"
#include <utility>
#include <stdexcept>
#include <cstring>
#include <cassert>
#ifdef WJSON_USE_CPPTL
# include <cpptl/conststring.h>
#endif
#include <cstddef>    // size_t
#ifndef WJSON_USE_SIMPLE_INTERNAL_ALLOCATOR
# include "wjson_batchallocator.h"
#endif // #ifndef WJSON_USE_SIMPLE_INTERNAL_ALLOCATOR

#define WJSON_ASSERT_UNREACHABLE assert( false )
#define WJSON_ASSERT( condition ) assert( condition );  // @todo <= change this into an exception throw
#define WJSON_ASSERT_MESSAGE( condition, message ) if (!( condition )) throw std::runtime_error( message );

namespace WJson {

const Value Value::null;
const Int Value::minInt = Int( ~(UInt(-1)/2) );
const Int Value::maxInt = Int( UInt(-1)/2 );
const UInt Value::maxUInt = UInt(-1);

// A "safe" implementation of strdup. Allow null pointer to be passed. 
// Also avoid warning on msvc80.
//
//inline wchar_t *safeStringDup( const wchar_t *czstring )
//{
//   if ( czstring )
//   {
//      const size_t length = (unsigned int)( wcslen(czstring) + 1 );
//      wchar_t *newString = static_cast<wchar_t *>( malloc( length ) );
//      memcpy( newString, czstring, length );
//      return newString;
//   }
//   return 0;
//}
//
//inline wchar_t *safeStringDup( const std::wstring &str )
//{
//   if ( !str.empty() )
//   {
//      const size_t length = str.length();
//      wchar_t *newString = static_cast<wchar_t *>( malloc( length + 1 ) );
//      memcpy( newString, str.c_str(), length );
//      newString[length] = 0;
//      return newString;
//   }
//   return 0;
//}

ValueAllocator::~ValueAllocator()
{
}

class DefaultValueAllocator : public ValueAllocator
{
public:
   virtual ~DefaultValueAllocator()
   {
   }

   virtual wchar_t *makeMemberName( const wchar_t *memberName )
   {
      return duplicateStringValue( memberName );
   }

   virtual void releaseMemberName( wchar_t *memberName )
   {
      releaseStringValue( memberName );
   }

   virtual wchar_t *duplicateStringValue( const wchar_t *value, 
                                       unsigned int length = unknown )
   {
      //@todo invesgate this old optimization
      //if ( !value  ||  value[0] == 0 )
      //   return 0;

      if ( length == unknown )
         length = (unsigned int)wcslen(value);
      wchar_t *newString = static_cast<wchar_t *>( malloc( (length + 1) * sizeof(wchar_t) ) );

	  wcscpy(newString, value);
      //memcpy(newString, value, length );

      newString[length] = 0;
      return newString;
   }

   virtual void releaseStringValue( wchar_t *value )
   {
      if ( value )
         free( value );
   }
};

static ValueAllocator *&valueAllocator()
{
   static DefaultValueAllocator defaultAllocator;
   static ValueAllocator *valueAllocator = &defaultAllocator;
   return valueAllocator;
}

static struct DummyValueAllocatorInitializer {
   DummyValueAllocatorInitializer() 
   {
      valueAllocator();      // ensure valueAllocator() statics are initialized before main().
   }
} dummyValueAllocatorInitializer;



// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// ValueInternals...
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
#ifdef WJSON_VALUE_USE_INTERNAL_MAP
# include "json_internalarray.inl"
# include "json_internalmap.inl"
#endif // WJSON_VALUE_USE_INTERNAL_MAP

# include "wjson_valueiterator.inl"


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::CommentInfo
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////


Value::CommentInfo::CommentInfo()
   : comment_( 0 )
{
}

Value::CommentInfo::~CommentInfo()
{
   if ( comment_ )
      valueAllocator()->releaseStringValue( comment_ );
}


void 
Value::CommentInfo::setComment( const wchar_t *text )
{
   if ( comment_ )
      valueAllocator()->releaseStringValue( comment_ );
   WJSON_ASSERT( text );
   WJSON_ASSERT_MESSAGE( text[0]=='\0' || text[0]=='/', "Comments must start with /");
   // It seems that /**/ style comments are acceptable as well.
   comment_ = valueAllocator()->duplicateStringValue( text );
}


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::CZString
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
# ifndef WJSON_VALUE_USE_INTERNAL_MAP

// Notes: index_ indicates if the string was allocated when
// a string is stored.

Value::CZString::CZString( int index )
   : cstr_( 0 )
   , index_( index )
{
}

Value::CZString::CZString( const wchar_t *cstr, DuplicationPolicy allocate )
   : cstr_( allocate == duplicate ? valueAllocator()->makeMemberName(cstr) 
                                  : cstr )
   , index_( allocate )
{
}

Value::CZString::CZString( const CZString &other )
: cstr_( other.index_ != noDuplication &&  other.cstr_ != 0
                ?  valueAllocator()->makeMemberName( other.cstr_ )
                : other.cstr_ )
   , index_( other.cstr_ ? (other.index_ == noDuplication ? noDuplication : duplicate)
                         : other.index_ )
{
}

Value::CZString::~CZString()
{
   if ( cstr_  &&  index_ == duplicate )
      valueAllocator()->releaseMemberName( const_cast<wchar_t *>( cstr_ ) );
}

void 
Value::CZString::swap( CZString &other )
{
   std::swap( cstr_, other.cstr_ );
   std::swap( index_, other.index_ );
}

Value::CZString &
Value::CZString::operator =( const CZString &other )
{
   CZString temp( other );
   swap( temp );
   return *this;
}

bool 
Value::CZString::operator<( const CZString &other ) const 
{
   if ( cstr_ )
      return wcscmp( cstr_, other.cstr_ ) < 0;
   return index_ < other.index_;
}

bool 
Value::CZString::operator==( const CZString &other ) const 
{
   if ( cstr_ )
      return wcscmp( cstr_, other.cstr_ ) == 0;
   return index_ == other.index_;
}


int 
Value::CZString::index() const
{
   return index_;
}


const wchar_t *
Value::CZString::c_str() const
{
   return cstr_;
}

bool 
Value::CZString::isStaticString() const
{
   return index_ == noDuplication;
}

#endif // ifndef WJSON_VALUE_USE_INTERNAL_MAP


// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// class Value::Value
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////

/*! \internal Default constructor initialization must be equivalent to:
 * memset( this, 0, sizeof(Value) )
 * This optimization is used in ValueInternalMap fast allocator.
 */
Value::Value( ValueType type )
   : type_( type )
   , allocated_( 0 )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   switch ( type )
   {
   case nullValue:
      break;
   case intValue:
   case uintValue:
      value_.int_ = 0;
      break;
   case realValue:
      value_.real_ = 0.0;
      break;
   case stringValue:
      value_.string_ = 0;
      break;
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      value_.map_ = new ObjectValues();
      break;
#else
   case arrayValue:
      value_.array_ = arrayAllocator()->newArray();
      break;
   case objectValue:
      value_.map_ = mapAllocator()->newMap();
      break;
#endif
   case booleanValue:
      value_.bool_ = false;
      break;
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
}


Value::Value( Int value )
   : type_( intValue )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.int_ = value;
}


Value::Value( UInt value )
   : type_( uintValue )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.uint_ = value;
}

Value::Value( double value )
   : type_( realValue )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.real_ = value;
}

Value::Value( const wchar_t *value )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = valueAllocator()->duplicateStringValue( value );
}


Value::Value( const wchar_t *beginValue, 
              const wchar_t *endValue )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = valueAllocator()->duplicateStringValue( beginValue, 
                                                            UInt(endValue - beginValue) );
}


Value::Value( const std::wstring &value )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = valueAllocator()->duplicateStringValue( value.c_str(), 
                                                            (unsigned int)value.length() );

}

Value::Value( const StaticString &value )
   : type_( stringValue )
   , allocated_( false )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = const_cast<wchar_t *>( value.c_str() );
}


# ifdef WJSON_USE_CPPTL
Value::Value( const CppTL::ConstString &value )
   : type_( stringValue )
   , allocated_( true )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.string_ = valueAllocator()->duplicateStringValue( value, value.length() );
}
# endif

Value::Value( bool value )
   : type_( booleanValue )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   value_.bool_ = value;
}


Value::Value( const Value &other )
   : type_( other.type_ )
   , comments_( 0 )
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
   , itemIsUsed_( 0 )
#endif
{
   switch ( type_ )
   {
   case nullValue:
   case intValue:
   case uintValue:
   case realValue:
   case booleanValue:
      value_ = other.value_;
      break;
   case stringValue:
      if ( other.value_.string_ )
      {
         value_.string_ = valueAllocator()->duplicateStringValue( other.value_.string_ );
         allocated_ = true;
      }
      else
         value_.string_ = 0;
      break;
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      value_.map_ = new ObjectValues( *other.value_.map_ );
      break;
#else
   case arrayValue:
      value_.array_ = arrayAllocator()->newArrayCopy( *other.value_.array_ );
      break;
   case objectValue:
      value_.map_ = mapAllocator()->newMapCopy( *other.value_.map_ );
      break;
#endif
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   if ( other.comments_ )
   {
      comments_ = new CommentInfo[numberOfCommentPlacement];
      for ( int comment =0; comment < numberOfCommentPlacement; ++comment )
      {
         const CommentInfo &otherComment = other.comments_[comment];
         if ( otherComment.comment_ )
            comments_[comment].setComment( otherComment.comment_ );
      }
   }
}


Value::~Value()
{
   switch ( type_ )
   {
   case nullValue:
   case intValue:
   case uintValue:
   case realValue:
   case booleanValue:
      break;
   case stringValue:
      if ( allocated_ )
         valueAllocator()->releaseStringValue( value_.string_ );
      break;
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      delete value_.map_;
      break;
#else
   case arrayValue:
      arrayAllocator()->destructArray( value_.array_ );
      break;
   case objectValue:
      mapAllocator()->destructMap( value_.map_ );
      break;
#endif
   default:
      WJSON_ASSERT_UNREACHABLE;
   }

   if ( comments_ )
      delete[] comments_;
}

Value &
Value::operator=( const Value &other )
{
   Value temp( other );
   swap( temp );
   return *this;
}

void 
Value::swap( Value &other )
{
   ValueType temp = type_;
   type_ = other.type_;
   other.type_ = temp;
   std::swap( value_, other.value_ );
   int temp2 = allocated_;
   allocated_ = other.allocated_;
   other.allocated_ = temp2;
}

ValueType 
Value::type() const
{
   return type_;
}


int 
Value::compare( const Value &other )
{
   /*
   int typeDelta = other.type_ - type_;
   switch ( type_ )
   {
   case nullValue:

      return other.type_ == type_;
   case intValue:
      if ( other.type_.isNumeric()
   case uintValue:
   case realValue:
   case booleanValue:
      break;
   case stringValue,
      break;
   case arrayValue:
      delete value_.array_;
      break;
   case objectValue:
      delete value_.map_;
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   */
   return 0;  // unreachable
}

bool 
Value::operator <( const Value &other ) const
{
   int typeDelta = type_ - other.type_;
   if ( typeDelta )
      return typeDelta < 0 ? true : false;
   switch ( type_ )
   {
   case nullValue:
      return false;
   case intValue:
      return value_.int_ < other.value_.int_;
   case uintValue:
      return value_.uint_ < other.value_.uint_;
   case realValue:
      return value_.real_ < other.value_.real_;
   case booleanValue:
      return value_.bool_ < other.value_.bool_;
   case stringValue:
      return ( value_.string_ == 0  &&  other.value_.string_ )
             || ( other.value_.string_  
                  &&  value_.string_  
                  && wcscmp( value_.string_, other.value_.string_ ) < 0 );
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      {
         int delta = int( value_.map_->size() - other.value_.map_->size() );
         if ( delta )
            return delta < 0;
         return (*value_.map_) < (*other.value_.map_);
      }
#else
   case arrayValue:
      return value_.array_->compare( *(other.value_.array_) ) < 0;
   case objectValue:
      return value_.map_->compare( *(other.value_.map_) ) < 0;
#endif
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return 0;  // unreachable
}

bool 
Value::operator <=( const Value &other ) const
{
   return !(other > *this);
}

bool 
Value::operator >=( const Value &other ) const
{
   return !(*this < other);
}

bool 
Value::operator >( const Value &other ) const
{
   return other < *this;
}

bool 
Value::operator ==( const Value &other ) const
{
   //if ( type_ != other.type_ )
   // GCC 2.95.3 says:
   // attempt to take address of bit-field structure member `WJson::Value::type_'
   // Beats me, but a temp solves the problem.
   int temp = other.type_;
   if ( type_ != temp )
      return false;
   switch ( type_ )
   {
   case nullValue:
      return true;
   case intValue:
      return value_.int_ == other.value_.int_;
   case uintValue:
      return value_.uint_ == other.value_.uint_;
   case realValue:
      return value_.real_ == other.value_.real_;
   case booleanValue:
      return value_.bool_ == other.value_.bool_;
   case stringValue:
      return ( value_.string_ == other.value_.string_ )
             || ( other.value_.string_  
                  &&  value_.string_  
                  && wcscmp( value_.string_, other.value_.string_ ) == 0 );
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      return value_.map_->size() == other.value_.map_->size()
             && (*value_.map_) == (*other.value_.map_);
#else
   case arrayValue:
      return value_.array_->compare( *(other.value_.array_) ) == 0;
   case objectValue:
      return value_.map_->compare( *(other.value_.map_) ) == 0;
#endif
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return 0;  // unreachable
}

bool 
Value::operator !=( const Value &other ) const
{
   return !( *this == other );
}

const wchar_t *
Value::asCString() const
{
   WJSON_ASSERT( type_ == stringValue );
   return value_.string_;
}


std::wstring 
Value::asString() const
{
   switch ( type_ )
   {
   case nullValue:
      return L"";
   case stringValue:
      return value_.string_ ? value_.string_ : L"";
   case booleanValue:
      return value_.bool_ ? L"true" : L"false";
   case intValue:
   case uintValue:
   case realValue:
   case arrayValue:
   case objectValue:
      WJSON_ASSERT_MESSAGE( false, "Type is not convertible to string" );
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return L""; // unreachable
}

# ifdef WJSON_USE_CPPTL
CppTL::ConstString 
Value::asConstString() const
{
   return CppTL::ConstString( asString().c_str() );
}
# endif

Value::Int 
Value::asInt() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0;
   case intValue:
      return value_.int_;
   case uintValue:
      WJSON_ASSERT_MESSAGE( value_.uint_ < (unsigned)maxInt, "integer out of signed integer range" );
      return value_.uint_;
   case realValue:
      WJSON_ASSERT_MESSAGE( value_.real_ >= minInt  &&  value_.real_ <= maxInt, "Real out of signed integer range" );
      return Int( value_.real_ );
   case booleanValue:
      return value_.bool_ ? 1 : 0;
   case stringValue:
   case arrayValue:
   case objectValue:
      WJSON_ASSERT_MESSAGE( false, "Type is not convertible to int" );
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}

Value::UInt 
Value::asUInt() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0;
   case intValue:
      WJSON_ASSERT_MESSAGE( value_.int_ >= 0, "Negative integer can not be converted to unsigned integer" );
      return value_.int_;
   case uintValue:
      return value_.uint_;
   case realValue:
      WJSON_ASSERT_MESSAGE( value_.real_ >= 0  &&  value_.real_ <= maxUInt,  "Real out of unsigned integer range" );
      return UInt( value_.real_ );
   case booleanValue:
      return value_.bool_ ? 1 : 0;
   case stringValue:
   case arrayValue:
   case objectValue:
      WJSON_ASSERT_MESSAGE( false, "Type is not convertible to uint" );
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}

double 
Value::asDouble() const
{
   switch ( type_ )
   {
   case nullValue:
      return 0.0;
   case intValue:
      return value_.int_;
   case uintValue:
      return value_.uint_;
   case realValue:
      return value_.real_;
   case booleanValue:
      return value_.bool_ ? 1.0 : 0.0;
   case stringValue:
   case arrayValue:
   case objectValue:
      WJSON_ASSERT_MESSAGE( false, "Type is not convertible to double" );
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}

bool 
Value::asBool() const
{
   switch ( type_ )
   {
   case nullValue:
      return false;
   case intValue:
   case uintValue:
      return value_.int_ != 0;
   case realValue:
      return value_.real_ != 0.0;
   case booleanValue:
      return value_.bool_;
   case stringValue:
      return value_.string_  &&  value_.string_[0] != 0;
   case arrayValue:
   case objectValue:
      return value_.map_->size() != 0;
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return false; // unreachable;
}


bool 
Value::isConvertibleTo( ValueType other ) const
{
   switch ( type_ )
   {
   case nullValue:
      return true;
   case intValue:
      return ( other == nullValue  &&  value_.int_ == 0 )
             || other == intValue
             || ( other == uintValue  && value_.int_ >= 0 )
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case uintValue:
      return ( other == nullValue  &&  value_.uint_ == 0 )
             || ( other == intValue  && value_.uint_ <= (unsigned)maxInt )
             || other == uintValue
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case realValue:
      return ( other == nullValue  &&  value_.real_ == 0.0 )
             || ( other == intValue  &&  value_.real_ >= minInt  &&  value_.real_ <= maxInt )
             || ( other == uintValue  &&  value_.real_ >= 0  &&  value_.real_ <= maxUInt )
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case booleanValue:
      return ( other == nullValue  &&  value_.bool_ == false )
             || other == intValue
             || other == uintValue
             || other == realValue
             || other == stringValue
             || other == booleanValue;
   case stringValue:
      return other == stringValue
             || ( other == nullValue  &&  (!value_.string_  ||  value_.string_[0] == 0) );
   case arrayValue:
      return other == arrayValue
             ||  ( other == nullValue  &&  value_.map_->size() == 0 );
   case objectValue:
      return other == objectValue
             ||  ( other == nullValue  &&  value_.map_->size() == 0 );
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return false; // unreachable;
}


/// Number of values in array or object
Value::UInt 
Value::size() const
{
   switch ( type_ )
   {
   case nullValue:
   case intValue:
   case uintValue:
   case realValue:
   case booleanValue:
   case stringValue:
      return 0;
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:  // size of the array is highest index + 1
      if ( !value_.map_->empty() )
      {
         ObjectValues::const_iterator itLast = value_.map_->end();
         --itLast;
         return (*itLast).first.index()+1;
      }
      return 0;
   case objectValue:
      return Int( value_.map_->size() );
#else
   case arrayValue:
      return Int( value_.array_->size() );
   case objectValue:
      return Int( value_.map_->size() );
#endif
   default:
      WJSON_ASSERT_UNREACHABLE;
   }
   return 0; // unreachable;
}


bool 
Value::empty() const
{
   if ( isNull() || isArray() || isObject() )
      return size() == 0u;
   else
      return false;
}


bool
Value::operator!() const
{
   return isNull();
}


void 
Value::clear()
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue  || type_ == objectValue );

   switch ( type_ )
   {
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
   case objectValue:
      value_.map_->clear();
      break;
#else
   case arrayValue:
      value_.array_->clear();
      break;
   case objectValue:
      value_.map_->clear();
      break;
#endif
   default:
      break;
   }
}

void 
Value::resize( UInt newSize )
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue );
   if ( type_ == nullValue )
      *this = Value( arrayValue );
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   UInt oldSize = size();
   if ( newSize == 0 )
      clear();
   else if ( newSize > oldSize )
      (*this)[ newSize - 1 ];
   else
   {
      for ( UInt index = newSize; index < oldSize; ++index )
         value_.map_->erase( index );
      assert( size() == newSize );
   }
#else
   value_.array_->resize( newSize );
#endif
}


Value &
Value::operator[]( UInt index )
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue );
   if ( type_ == nullValue )
      *this = Value( arrayValue );
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   CZString key( index );
   ObjectValues::iterator it = value_.map_->lower_bound( key );
    if ( it != value_.map_->end()  &&  (*it).first == key ) {
        Value& val = const_cast<Value&>(it->second);
        val.parent_ = this;
        return val;
    }

   ObjectValues::value_type defaultValue( key, null );
   it = value_.map_->insert( it, defaultValue );
    it->second.parent_ = this;
   return (*it).second;
#else
   return value_.array_->resolveReference( index );
#endif
}

    const Value* Value::parent() const {
        if(parent_)
            return parent_;
        return &null;
    }

const Value &
Value::operator[]( UInt index ) const
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == arrayValue );
   if ( type_ == nullValue )
      return null;
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   CZString key( index );
   ObjectValues::const_iterator it = value_.map_->find( key );
   if ( it == value_.map_->end() )
      return null;
    Value& val = const_cast<Value&>(it->second);
    val.parent_ = this;
    return val;
#else
   Value *value = value_.array_->find( index );
    if(value)
        value->parent_ = this;
   return value ? *value : null;
#endif
}


Value &
Value::operator[]( const wchar_t *key )
{
   return resolveReference( key, false );
}


Value &
Value::resolveReference( const wchar_t *key, 
                         bool isStatic )
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
      *this = Value( objectValue );
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   CZString actualKey( key, isStatic ? CZString::noDuplication 
                                     : CZString::duplicateOnCopy );
   ObjectValues::iterator it = value_.map_->lower_bound( actualKey );
   if ( it != value_.map_->end()  &&  (*it).first == actualKey )
      return (*it).second;

   ObjectValues::value_type defaultValue( actualKey, null );
   it = value_.map_->insert( it, defaultValue );
   Value &value = (*it).second;
    value.parent_ = this;
   return value;
#else
   return value_.map_->resolveReference( key, isStatic );
#endif
}


Value 
Value::get( UInt index, 
            const Value &defaultValue ) const
{
    Value *value = const_cast<Value*>(&((*this)[index]));
    if(value)
        value->parent_ = this;
   return value == &null ? defaultValue : *value;
}


bool 
Value::isValidIndex( UInt index ) const
{
   return index < size();
}



const Value &
Value::operator[]( const wchar_t *key ) const
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
      return null;
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   CZString actualKey( key, CZString::noDuplication );
   ObjectValues::const_iterator it = value_.map_->find( actualKey );
   if ( it == value_.map_->end() )
      return null;
    Value& val = const_cast<Value&>(it->second);
    val.parent_ = this;
    return val;
#else
   const Value *value = value_.map_->find( key );
    if(value)
        value->parent_ = this;
   return value ? *value : null;
#endif
}


Value &
Value::operator[]( const std::wstring &key )
{
   return (*this)[ key.c_str() ];
}


const Value &
Value::operator[]( const std::wstring &key ) const
{
   return (*this)[ key.c_str() ];
}

Value &
Value::operator[]( const StaticString &key )
{
   return resolveReference( key, true );
}


# ifdef WJSON_USE_CPPTL
Value &
Value::operator[]( const CppTL::ConstString &key )
{
   return (*this)[ key.c_str() ];
}


const Value &
Value::operator[]( const CppTL::ConstString &key ) const
{
   return (*this)[ key.c_str() ];
}
# endif


Value &
Value::append( const Value &value )
{
   return (*this)[size()] = value;
}


Value 
Value::get( const wchar_t *key, 
            const Value &defaultValue ) const
{
    Value *value = const_cast<Value*>(&((*this)[key]));
    if(value)
        value->parent_ = this;
   return value == &null ? defaultValue : *value;
}


Value 
Value::get( const std::wstring &key,
            const Value &defaultValue ) const
{
   return get( key.c_str(), defaultValue );
}

Value
Value::removeMember( const wchar_t* key )
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
      return null;
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   CZString actualKey( key, CZString::noDuplication );
   ObjectValues::iterator it = value_.map_->find( actualKey );
   if ( it == value_.map_->end() )
      return null;
   Value old(it->second);
   value_.map_->erase(it);
   return old;
#else
   Value *value = value_.map_->find( key );
   if (value){
      Value old(*value);
      value_.map_.remove( key );
      return old;
   } else {
      return null;
   }
#endif
}

Value
Value::removeMember( const std::wstring &key )
{
   return removeMember( key.c_str() );
}

# ifdef WJSON_USE_CPPTL
Value 
Value::get( const CppTL::ConstString &key,
            const Value &defaultValue ) const
{
   return get( key.c_str(), defaultValue );
}
# endif

bool 
Value::isMember( const wchar_t *key ) const
{
   const Value *value = &((*this)[key]);
   return value != &null;
}


bool 
Value::isMember( const std::wstring &key ) const
{
   return isMember( key.c_str() );
}


# ifdef WJSON_USE_CPPTL
bool 
Value::isMember( const CppTL::ConstString &key ) const
{
   return isMember( key.c_str() );
}
#endif

Value::Members 
Value::getMemberNames() const
{
   WJSON_ASSERT( type_ == nullValue  ||  type_ == objectValue );
   if ( type_ == nullValue )
       return Value::Members();
   Members members;
   members.reserve( value_.map_->size() );
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
   ObjectValues::const_iterator it = value_.map_->begin();
   ObjectValues::const_iterator itEnd = value_.map_->end();
   for ( ; it != itEnd; ++it )
      members.push_back( std::wstring( (*it).first.c_str() ) );
#else
   ValueInternalMap::IteratorState it;
   ValueInternalMap::IteratorState itEnd;
   value_.map_->makeBeginIterator( it );
   value_.map_->makeEndIterator( itEnd );
   for ( ; !ValueInternalMap::equals( it, itEnd ); ValueInternalMap::increment(it) )
      members.push_back( std::wstring( ValueInternalMap::key( it ) ) );
#endif
   return members;
}
//
//# ifdef WJSON_USE_CPPTL
//EnumMemberNames
//Value::enumMemberNames() const
//{
//   if ( type_ == objectValue )
//   {
//      return CppTL::Enum::any(  CppTL::Enum::transform(
//         CppTL::Enum::keys( *(value_.map_), CppTL::Type<const CZString &>() ),
//         MemberNamesTransform() ) );
//   }
//   return EnumMemberNames();
//}
//
//
//EnumValues 
//Value::enumValues() const
//{
//   if ( type_ == objectValue  ||  type_ == arrayValue )
//      return CppTL::Enum::anyValues( *(value_.map_), 
//                                     CppTL::Type<const Value &>() );
//   return EnumValues();
//}
//
//# endif


bool
Value::isNull() const
{
   return type_ == nullValue;
}


bool 
Value::isBool() const
{
   return type_ == booleanValue;
}


bool 
Value::isInt() const
{
   return type_ == intValue;
}


bool 
Value::isUInt() const
{
   return type_ == uintValue;
}


bool 
Value::isIntegral() const
{
   return type_ == intValue  
          ||  type_ == uintValue  
          ||  type_ == booleanValue;
}


bool 
Value::isDouble() const
{
   return type_ == realValue;
}


bool 
Value::isNumeric() const
{
   return isIntegral() || isDouble();
}


bool 
Value::isString() const
{
   return type_ == stringValue;
}


bool 
Value::isArray() const
{
   return type_ == nullValue  ||  type_ == arrayValue;
}


bool 
Value::isObject() const
{
   return type_ == nullValue  ||  type_ == objectValue;
}


void 
Value::setComment( const wchar_t *comment,
                   CommentPlacement placement )
{
   if ( !comments_ )
      comments_ = new CommentInfo[numberOfCommentPlacement];
   comments_[placement].setComment( comment );
}


void 
Value::setComment( const std::wstring &comment,
                   CommentPlacement placement )
{
   setComment( comment.c_str(), placement );
}


bool 
Value::hasComment( CommentPlacement placement ) const
{
   return comments_ != 0  &&  comments_[placement].comment_ != 0;
}

std::wstring 
Value::getComment( CommentPlacement placement ) const
{
   if ( hasComment(placement) )
      return comments_[placement].comment_;
   return L"";
}


std::wstring 
Value::toStyledString() const
{
   StyledWriter writer;
   return writer.write( *this );
}


Value::const_iterator 
Value::begin() const
{
   switch ( type_ )
   {
#ifdef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeBeginIterator( it );
         return const_iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeBeginIterator( it );
         return const_iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return const_iterator( value_.map_->begin() );
      break;
#endif
   default:
      break;
   }
   return const_iterator();
}

Value::const_iterator 
Value::end() const
{
   switch ( type_ )
   {
#ifdef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeEndIterator( it );
         return const_iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeEndIterator( it );
         return const_iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return const_iterator( value_.map_->end() );
      break;
#endif
   default:
      break;
   }
   return const_iterator();
}


Value::iterator 
Value::begin()
{
   switch ( type_ )
   {
#ifdef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeBeginIterator( it );
         return iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeBeginIterator( it );
         return iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return iterator( value_.map_->begin() );
      break;
#endif
   default:
      break;
   }
   return iterator();
}

Value::iterator 
Value::end()
{
   switch ( type_ )
   {
#ifdef WJSON_VALUE_USE_INTERNAL_MAP
   case arrayValue:
      if ( value_.array_ )
      {
         ValueInternalArray::IteratorState it;
         value_.array_->makeEndIterator( it );
         return iterator( it );
      }
      break;
   case objectValue:
      if ( value_.map_ )
      {
         ValueInternalMap::IteratorState it;
         value_.map_->makeEndIterator( it );
         return iterator( it );
      }
      break;
#else
   case arrayValue:
   case objectValue:
      if ( value_.map_ )
         return iterator( value_.map_->end() );
      break;
#endif
   default:
      break;
   }
   return iterator();
}


// class PathArgument
// //////////////////////////////////////////////////////////////////

PathArgument::PathArgument()
   : kind_( kindNone )
{
}


PathArgument::PathArgument( Value::UInt index )
   : index_( index )
   , kind_( kindIndex )
{
}


PathArgument::PathArgument( const wchar_t *key )
   : key_( key )
   , kind_( kindKey )
{
}


PathArgument::PathArgument( const std::wstring &key )
   : key_( key.c_str() )
   , kind_( kindKey )
{
}

// class Path
// //////////////////////////////////////////////////////////////////

Path::Path( const std::wstring &path,
            const PathArgument &a1,
            const PathArgument &a2,
            const PathArgument &a3,
            const PathArgument &a4,
            const PathArgument &a5 )
{
   InArgs in;
   in.push_back( &a1 );
   in.push_back( &a2 );
   in.push_back( &a3 );
   in.push_back( &a4 );
   in.push_back( &a5 );
   makePath( path, in );
}


void 
Path::makePath( const std::wstring &path,
                const InArgs &in )
{
   const wchar_t *current = path.c_str();
   const wchar_t *end = current + path.length();
   InArgs::const_iterator itInArg = in.begin();
   while ( current != end )
   {
      if ( *current == '[' )
      {
         ++current;
         if ( *current == '%' )
            addPathInArg( path, in, itInArg, PathArgument::kindIndex );
         else
         {
            Value::UInt index = 0;
            for ( ; current != end && *current >= '0'  &&  *current <= '9'; ++current )
               index = index * 10 + Value::UInt(*current - '0');
            args_.push_back( index );
         }
         if ( current == end  ||  *current++ != ']' )
            invalidPath( path, int(current - path.c_str()) );
      }
      else if ( *current == '%' )
      {
         addPathInArg( path, in, itInArg, PathArgument::kindKey );
         ++current;
      }
      else if ( *current == '.' )
      {
         ++current;
      }
      else
      {
         const wchar_t *beginName = current;
         while ( current != end  &&  !strchr( "[.", *current ) )
            ++current;
         args_.push_back( std::wstring( beginName, current ) );
      }
   }
}


void 
Path::addPathInArg( const std::wstring &path, 
                    const InArgs &in, 
                    InArgs::const_iterator &itInArg, 
                    PathArgument::Kind kind )
{
   if ( itInArg == in.end() )
   {
      // Error: missing argument %d
   }
   else if ( (*itInArg)->kind_ != kind )
   {
      // Error: bad argument type
   }
   else
   {
      args_.push_back( **itInArg );
   }
}


void 
Path::invalidPath( const std::wstring &path, 
                   int location )
{
   // Error: invalid path.
}


const Value &
Path::resolve( const Value &root ) const
{
   const Value *node = &root;
   for ( Args::const_iterator it = args_.begin(); it != args_.end(); ++it )
   {
      const PathArgument &arg = *it;
      if ( arg.kind_ == PathArgument::kindIndex )
      {
         if ( !node->isArray()  ||  node->isValidIndex( arg.index_ ) )
         {
            // Error: unable to resolve path (array value expected at position...
         }
         node = &((*node)[arg.index_]);
      }
      else if ( arg.kind_ == PathArgument::kindKey )
      {
         if ( !node->isObject() )
         {
            // Error: unable to resolve path (object value expected at position...)
         }
         node = &((*node)[arg.key_]);
         if ( node == &Value::null )
         {
            // Error: unable to resolve path (object has no member named '' at position...)
         }
      }
   }
   return *node;
}


Value 
Path::resolve( const Value &root, 
               const Value &defaultValue ) const
{
   const Value *node = &root;
   for ( Args::const_iterator it = args_.begin(); it != args_.end(); ++it )
   {
      const PathArgument &arg = *it;
      if ( arg.kind_ == PathArgument::kindIndex )
      {
         if ( !node->isArray()  ||  node->isValidIndex( arg.index_ ) )
            return defaultValue;
         node = &((*node)[arg.index_]);
      }
      else if ( arg.kind_ == PathArgument::kindKey )
      {
         if ( !node->isObject() )
            return defaultValue;
         node = &((*node)[arg.key_]);
         if ( node == &Value::null )
            return defaultValue;
      }
   }
   return *node;
}


Value &
Path::make( Value &root ) const
{
   Value *node = &root;
   for ( Args::const_iterator it = args_.begin(); it != args_.end(); ++it )
   {
      const PathArgument &arg = *it;
      if ( arg.kind_ == PathArgument::kindIndex )
      {
         if ( !node->isArray() )
         {
            // Error: node is not an array at position ...
         }
         node = &((*node)[arg.index_]);
      }
      else if ( arg.kind_ == PathArgument::kindKey )
      {
         if ( !node->isObject() )
         {
            // Error: node is not an object at position...
         }
         node = &((*node)[arg.key_]);
      }
   }
   return *node;
}


} // namespace WJson

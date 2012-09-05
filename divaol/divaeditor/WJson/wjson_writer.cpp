#include "wwriter.h"
#include "wvalue.h"
#include "wforwards.h"
#include <utility>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#if _MSC_VER >= 1400 // VC++ 8.0
#pragma warning( disable : 4996 )   // disable warning about strdup being deprecated.
#endif

namespace WJson {

static bool isControlCharacter(wchar_t ch)
{
   return ch > 0 && ch <= 0x1F;
}

static bool containsControlCharacter( const wchar_t* str )
{
   while ( *str ) 
   {
      if ( isControlCharacter( *(str++) ) )
         return true;
   }
   return false;
}
static void uintToString( unsigned int value, 
                          wchar_t *&current )
{
   *--current = 0;
   do
   {
      *--current = (value % 10) + L'0';
      value /= 10;
   }
   while ( value != 0 );
}

std::wstring valueToString( Int value )
{
   wchar_t buffer[32];
   wchar_t *current = buffer + sizeof(buffer)/sizeof(wchar_t);
   bool isNegative = value < 0;
   if ( isNegative )
      value = -value;
   uintToString( UInt(value), current );
   if ( isNegative )
      *--current = L'-';
   assert( current >= buffer );
   return current;
}


std::wstring valueToString( UInt value )
{
   wchar_t buffer[32];
   wchar_t *current = buffer + sizeof(buffer)/sizeof(wchar_t);
   uintToString( value, current );
   assert( current >= buffer );
   return current;
}

std::wstring valueToString( double value )
{
	std::wostringstream retStream;
	retStream.precision(16);
	retStream<<value;

	std::wstring ret = retStream.str();
	if(ret.find(L'.')==std::wstring::npos)
		ret = ret + L".0";

   return ret;
}


std::wstring valueToString( bool value )
{
   return value ? L"true" : L"false";
}

std::wstring valueToQuotedString( const wchar_t *value )
{
   // Not sure how to handle unicode...
   if (wcspbrk(value, L"\"\\\b\f\n\r\t") == NULL && !containsControlCharacter( value ))
      return std::wstring(L"\"") + value + L"\"";
   // We have to walk value and escape any special characters.
   // Appending to std::wstring is not efficient, but this should be rare.
   // (Note: forward slashes are *not* rare, but I am not escaping them.)
   unsigned maxsize = wcslen(value)*2 + 3; // allescaped+quotes+NULL
   std::wstring result;
   result.reserve(maxsize); // to avoid lots of mallocs
   result += L"\"";
   for (const wchar_t* c=value; *c != 0; ++c)
   {
      switch(*c)
      {
         case L'\"':
            result += L"\\\"";
            break;
         case L'\\':
            result += L"\\\\";
            break;
         case L'\b':
            result += L"\\b";
            break;
         case L'\f':
            result += L"\\f";
            break;
         case L'\n':
            result += L"\\n";
            break;
         case L'\r':
            result += L"\\r";
            break;
         case L'\t':
            result += L"\\t";
            break;
         //case L'/':
            // Even though \/ is considered a legal escape in JSON, a bare
            // slash is also legal, so I see no reason to escape it.
            // (I hope I am not misunderstanding something.
            // blep notes: actually escaping \/ may be useful in javascript to avoid </ 
            // sequence.
            // Should add a flag to allow this compatibility mode and prevent this 
            // sequence from occurring.
         default:
            if ( isControlCharacter( *c ) )
            {
               std::wostringstream oss;
               oss << L"\\u" << std::hex << std::uppercase << std::setfill(L'0') << std::setw(4) << static_cast<int>(*c);
               result += oss.str();
            }
            else
            {
               result += *c;
            }
            break;
      }
   }
   result += L"\"";
   return result;
}

// Class Writer
// //////////////////////////////////////////////////////////////////
Writer::~Writer()
{
}


// Class FastWriter
// //////////////////////////////////////////////////////////////////

FastWriter::FastWriter()
   : yamlCompatiblityEnabled_( false )
{
}


void 
FastWriter::enableYAMLCompatibility()
{
   yamlCompatiblityEnabled_ = true;
}


std::wstring 
FastWriter::write( const Value &root )
{
   document_ = L"";
   writeValue( root );
   document_ += L"\n";
   return document_;
}


void 
FastWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      document_ += L"null";
      break;
   case intValue:
      document_ += valueToString( value.asInt() );
      break;
   case uintValue:
      document_ += valueToString( value.asUInt() );
      break;
   case realValue:
      document_ += valueToString( value.asDouble() );
      break;
   case stringValue:
      document_ += valueToQuotedString( value.asCString() );
      break;
   case booleanValue:
      document_ += valueToString( value.asBool() );
      break;
   case arrayValue:
      {
         document_ += L"[";
         int size = value.size();
         for ( int index =0; index < size; ++index )
         {
            if ( index > 0 )
               document_ += L",";
            writeValue( value[index] );
         }
         document_ += L"]";
      }
      break;
   case objectValue:
      {
         Value::Members members( value.getMemberNames() );
         document_ += L"{";
         for ( Value::Members::iterator it = members.begin(); 
               it != members.end(); 
               ++it )
         {
            const std::wstring &name = *it;
            if ( it != members.begin() )
               document_ += L",";
            document_ += valueToQuotedString( name.c_str() );
            document_ += yamlCompatiblityEnabled_ ? L": " 
                                                  : L":";
            writeValue( value[name] );
         }
         document_ += L"}";
      }
      break;
   }
}


// Class StyledWriter
// //////////////////////////////////////////////////////////////////

StyledWriter::StyledWriter()
   : rightMargin_( 74 )
   , indentSize_( 3 )
{
}


std::wstring 
StyledWriter::write( const Value &root )
{
   document_ = L"";
   addChildValues_ = false;
   indentString_ = L"";
   writeCommentBeforeValue( root );
   writeValue( root );
   writeCommentAfterValueOnSameLine( root );
   document_ += L"\n";
   return document_;
}


void 
StyledWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      pushValue( L"null" );
      break;
   case intValue:
      pushValue( valueToString( value.asInt() ) );
      break;
   case uintValue:
      pushValue( valueToString( value.asUInt() ) );
      break;
   case realValue:
      pushValue( valueToString( value.asDouble() ) );
      break;
   case stringValue:
      pushValue( valueToQuotedString( value.asCString() ) );
      break;
   case booleanValue:
      pushValue( valueToString( value.asBool() ) );
      break;
   case arrayValue:
      writeArrayValue( value);
      break;
   case objectValue:
      {
         Value::Members members( value.getMemberNames() );
         if ( members.empty() )
            pushValue( L"{}" );
         else
         {
            writeWithIndent( L"{" );
            indent();
            Value::Members::iterator it = members.begin();
            while ( true )
            {
               const std::wstring &name = *it;
               const Value &childValue = value[name];
               writeCommentBeforeValue( childValue );
               writeWithIndent( valueToQuotedString( name.c_str() ) );
               document_ += L" : ";
               writeValue( childValue );
               if ( ++it == members.end() )
               {
                  writeCommentAfterValueOnSameLine( childValue );
                  break;
               }
               document_ += L",";
               writeCommentAfterValueOnSameLine( childValue );
            }
            unindent();
            writeWithIndent( L"}" );
         }
      }
      break;
   }
}


void 
StyledWriter::writeArrayValue( const Value &value )
{
   unsigned size = value.size();
   if ( size == 0 )
      pushValue( L"[]" );
   else
   {
      bool isArrayMultiLine = isMultineArray( value );
      if ( isArrayMultiLine )
      {
         writeWithIndent( L"[" );
         indent();
         bool hasChildValue = !childValues_.empty();
         unsigned index =0;
         while ( true )
         {
            const Value &childValue = value[index];
            writeCommentBeforeValue( childValue );
            if ( hasChildValue )
               writeWithIndent( childValues_[index] );
            else
            {
               writeIndent();
               writeValue( childValue );
            }
            if ( ++index == size )
            {
               writeCommentAfterValueOnSameLine( childValue );
               break;
            }
            document_ += L",";
            writeCommentAfterValueOnSameLine( childValue );
         }
         unindent();
         writeWithIndent( L"]" );
      }
      else // output on a single line
      {
         assert( childValues_.size() == size );
         document_ += L"[ ";
         for ( unsigned index =0; index < size; ++index )
         {
            if ( index > 0 )
               document_ += L", ";
            document_ += childValues_[index];
         }
         document_ += L" ]";
      }
   }
}


bool 
StyledWriter::isMultineArray( const Value &value )
{
   int size = value.size();
   bool isMultiLine = size*3 >= rightMargin_ ;
   childValues_.clear();
   for ( int index =0; index < size  &&  !isMultiLine; ++index )
   {
      const Value &childValue = value[index];
      isMultiLine = isMultiLine  ||
                     ( (childValue.isArray()  ||  childValue.isObject())  &&  
                        childValue.size() > 0 );
   }
   if ( !isMultiLine ) // check if line length > max line length
   {
      childValues_.reserve( size );
      addChildValues_ = true;
      int lineLength = 4 + (size-1)*2; // L'[ ' + ', '*n + ' ]'
      for ( int index =0; index < size  &&  !isMultiLine; ++index )
      {
         writeValue( value[index] );
         lineLength += int( childValues_[index].length() );
         isMultiLine = isMultiLine  &&  hasCommentForValue( value[index] );
      }
      addChildValues_ = false;
      isMultiLine = isMultiLine  ||  lineLength >= rightMargin_;
   }
   return isMultiLine;
}


void 
StyledWriter::pushValue( const std::wstring &value )
{
   if ( addChildValues_ )
      childValues_.push_back( value );
   else
      document_ += value;
}


void 
StyledWriter::writeIndent()
{
   if ( !document_.empty() )
   {
      wchar_t last = document_[document_.length()-1];
      if ( last == L' ' )     // already indented
         return;
      if ( last != L'\n' )    // Comments may add new-line
         document_ += L'\n';
   }
   document_ += indentString_;
}


void 
StyledWriter::writeWithIndent( const std::wstring &value )
{
   writeIndent();
   document_ += value;
}


void 
StyledWriter::indent()
{
   indentString_ += std::wstring( indentSize_, L' ' );
}


void 
StyledWriter::unindent()
{
   assert( int(indentString_.size()) >= indentSize_ );
   indentString_.resize( indentString_.size() - indentSize_ );
}


void 
StyledWriter::writeCommentBeforeValue( const Value &root )
{
   if ( !root.hasComment( commentBefore ) )
      return;
   document_ += normalizeEOL( root.getComment( commentBefore ) );
   document_ += L"\n";
}


void 
StyledWriter::writeCommentAfterValueOnSameLine( const Value &root )
{
   if ( root.hasComment( commentAfterOnSameLine ) )
      document_ += L" " + normalizeEOL( root.getComment( commentAfterOnSameLine ) );

   if ( root.hasComment( commentAfter ) )
   {
      document_ += L"\n";
      document_ += normalizeEOL( root.getComment( commentAfter ) );
      document_ += L"\n";
   }
}


bool 
StyledWriter::hasCommentForValue( const Value &value )
{
   return value.hasComment( commentBefore )
          ||  value.hasComment( commentAfterOnSameLine )
          ||  value.hasComment( commentAfter );
}


std::wstring 
StyledWriter::normalizeEOL( const std::wstring &text )
{
   std::wstring normalized;
   normalized.reserve( text.length() );
   const wchar_t *begin = text.c_str();
   const wchar_t *end = begin + text.length();
   const wchar_t *current = begin;
   while ( current != end )
   {
      wchar_t c = *current++;
      if ( c == L'\r' ) // mac or dos EOL
      {
         if ( *current == L'\n' ) // convert dos EOL
            ++current;
         normalized += L'\n';
      }
      else // handle unix EOL & other wchar_t
         normalized += c;
   }
   return normalized;
}


// Class StyledStreamWriter
// //////////////////////////////////////////////////////////////////

StyledStreamWriter::StyledStreamWriter( std::wstring indentation )
   : document_(NULL)
   , rightMargin_( 74 )
   , indentation_( indentation )
{
}


void
StyledStreamWriter::write( std::wostream &out, const Value &root )
{
   document_ = &out;
   addChildValues_ = false;
   indentString_ = L"";
   writeCommentBeforeValue( root );
   writeValue( root );
   writeCommentAfterValueOnSameLine( root );
   *document_ << L"\n";
   document_ = NULL; // Forget the stream, for safety.
}


void 
StyledStreamWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      pushValue( L"null" );
      break;
   case intValue:
      pushValue( valueToString( value.asInt() ) );
      break;
   case uintValue:
      pushValue( valueToString( value.asUInt() ) );
      break;
   case realValue:
      pushValue( valueToString( value.asDouble() ) );
      break;
   case stringValue:
      pushValue( valueToQuotedString( value.asCString() ) );
      break;
   case booleanValue:
      pushValue( valueToString( value.asBool() ) );
      break;
   case arrayValue:
      writeArrayValue( value);
      break;
   case objectValue:
      {
         Value::Members members( value.getMemberNames() );
         if ( members.empty() )
            pushValue( L"{}" );
         else
         {
            writeWithIndent( L"{" );
            indent();
            Value::Members::iterator it = members.begin();
            while ( true )
            {
               const std::wstring &name = *it;
               const Value &childValue = value[name];
               writeCommentBeforeValue( childValue );
               writeWithIndent( valueToQuotedString( name.c_str() ) );
               *document_ << L" : ";
               writeValue( childValue );
               if ( ++it == members.end() )
               {
                  writeCommentAfterValueOnSameLine( childValue );
                  break;
               }
               *document_ << L",";
               writeCommentAfterValueOnSameLine( childValue );
            }
            unindent();
            writeWithIndent( L"}" );
         }
      }
      break;
   }
}


void 
StyledStreamWriter::writeArrayValue( const Value &value )
{
   unsigned size = value.size();
   if ( size == 0 )
      pushValue( L"[]" );
   else
   {
      bool isArrayMultiLine = isMultineArray( value );
      if ( isArrayMultiLine )
      {
         writeWithIndent( L"[" );
         indent();
         bool hasChildValue = !childValues_.empty();
         unsigned index =0;
         while ( true )
         {
            const Value &childValue = value[index];
            writeCommentBeforeValue( childValue );
            if ( hasChildValue )
               writeWithIndent( childValues_[index] );
            else
            {
	       writeIndent();
               writeValue( childValue );
            }
            if ( ++index == size )
            {
               writeCommentAfterValueOnSameLine( childValue );
               break;
            }
            *document_ << L",";
            writeCommentAfterValueOnSameLine( childValue );
         }
         unindent();
         writeWithIndent( L"]" );
      }
      else // output on a single line
      {
         assert( childValues_.size() == size );
         *document_ << L"[ ";
         for ( unsigned index =0; index < size; ++index )
         {
            if ( index > 0 )
               *document_ << L", ";
            *document_ << childValues_[index];
         }
         *document_ << L" ]";
      }
   }
}


bool 
StyledStreamWriter::isMultineArray( const Value &value )
{
   int size = value.size();
   bool isMultiLine = size*3 >= rightMargin_ ;
   childValues_.clear();
   for ( int index =0; index < size  &&  !isMultiLine; ++index )
   {
      const Value &childValue = value[index];
      isMultiLine = isMultiLine  ||
                     ( (childValue.isArray()  ||  childValue.isObject())  &&  
                        childValue.size() > 0 );
   }
   if ( !isMultiLine ) // check if line length > max line length
   {
      childValues_.reserve( size );
      addChildValues_ = true;
      int lineLength = 4 + (size-1)*2; // L'[ ' + ', '*n + ' ]'
      for ( int index =0; index < size  &&  !isMultiLine; ++index )
      {
         writeValue( value[index] );
         lineLength += int( childValues_[index].length() );
         isMultiLine = isMultiLine  &&  hasCommentForValue( value[index] );
      }
      addChildValues_ = false;
      isMultiLine = isMultiLine  ||  lineLength >= rightMargin_;
   }
   return isMultiLine;
}


void 
StyledStreamWriter::pushValue( const std::wstring &value )
{
   if ( addChildValues_ )
      childValues_.push_back( value );
   else
      *document_ << value;
}


void 
StyledStreamWriter::writeIndent()
{
  /*
    Some comments in this method would have been nice. ;-)

   if ( !document_.empty() )
   {
      wchar_t last = document_[document_.length()-1];
      if ( last == L' ' )     // already indented
         return;
      if ( last != L'\n' )    // Comments may add new-line
         *document_ << L'\n';
   }
  */
   *document_ << L'\n' << indentString_;
}


void 
StyledStreamWriter::writeWithIndent( const std::wstring &value )
{
   writeIndent();
   *document_ << value;
}


void 
StyledStreamWriter::indent()
{
   indentString_ += indentation_;
}


void 
StyledStreamWriter::unindent()
{
   assert( indentString_.size() >= indentation_.size() );
   indentString_.resize( indentString_.size() - indentation_.size() );
}


void 
StyledStreamWriter::writeCommentBeforeValue( const Value &root )
{
   if ( !root.hasComment( commentBefore ) )
      return;
   *document_ << normalizeEOL( root.getComment( commentBefore ) );
   *document_ << L"\n";
}


void 
StyledStreamWriter::writeCommentAfterValueOnSameLine( const Value &root )
{
   if ( root.hasComment( commentAfterOnSameLine ) )
      *document_ << L" " + normalizeEOL( root.getComment( commentAfterOnSameLine ) );

   if ( root.hasComment( commentAfter ) )
   {
      *document_ << L"\n";
      *document_ << normalizeEOL( root.getComment( commentAfter ) );
      *document_ << L"\n";
   }
}


bool 
StyledStreamWriter::hasCommentForValue( const Value &value )
{
   return value.hasComment( commentBefore )
          ||  value.hasComment( commentAfterOnSameLine )
          ||  value.hasComment( commentAfter );
}


std::wstring 
StyledStreamWriter::normalizeEOL( const std::wstring &text )
{
   std::wstring normalized;
   normalized.reserve( text.length() );
   const wchar_t *begin = text.c_str();
   const wchar_t *end = begin + text.length();
   const wchar_t *current = begin;
   while ( current != end )
   {
      wchar_t c = *current++;
      if ( c == L'\r' ) // mac or dos EOL
      {
         if ( *current == L'\n' ) // convert dos EOL
            ++current;
         normalized += L'\n';
      }
      else // handle unix EOL & other wchar_t
         normalized += c;
   }
   return normalized;
}


std::wostream& operator<<( std::wostream &sout, const Value &root )
{
   WJson::StyledStreamWriter writer;
   writer.write(sout, root);
   return sout;
}


} // namespace WJson

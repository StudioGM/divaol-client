#include "common.h"
#include "writer.h"
#include <utility>
#include <assert.h>
#include <stdio.h>
#include <iostream>

#if _MSC_VER >= 1400 // VC++ 8.0
#pragma warning( disable : 4996 )   // disable warning about strdup being deprecated.
#endif

namespace WJson {

static void uintToString( unsigned int value, 
                          TCHAR *&current )
{
   *--current = 0;
   do
   {
      *--current = (value % 10) + '0';
      value /= 10;
   }
   while ( value != 0 );
}

tstring valueToString( Value::Int value )
{
   TCHAR buffer[32];
   TCHAR *current = buffer + sizeof(buffer);
   bool isNegative = value < 0;
   if ( isNegative )
      value = -value;
   uintToString( Value::UInt(value), current );
   if ( isNegative )
      *--current = '-';
   assert( current >= buffer );
   return current;
}


tstring valueToString( Value::UInt value )
{
   TCHAR buffer[32];
   TCHAR *current = buffer + sizeof(buffer);
   uintToString( value, current );
   assert( current >= buffer );
   return current;
}

tstring valueToString( double value )
{
   TCHAR buffer[32];
#ifdef __STDC_SECURE_LIB__ // Use secure version with visual studio 2005 to avoid warning.
   _stprintf_s(buffer, sizeof(buffer), _T("%.16g"), value); 
#else	
   _stprintf(buffer, _T("%.16g"), value); 
#endif
   return buffer;
}


tstring valueToString( bool value )
{
   return value ? _T("true") : _T("false");
}

tstring valueToQuotedString( const TCHAR *value )
{
   // Not sure how to handle unicode...
	if (_tcspbrk(value, _T("\"\\\b\f\n\r\t")) == NULL)
      return tstring(_T("\"")) + value + _T("\"");
   // We have to walk value and escape any special characters.
   // Appending to tstring is not efficient, but this should be rare.
   // (Note: forward slashes are *not* rare, but I am not escaping them.)
   unsigned maxsize = _tcslen(value)*2 + 3; // allescaped+quotes+NULL
   tstring result;
   result.reserve(maxsize); // to avoid lots of mallocs
   result += _T("\"");
   for (const TCHAR* c=value; *c != 0; ++c){
      switch(*c){
         case '\"':
	 result += _T("\\\"");
	 break;
	 case '\\':
	 result += _T("\\\\");
	 break;
	 case '\b':
	 result += _T("\\b");
	 break;
	 case '\f':
	 result += _T("\\f");
	 break;
	 case '\n':
	 result += _T("\\n");
	 break;
	 case '\r':
	 result += _T("\\r");
	 break;
	 case '\t':
	 result += _T("\\t");
	 break;
	 case '/':
	 // Even though \/ is considered a legal escape in WJson, a bare
	 // slash is also legal, so I see no reason to escape it.
	 // (I hope I am not misunderstanding something.)
	 default:
	    result += *c;
      }
   }
   result += _T("\"");
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


tstring 
FastWriter::write( const Value &root )
{
   document_ = _T("");
   writeValue( root );
   document_ += _T("\n");
   return document_;
}


void 
FastWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      document_ += _T("null");
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
         document_ += _T("[");
         int size = value.size();
         for ( int index =0; index < size; ++index )
         {
            if ( index > 0 )
               document_ += _T(",");
            writeValue( value[index] );
         }
         document_ += _T("]");
      }
      break;
   case objectValue:
      {
         Value::Members members( value.getMemberNames() );
         document_ += _T("{");
         for ( Value::Members::iterator it = members.begin(); 
               it != members.end(); 
               ++it )
         {
            const tstring &name = *it;
            if ( it != members.begin() )
               document_ += _T(",");
            document_ += valueToQuotedString( name.c_str() );
            document_ += yamlCompatiblityEnabled_ ? _T(": ") 
                                                  : _T(":");
            writeValue( value[name] );
         }
         document_ += _T("}");
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


tstring 
StyledWriter::write( const Value &root )
{
   document_ = _T("");
   addChildValues_ = false;
   indentString_ = _T("");
   writeCommentBeforeValue( root );
   writeValue( root );
   writeCommentAfterValueOnSameLine( root );
   document_ += _T("\n");
   return document_;
}


void 
StyledWriter::writeValue( const Value &value )
{
   switch ( value.type() )
   {
   case nullValue:
      pushValue( _T("null") );
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
            pushValue( _T("{}") );
         else
         {
            writeWithIndent( _T("{") );
            indent();
            Value::Members::iterator it = members.begin();
            while ( true )
            {
               const tstring &name = *it;
               const Value &childValue = value[name];
               writeCommentBeforeValue( childValue );
               writeWithIndent( valueToQuotedString( name.c_str() ) );
               document_ += _T(" : ");
               writeValue( childValue );
               if ( ++it == members.end() )
               {
                  writeCommentAfterValueOnSameLine( childValue );
                  break;
               }
               document_ += _T(",");
               writeCommentAfterValueOnSameLine( childValue );
            }
            unindent();
            writeWithIndent( _T("}") );
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
      pushValue( _T("[]") );
   else
   {
      bool isArrayMultiLine = isMultineArray( value );
      if ( isArrayMultiLine )
      {
         writeWithIndent( _T("[") );
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
            document_ += _T(",");
            writeCommentAfterValueOnSameLine( childValue );
         }
         unindent();
         writeWithIndent( _T("]") );
      }
      else // output on a single line
      {
         assert( childValues_.size() == size );
         document_ += _T("[ ");
         for ( unsigned index =0; index < size; ++index )
         {
            if ( index > 0 )
               document_ += _T(", ");
            document_ += childValues_[index];
         }
         document_ += _T(" ]");
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
      int lineLength = 4 + (size-1)*2; // '[ ' + ', '*n + ' ]'
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
StyledWriter::pushValue( const tstring &value )
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
      TCHAR last = document_[document_.length()-1];
      if ( last == ' ' )     // already indented
         return;
      if ( last != '\n' )    // Comments may add new-line
         document_ += '\n';
   }
   document_ += indentString_;
}


void 
StyledWriter::writeWithIndent( const tstring &value )
{
   writeIndent();
   document_ += value;
}


void 
StyledWriter::indent()
{
   indentString_ += tstring( indentSize_, ' ' );
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
   document_ += _T("\n");
}


void 
StyledWriter::writeCommentAfterValueOnSameLine( const Value &root )
{
   if ( root.hasComment( commentAfterOnSameLine ) )
      document_ += _T(" ") + normalizeEOL( root.getComment( commentAfterOnSameLine ) );

   if ( root.hasComment( commentAfter ) )
   {
      document_ += _T("\n");
      document_ += normalizeEOL( root.getComment( commentAfter ) );
      document_ += _T("\n");
   }
}


bool 
StyledWriter::hasCommentForValue( const Value &value )
{
   return value.hasComment( commentBefore )
          ||  value.hasComment( commentAfterOnSameLine )
          ||  value.hasComment( commentAfter );
}


tstring 
StyledWriter::normalizeEOL( const tstring &text )
{
   tstring normalized;
   normalized.reserve( text.length() );
   const TCHAR *begin = text.c_str();
   const TCHAR *end = begin + text.length();
   const TCHAR *current = begin;
   while ( current != end )
   {
      TCHAR c = *current++;
      if ( c == '\r' ) // mac or dos EOL
      {
         if ( *current == '\n' ) // convert dos EOL
            ++current;
         normalized += '\n';
      }
      else // handle unix EOL & other TCHAR
         normalized += c;
   }
   return normalized;
}

std::ostream& operator<<( std::ostream &sout, const Value &root )
{
   //WJson::StyledWriter writer;
   //sout << writer.write(root);
   return sout;
}


} // namespace WJson

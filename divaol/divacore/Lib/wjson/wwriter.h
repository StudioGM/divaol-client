#ifndef WJSON_WRITER_H_INCLUDED
# define WJSON_WRITER_H_INCLUDED

# include "wvalue.h"
# include <vector>
# include <string>
# include <iostream>

namespace WJson {

   class Value;

   /** \brief Abstract class for writers.
    */
   class WJSON_API Writer
   {
   public:
      virtual ~Writer();

      virtual std::wstring write( const Value &root ) = 0;
   };

   /** \brief Outputs a Value in <a HREF="http://www.json.org">JSON</a> format without formatting (not human friendly).
    *
    * The JSON document is written in a single line. It is not intended for 'human' consumption,
    * but may be usefull to support feature such as RPC where bandwith is limited.
    * \sa Reader, Value
    */
   class WJSON_API FastWriter : public Writer
   {
   public:
      FastWriter();
      virtual ~FastWriter(){}

      void enableYAMLCompatibility();

   public: // overridden from Writer
      virtual std::wstring write( const Value &root );

   private:
      void writeValue( const Value &value );

      std::wstring document_;
      bool yamlCompatiblityEnabled_;
   };

   /** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a human friendly way.
    *
    * The rules for line break and indent are as follow:
    * - Object value:
    *     - if empty then print {} without indent and line break
    *     - if not empty the print '{', line break & indent, print one value per line
    *       and then unindent and line break and print '}'.
    * - Array value:
    *     - if empty then print [] without indent and line break
    *     - if the array contains no object value, empty array or some other value types,
    *       and all the values fit on one lines, then print the array on a single line.
    *     - otherwise, it the values do not fit on one line, or the array contains
    *       object or non empty array, then print one value per line.
    *
    * If the Value have comments then they are outputed according to their #CommentPlacement.
    *
    * \sa Reader, Value, Value::setComment()
    */
   class WJSON_API StyledWriter: public Writer
   {
   public:
      StyledWriter();
      virtual ~StyledWriter(){}

   public: // overridden from Writer
      /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
       * \param root Value to serialize.
       * \return String containing the JSON document that represents the root value.
       */
      virtual std::wstring write( const Value &root );

   private:
      void writeValue( const Value &value );
      void writeArrayValue( const Value &value );
      bool isMultineArray( const Value &value );
      void pushValue( const std::wstring &value );
      void writeIndent();
      void writeWithIndent( const std::wstring &value );
      void indent();
      void unindent();
      void writeCommentBeforeValue( const Value &root );
      void writeCommentAfterValueOnSameLine( const Value &root );
      bool hasCommentForValue( const Value &value );
      static std::wstring normalizeEOL( const std::wstring &text );

      typedef std::vector<std::wstring> ChildValues;

      ChildValues childValues_;
      std::wstring document_;
      std::wstring indentString_;
      int rightMargin_;
      int indentSize_;
      bool addChildValues_;
   };

   /** \brief Writes a Value in <a HREF="http://www.json.org">JSON</a> format in a human friendly way,
        to a stream rather than to a string.
    *
    * The rules for line break and indent are as follow:
    * - Object value:
    *     - if empty then print {} without indent and line break
    *     - if not empty the print '{', line break & indent, print one value per line
    *       and then unindent and line break and print '}'.
    * - Array value:
    *     - if empty then print [] without indent and line break
    *     - if the array contains no object value, empty array or some other value types,
    *       and all the values fit on one lines, then print the array on a single line.
    *     - otherwise, it the values do not fit on one line, or the array contains
    *       object or non empty array, then print one value per line.
    *
    * If the Value have comments then they are outputed according to their #CommentPlacement.
    *
    * \param indentation Each level will be indented by this amount extra.
    * \sa Reader, Value, Value::setComment()
    */
   class WJSON_API StyledStreamWriter
   {
   public:
      StyledStreamWriter( std::wstring indentation=L"\t" );
      ~StyledStreamWriter(){}

   public:
      /** \brief Serialize a Value in <a HREF="http://www.json.org">JSON</a> format.
       * \param out Stream to write to. (Can be ostringstream, e.g.)
       * \param root Value to serialize.
       * \note There is no point in deriving from Writer, since write() should not return a value.
       */
      void write( std::wostream &out, const Value &root );

   private:
      void writeValue( const Value &value );
      void writeArrayValue( const Value &value );
      bool isMultineArray( const Value &value );
      void pushValue( const std::wstring &value );
      void writeIndent();
      void writeWithIndent( const std::wstring &value );
      void indent();
      void unindent();
      void writeCommentBeforeValue( const Value &root );
      void writeCommentAfterValueOnSameLine( const Value &root );
      bool hasCommentForValue( const Value &value );
      static std::wstring normalizeEOL( const std::wstring &text );

      typedef std::vector<std::wstring> ChildValues;

      ChildValues childValues_;
      std::wostream* document_;
      std::wstring indentString_;
      int rightMargin_;
      std::wstring indentation_;
      bool addChildValues_;
   };

   std::wstring WJSON_API valueToString( Int value );
   std::wstring WJSON_API valueToString( UInt value );
   std::wstring WJSON_API valueToString( double value );
   std::wstring WJSON_API valueToString( bool value );
   std::wstring WJSON_API valueToQuotedString( const wchar_t *value );

   /// \brief Output using the StyledStreamWriter.
   /// \see WJson::operator>>()
   std::wostream& operator<<( std::wostream&, const Value &root );

} // namespace WJson



#endif // WJSON_WRITER_H_INCLUDED

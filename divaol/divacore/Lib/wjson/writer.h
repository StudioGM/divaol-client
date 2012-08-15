#ifndef WJSON_WRITER_H_INCLUDED
# define WJSON_WRITER_H_INCLUDED

# include "value.h"
# include <vector>
# include <string>
# include <iostream>

namespace WJson {

   class Value;

   /** \brief Abstract class for writers.
    */
   class JSON_API Writer
   {
   public:
      virtual ~Writer();

      virtual tstring write( const Value &root ) = 0;
   };

   /** \brief Outputs a Value in <a HREF="http://www.WJson.org">WJson</a> format without formatting (not human friendly).
    *
    * The WJson document is written in a single line. It is not intended for 'human' consumption,
    * but may be usefull to support feature such as RPC where bandwith is limited.
    * \sa Reader, Value
    */
   class JSON_API FastWriter : public Writer
   {
   public:
      FastWriter();

      void enableYAMLCompatibility();

   public: // overridden from Writer
      virtual tstring write( const Value &root );

   private:
      void writeValue( const Value &value );

      tstring document_;
      bool yamlCompatiblityEnabled_;
   };

   /** \brief Writes a Value in <a HREF="http://www.WJson.org">WJson</a> format in a human friendly way.
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
   class JSON_API StyledWriter
   {
   public:
      StyledWriter();
      virtual ~StyledWriter(){}

   public: // overridden from Writer
      /** \brief Serialize a Value in <a HREF="http://www.WJson.org">WJson</a> format.
       * \param root Value to serialize.
       * \return String containing the WJson document that represents the root value.
       */
      virtual tstring write( const Value &root );

   private:
      void writeValue( const Value &value );
      void writeArrayValue( const Value &value );
      bool isMultineArray( const Value &value );
      void pushValue( const tstring &value );
      void writeIndent();
      void writeWithIndent( const tstring &value );
      void indent();
      void unindent();
      void writeCommentBeforeValue( const Value &root );
      void writeCommentAfterValueOnSameLine( const Value &root );
      bool hasCommentForValue( const Value &value );
      static tstring normalizeEOL( const tstring &text );

      typedef std::vector<tstring> ChildValues;

      ChildValues childValues_;
      tstring document_;
      tstring indentString_;
      int rightMargin_;
      int indentSize_;
      bool addChildValues_;
   };

   tstring JSON_API valueToString( Value::Int value );
   tstring JSON_API valueToString( Value::UInt value );
   tstring JSON_API valueToString( double value );
   tstring JSON_API valueToString( bool value );
   tstring JSON_API valueToQuotedString( const TCHAR *value );

   /// \brief Output using the StyledWriter.
   /// \see WJson::operator>>()
   std::ostream& operator<<( std::ostream&, const Value &root );

} // namespace WJson



#endif // JSON_WRITER_H_INCLUDED

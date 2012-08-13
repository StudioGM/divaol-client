#ifndef CPPTL_WJSON_READER_H_INCLUDED
# define CPPTL_WJSON_READER_H_INCLUDED

# include "forwards.h"
# include "value.h"
# include <deque>
# include <stack>
# include <string>
# include <iostream>

namespace WJson {

   class Value;

   /** \brief Unserialize a <a HREF="http://www.WJson.org">WJson</a> document into a Value.
    *
    *
    */
   class JSON_API Reader
   {
   public:
      typedef TCHAR Char;
      typedef const Char *Location;

      Reader();

      /** \brief Read a Value from a <a HREF="http://www.WJson.org">WJson</a> document.
       * \param document UTF-8 encoded string containing the document to read.
       * \param root [out] Contains the root value of the document if it was
       *             successfully parsed.
       * \param collectComments \c true to collect comment and allow writing them back during
       *                        serialization, \c false to discard comments.
       * \return \c true if the document was successfully parsed, \c false if an error occurred.
       */
      bool parse( const tstring &document, 
                  Value &root,
                  bool collectComments = true );

      /** \brief Read a Value from a <a HREF="http://www.WJson.org">WJson</a> document.
       * \param document UTF-8 encoded string containing the document to read.
       * \param root [out] Contains the root value of the document if it was
       *             successfully parsed.
       * \param collectComments \c true to collect comment and allow writing them back during
       *                        serialization, \c false to discard comments.
       * \return \c true if the document was successfully parsed, \c false if an error occurred.
       */
      bool parse( const TCHAR *beginDoc, const TCHAR *endDoc, 
                  Value &root,
                  bool collectComments = true );

      /// \brief Parse from input stream.
      /// \see WJson::operator>>(tistream&, WJson::Value&).
      bool parse( tistream&,
                  Value &root,
                  bool collectComments = true );

      /** \brief Returns a user friendly string that list errors in the parsed document.
       * \return Formatted error message with the list of errors with their location in 
       *         the parsed document. An empty string is returned if no error occurred
       *         during parsing.
       */
      tstring getFormatedErrorMessages() const;

   private:
      enum TokenType
      {
         tokenEndOfStream = 0,
         tokenObjectBegin,
         tokenObjectEnd,
         tokenArrayBegin,
         tokenArrayEnd,
         tokenString,
         tokenNumber,
         tokenTrue,
         tokenFalse,
         tokenNull,
         tokenArraySeparator,
         tokenMemberSeparator,
         tokenComment,
         tokenError
      };

      class Token
      {
      public:
         TokenType type_;
         Location start_;
         Location end_;
      };

      class ErrorInfo
      {
      public:
         Token token_;
         tstring message_;
         Location extra_;
      };

      typedef std::deque<ErrorInfo> Errors;

      bool expectToken( TokenType type, Token &token, const TCHAR *message );
      bool readToken( Token &token );
      void skipSpaces();
      bool match( Location pattern, 
                  int patternLength );
      bool readComment();
      bool readCStyleComment();
      bool readCppStyleComment();
      bool readString();
      void readNumber();
      bool readValue();
      bool readObject( Token &token );
      bool readArray( Token &token );
      bool decodeNumber( Token &token );
      bool decodeString( Token &token );
      bool decodeString( Token &token, tstring &decoded );
      bool decodeDouble( Token &token );
      bool decodeUnicodeEscapeSequence( Token &token, 
                                        Location &current, 
                                        Location end, 
                                        unsigned int &unicode );
      bool addError( const tstring &message, 
                     Token &token,
                     Location extra = 0 );
      bool recoverFromError( TokenType skipUntilToken );
      bool addErrorAndRecover( const tstring &message, 
                               Token &token,
                               TokenType skipUntilToken );
      void skipUntilSpace();
      Value &currentValue();
      Char getNextChar();
      void getLocationLineAndColumn( Location location,
                                     int &line,
                                     int &column ) const;
      tstring getLocationLineAndColumn( Location location ) const;
      void addComment( Location begin, 
                       Location end, 
                       CommentPlacement placement );
      void skipCommentTokens( Token &token );
   
      typedef std::stack<Value *> Nodes;
      Nodes nodes_;
      Errors errors_;
      tstring document_;
      Location begin_;
      Location end_;
      Location current_;
      Location lastValueEnd_;
      Value *lastValue_;
      tstring commentsBefore_;
      bool collectComments_;
   };

   /** \brief Read from 'sin' into 'root'.

    Always keep comments from the input WJson.

    This can be used to read a file into a particular sub-object.
    For example:
    \code
    WJson::Value root;
    cin >> root["dir"]["file"];
    cout << root;
    \endcode
    Result:
    \verbatim
    {
	"dir": {
	    "file": {
		// The input stream WJson would be nested here.
	    }
	}
    }
    \endverbatim
    \throw std::exception on parse error.
    \see WJson::operator<<()
   */
   tistream& operator>>( tistream&, Value& );

} // namespace WJson

#endif // CPPTL_JSON_READER_H_INCLUDED

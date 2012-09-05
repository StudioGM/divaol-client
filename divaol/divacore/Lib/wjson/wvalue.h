#ifndef CPPTL_WJSON_H_INCLUDED
# define CPPTL_WJSON_H_INCLUDED

# include "wforwards.h"
# include <string>
# include <vector>

# ifndef WJSON_USE_CPPTL_SMALLMAP
#  include <map>
# else
#  include <cpptl/smallmap.h>
# endif
# ifdef WJSON_USE_CPPTL
#  include <cpptl/forwards.h>
# endif

/** \brief JSON (JavaScript Object Notation).
 */
namespace WJson {

   /** \brief Type of the value held by a Value object.
    */
   enum ValueType
   {
      nullValue = 0, ///< 'null' value
      intValue,      ///< signed integer value
      uintValue,     ///< unsigned integer value
      realValue,     ///< double value
      stringValue,   ///< UTF-8 string value
      booleanValue,  ///< bool value
      arrayValue,    ///< array value (ordered list)
      objectValue    ///< object value (collection of name/value pairs).
   };

   enum CommentPlacement
   {
      commentBefore = 0,        ///< a comment placed on the line before a value
      commentAfterOnSameLine,   ///< a comment just after a value on the same line
      commentAfter,             ///< a comment on the line after a value (only make sense for root value)
      numberOfCommentPlacement
   };

//# ifdef WJSON_USE_CPPTL
//   typedef CppTL::AnyEnumerator<const wchar_t *> EnumMemberNames;
//   typedef CppTL::AnyEnumerator<const Value &> EnumValues;
//# endif

   /** \brief Lightweight wrapper to tag static string.
    *
    * Value constructor and objectValue member assignement takes advantage of the
    * StaticString and avoid the cost of string duplication when storing the
    * string or the member name.
    *
    * Example of usage:
    * \code
    * WJson::Value aValue( StaticString("some text") );
    * WJson::Value object;
    * static const StaticString code("code");
    * object[code] = 1234;
    * \endcode
    */
   class WJSON_API StaticString
   {
   public:
      explicit StaticString( const wchar_t *czstring )
         : str_( czstring )
      {
      }

      operator const wchar_t *() const
      {
         return str_;
      }

      const wchar_t *c_str() const
      {
         return str_;
      }

   private:
      const wchar_t *str_;
   };

   /** \brief Represents a <a HREF="http://www.json.org">JSON</a> value.
    *
    * This class is a discriminated union wrapper that can represents a:
    * - signed integer [range: Value::minInt - Value::maxInt]
    * - unsigned integer (range: 0 - Value::maxUInt)
    * - double
    * - UTF-8 string
    * - boolean
    * - 'null'
    * - an ordered list of Value
    * - collection of name/value pairs (javascript object)
    *
    * The type of the held value is represented by a #ValueType and 
    * can be obtained using type().
    *
    * values of an #objectValue or #arrayValue can be accessed using operator[]() methods. 
    * Non const methods will automatically create the a #nullValue element 
    * if it does not exist. 
    * The sequence of an #arrayValue will be automatically resize and initialized 
    * with #nullValue. resize() can be used to enlarge or truncate an #arrayValue.
    *
    * The get() methods can be used to obtanis default value in the case the required element
    * does not exist.
    *
    * It is possible to iterate over the list of a #objectValue values using 
    * the getMemberNames() method.
    */
   class WJSON_API Value 
   {
      friend class ValueIteratorBase;
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
      friend class ValueInternalLink;
      friend class ValueInternalMap;
# endif
   public:
      typedef std::vector<std::wstring> Members;
      typedef ValueIterator iterator;
      typedef ValueConstIterator const_iterator;
      typedef WJson::UInt UInt;
      typedef WJson::Int Int;
      typedef UInt ArrayIndex;

      static const Value null;
      static const Int minInt;
      static const Int maxInt;
      static const UInt maxUInt;

   private:
#ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
# ifndef WJSON_VALUE_USE_INTERNAL_MAP
      class CZString 
      {
      public:
         enum DuplicationPolicy 
         {
            noDuplication = 0,
            duplicate,
            duplicateOnCopy
         };
         CZString( int index );
         CZString( const wchar_t *cstr, DuplicationPolicy allocate );
         CZString( const CZString &other );
         ~CZString();
         CZString &operator =( const CZString &other );
         bool operator<( const CZString &other ) const;
         bool operator==( const CZString &other ) const;
         int index() const;
         const wchar_t *c_str() const;
         bool isStaticString() const;
      private:
         void swap( CZString &other );
         const wchar_t *cstr_;
         int index_;
      };

   public:
#  ifndef WJSON_USE_CPPTL_SMALLMAP
      typedef std::map<CZString, Value> ObjectValues;
#  else
      typedef CppTL::SmallMap<CZString, Value> ObjectValues;
#  endif // ifndef WJSON_USE_CPPTL_SMALLMAP
# endif // ifndef WJSON_VALUE_USE_INTERNAL_MAP
#endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

   public:
      /** \brief Create a default Value of the given type.

        This is a very useful constructor.
        To create an empty array, pass arrayValue.
        To create an empty object, pass objectValue.
        Another Value can then be set to this one by assignment.
	This is useful since clear() and resize() will not alter types.

        Examples:
	\code
	WJson::Value null_value; // null
	WJson::Value arr_value(WJson::arrayValue); // []
	WJson::Value obj_value(WJson::objectValue); // {}
	\endcode
      */
      Value( ValueType type = nullValue );
      Value( Int value );
      Value( UInt value );
      Value( double value );
      Value( const wchar_t *value );
      Value( const wchar_t *beginValue, const wchar_t *endValue );
      /** \brief Constructs a value from a static string.

       * Like other value string constructor but do not duplicate the string for
       * internal storage. The given string must remain alive after the call to this
       * constructor.
       * Example of usage:
       * \code
       * WJson::Value aValue( StaticString("some text") );
       * \endcode
       */
      Value( const StaticString &value );
      Value( const std::wstring &value );
# ifdef WJSON_USE_CPPTL
      Value( const CppTL::ConstString &value );
# endif
      Value( bool value );
      Value( const Value &other );
      ~Value();

      Value &operator=( const Value &other );
      /// Swap values.
      /// \note Currently, comments are intentionally not swapped, for
      /// both logic and efficiency.
      void swap( Value &other );

      ValueType type() const;

      bool operator <( const Value &other ) const;
      bool operator <=( const Value &other ) const;
      bool operator >=( const Value &other ) const;
      bool operator >( const Value &other ) const;

      bool operator ==( const Value &other ) const;
      bool operator !=( const Value &other ) const;

      int compare( const Value &other );

      const wchar_t *asCString() const;
      std::wstring asString() const;
# ifdef WJSON_USE_CPPTL
      CppTL::ConstString asConstString() const;
# endif
      Int asInt() const;
      UInt asUInt() const;
      double asDouble() const;
      bool asBool() const;

      bool isNull() const;
      bool isBool() const;
      bool isInt() const;
      bool isUInt() const;
      bool isIntegral() const;
      bool isDouble() const;
      bool isNumeric() const;
      bool isString() const;
      bool isArray() const;
      bool isObject() const;

      bool isConvertibleTo( ValueType other ) const;

      /// Number of values in array or object
      UInt size() const;

      /// \brief Return true if empty array, empty object, or null;
      /// otherwise, false.
      bool empty() const;

      /// Return isNull()
      bool operator!() const;

      /// Remove all object members and array elements.
      /// \pre type() is arrayValue, objectValue, or nullValue
      /// \post type() is unchanged
      void clear();

      /// Resize the array to size elements. 
      /// New elements are initialized to null.
      /// May only be called on nullValue or arrayValue.
      /// \pre type() is arrayValue or nullValue
      /// \post type() is arrayValue
      void resize( UInt size );

      /// Access an array element (zero based index ).
      /// If the array contains less than index element, then null value are inserted
      /// in the array so that its size is index+1.
      /// (You may need to say 'value[0u]' to get your compiler to distinguish
      ///  this from the operator[] which takes a string.)
      Value &operator[]( UInt index );
      /// Access an array element (zero based index )
      /// (You may need to say 'value[0u]' to get your compiler to distinguish
      ///  this from the operator[] which takes a string.)
      const Value &operator[]( UInt index ) const;
      /// If the array contains at least index+1 elements, returns the element value, 
      /// otherwise returns defaultValue.
      Value get( UInt index, 
                 const Value &defaultValue ) const;
      /// Return true if index < size().
      bool isValidIndex( UInt index ) const;
      /// \brief Append value to array at the end.
      ///
      /// Equivalent to jsonvalue[jsonvalue.size()] = value;
      Value &append( const Value &value );

      /// Access an object value by name, create a null member if it does not exist.
      Value &operator[]( const wchar_t *key );
      /// Access an object value by name, returns null if there is no member with that name.
      const Value &operator[]( const wchar_t *key ) const;
      /// Access an object value by name, create a null member if it does not exist.
      Value &operator[]( const std::wstring &key );
      /// Access an object value by name, returns null if there is no member with that name.
      const Value &operator[]( const std::wstring &key ) const;
      /** \brief Access an object value by name, create a null member if it does not exist.

       * If the object as no entry for that name, then the member name used to store
       * the new entry is not duplicated.
       * Example of use:
       * \code
       * WJson::Value object;
       * static const StaticString code("code");
       * object[code] = 1234;
       * \endcode
       */
      Value &operator[]( const StaticString &key );
# ifdef WJSON_USE_CPPTL
      /// Access an object value by name, create a null member if it does not exist.
      Value &operator[]( const CppTL::ConstString &key );
      /// Access an object value by name, returns null if there is no member with that name.
      const Value &operator[]( const CppTL::ConstString &key ) const;
# endif
      /// Return the member named key if it exist, defaultValue otherwise.
      Value get( const wchar_t *key, 
                 const Value &defaultValue ) const;
      /// Return the member named key if it exist, defaultValue otherwise.
      Value get( const std::wstring &key,
                 const Value &defaultValue ) const;
# ifdef WJSON_USE_CPPTL
      /// Return the member named key if it exist, defaultValue otherwise.
      Value get( const CppTL::ConstString &key,
                 const Value &defaultValue ) const;
# endif
      /// \brief Remove and return the named member.  
      ///
      /// Do nothing if it did not exist.
      /// \return the removed Value, or null.
      /// \pre type() is objectValue or nullValue
      /// \post type() is unchanged
      Value removeMember( const wchar_t* key );
      /// Same as removeMember(const wchar_t*)
      Value removeMember( const std::wstring &key );

      /// Return true if the object has a member named key.
      bool isMember( const wchar_t *key ) const;
      /// Return true if the object has a member named key.
      bool isMember( const std::wstring &key ) const;
# ifdef WJSON_USE_CPPTL
      /// Return true if the object has a member named key.
      bool isMember( const CppTL::ConstString &key ) const;
# endif

      /// \brief Return a list of the member names.
      ///
      /// If null, return an empty list.
      /// \pre type() is objectValue or nullValue
      /// \post if type() was nullValue, it remains nullValue
      Members getMemberNames() const;

//# ifdef WJSON_USE_CPPTL
//      EnumMemberNames enumMemberNames() const;
//      EnumValues enumValues() const;
//# endif

      /// Comments must be //... or /* ... */
      void setComment( const wchar_t *comment,
                       CommentPlacement placement );
      /// Comments must be //... or /* ... */
      void setComment( const std::wstring &comment,
                       CommentPlacement placement );
      bool hasComment( CommentPlacement placement ) const;
      /// Include delimiters and embedded newlines.
      std::wstring getComment( CommentPlacement placement ) const;

      std::wstring toStyledString() const;

      const_iterator begin() const;
      const_iterator end() const;

      iterator begin();
      iterator end();
       
      const Value* parent() const;

   private:
      Value &resolveReference( const wchar_t *key, 
                               bool isStatic );

# ifdef WJSON_VALUE_USE_INTERNAL_MAP
      inline bool isItemAvailable() const
      {
         return itemIsUsed_ == 0;
      }

      inline void setItemUsed( bool isUsed = true )
      {
         itemIsUsed_ = isUsed ? 1 : 0;
      }

      inline bool isMemberNameStatic() const
      {
         return memberNameIsStatic_ == 0;
      }

      inline void setMemberNameIsStatic( bool isStatic )
      {
         memberNameIsStatic_ = isStatic ? 1 : 0;
      }
# endif // # ifdef WJSON_VALUE_USE_INTERNAL_MAP

   private:
      struct CommentInfo
      {
         CommentInfo();
         ~CommentInfo();

         void setComment( const wchar_t *text );

         wchar_t *comment_;
      };

      //struct MemberNamesTransform
      //{
      //   typedef const wchar_t *result_type;
      //   const wchar_t *operator()( const CZString &name ) const
      //   {
      //      return name.c_str();
      //   }
      //};

      union ValueHolder
      {
         Int int_;
         UInt uint_;
         double real_;
         bool bool_;
         wchar_t *string_;
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
         ValueInternalArray *array_;
         ValueInternalMap *map_;
#else
         ObjectValues *map_;
# endif
      } value_;
      ValueType type_ : 8;
      int allocated_ : 1;     // Notes: if declared as bool, bitfield is useless.
# ifdef WJSON_VALUE_USE_INTERNAL_MAP
      unsigned int itemIsUsed_ : 1;      // used by the ValueInternalMap container.
      int memberNameIsStatic_ : 1;       // used by the ValueInternalMap container.
# endif
      CommentInfo *comments_;
       
      const Value* parent_;
   };


   /** \brief Experimental and untested: represents an element of the "path" to access a node.
    */
   class PathArgument
   {
   public:
      friend class Path;

      PathArgument();
      PathArgument( UInt index );
      PathArgument( const wchar_t *key );
      PathArgument( const std::wstring &key );

   private:
      enum Kind
      {
         kindNone = 0,
         kindIndex,
         kindKey
      };
      std::wstring key_;
      UInt index_;
      Kind kind_;
   };

   /** \brief Experimental and untested: represents a "path" to access a node.
    *
    * Syntax:
    * - "." => root node
    * - ".[n]" => elements at index 'n' of root node (an array value)
    * - ".name" => member named 'name' of root node (an object value)
    * - ".name1.name2.name3"
    * - ".[0][1][2].name1[3]"
    * - ".%" => member name is provided as parameter
    * - ".[%]" => index is provied as parameter
    */
   class Path
   {
   public:
      Path( const std::wstring &path,
            const PathArgument &a1 = PathArgument(),
            const PathArgument &a2 = PathArgument(),
            const PathArgument &a3 = PathArgument(),
            const PathArgument &a4 = PathArgument(),
            const PathArgument &a5 = PathArgument() );

      const Value &resolve( const Value &root ) const;
      Value resolve( const Value &root, 
                     const Value &defaultValue ) const;
      /// Creates the "path" to access the specified node and returns a reference on the node.
      Value &make( Value &root ) const;

   private:
      typedef std::vector<const PathArgument *> InArgs;
      typedef std::vector<PathArgument> Args;

      void makePath( const std::wstring &path,
                     const InArgs &in );
      void addPathInArg( const std::wstring &path, 
                         const InArgs &in, 
                         InArgs::const_iterator &itInArg, 
                         PathArgument::Kind kind );
      void invalidPath( const std::wstring &path, 
                        int location );

      Args args_;
   };

   /** \brief Experimental do not use: Allocator to customize member name and string value memory management done by Value.
    *
    * - makeMemberName() and releaseMemberName() are called to respectively duplicate and
    *   free an WJson::objectValue member name.
    * - duplicateStringValue() and releaseStringValue() are called similarly to
    *   duplicate and free a WJson::stringValue value.
    */
   class ValueAllocator
   {
   public:
      enum { unknown = (unsigned)-1 };

      virtual ~ValueAllocator();

      virtual wchar_t *makeMemberName( const wchar_t *memberName ) = 0;
      virtual void releaseMemberName( wchar_t *memberName ) = 0;
      virtual wchar_t *duplicateStringValue( const wchar_t *value, 
                                          unsigned int length = unknown ) = 0;
      virtual void releaseStringValue( wchar_t *value ) = 0;
   };

#ifdef WJSON_VALUE_USE_INTERNAL_MAP
   /** \brief Allocator to customize Value internal map.
    * Below is an example of a simple implementation (default implementation actually
    * use memory pool for speed).
    * \code
      class DefaultValueMapAllocator : public ValueMapAllocator
      {
      public: // overridden from ValueMapAllocator
         virtual ValueInternalMap *newMap()
         {
            return new ValueInternalMap();
         }

         virtual ValueInternalMap *newMapCopy( const ValueInternalMap &other )
         {
            return new ValueInternalMap( other );
         }

         virtual void destructMap( ValueInternalMap *map )
         {
            delete map;
         }

         virtual ValueInternalLink *allocateMapBuckets( unsigned int size )
         {
            return new ValueInternalLink[size];
         }

         virtual void releaseMapBuckets( ValueInternalLink *links )
         {
            delete [] links;
         }

         virtual ValueInternalLink *allocateMapLink()
         {
            return new ValueInternalLink();
         }

         virtual void releaseMapLink( ValueInternalLink *link )
         {
            delete link;
         }
      };
    * \endcode
    */ 
   class WJSON_API ValueMapAllocator
   {
   public:
      virtual ~ValueMapAllocator();
      virtual ValueInternalMap *newMap() = 0;
      virtual ValueInternalMap *newMapCopy( const ValueInternalMap &other ) = 0;
      virtual void destructMap( ValueInternalMap *map ) = 0;
      virtual ValueInternalLink *allocateMapBuckets( unsigned int size ) = 0;
      virtual void releaseMapBuckets( ValueInternalLink *links ) = 0;
      virtual ValueInternalLink *allocateMapLink() = 0;
      virtual void releaseMapLink( ValueInternalLink *link ) = 0;
   };

   /** \brief ValueInternalMap hash-map bucket chain link (for internal use only).
    * \internal previous_ & next_ allows for bidirectional traversal.
    */
   class WJSON_API ValueInternalLink
   {
   public:
      enum { itemPerLink = 6 };  // sizeof(ValueInternalLink) = 128 on 32 bits architecture.
      enum InternalFlags { 
         flagAvailable = 0,
         flagUsed = 1
      };

      ValueInternalLink();

      ~ValueInternalLink();

      Value items_[itemPerLink];
      wchar_t *keys_[itemPerLink];
      ValueInternalLink *previous_;
      ValueInternalLink *next_;
   };


   /** \brief A linked page based hash-table implementation used internally by Value.
    * \internal ValueInternalMap is a tradional bucket based hash-table, with a linked
    * list in each bucket to handle collision. There is an addional twist in that
    * each node of the collision linked list is a page containing a fixed amount of
    * value. This provides a better compromise between memory usage and speed.
    * 
    * Each bucket is made up of a chained list of ValueInternalLink. The last
    * link of a given bucket can be found in the 'previous_' field of the following bucket.
    * The last link of the last bucket is stored in tailLink_ as it has no following bucket.
    * Only the last link of a bucket may contains 'available' item. The last link always
    * contains at least one element unless is it the bucket one very first link.
    */
   class WJSON_API ValueInternalMap
   {
      friend class ValueIteratorBase;
      friend class Value;
   public:
      typedef unsigned int HashKey;
      typedef unsigned int BucketIndex;

# ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
      struct IteratorState
      {
         IteratorState() 
            : map_(0)
            , link_(0)
            , itemIndex_(0)
            , bucketIndex_(0) 
         {
         }
         ValueInternalMap *map_;
         ValueInternalLink *link_;
         BucketIndex itemIndex_;
         BucketIndex bucketIndex_;
      };
# endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

      ValueInternalMap();
      ValueInternalMap( const ValueInternalMap &other );
      ValueInternalMap &operator =( const ValueInternalMap &other );
      ~ValueInternalMap();

      void swap( ValueInternalMap &other );

      BucketIndex size() const;

      void clear();

      bool reserveDelta( BucketIndex growth );

      bool reserve( BucketIndex newItemCount );

      const Value *find( const wchar_t *key ) const;

      Value *find( const wchar_t *key );

      Value &resolveReference( const wchar_t *key, 
                               bool isStatic );

      void remove( const wchar_t *key );

      void doActualRemove( ValueInternalLink *link, 
                           BucketIndex index,
                           BucketIndex bucketIndex );

      ValueInternalLink *&getLastLinkInBucket( BucketIndex bucketIndex );

      Value &setNewItem( const wchar_t *key, 
                         bool isStatic, 
                         ValueInternalLink *link, 
                         BucketIndex index );

      Value &unsafeAdd( const wchar_t *key, 
                        bool isStatic, 
                        HashKey hashedKey );

      HashKey hash( const wchar_t *key ) const;

      int compare( const ValueInternalMap &other ) const;

   private:
      void makeBeginIterator( IteratorState &it ) const;
      void makeEndIterator( IteratorState &it ) const;
      static bool equals( const IteratorState &x, const IteratorState &other );
      static void increment( IteratorState &iterator );
      static void incrementBucket( IteratorState &iterator );
      static void decrement( IteratorState &iterator );
      static const wchar_t *key( const IteratorState &iterator );
      static const wchar_t *key( const IteratorState &iterator, bool &isStatic );
      static Value &value( const IteratorState &iterator );
      static int distance( const IteratorState &x, const IteratorState &y );

   private:
      ValueInternalLink *buckets_;
      ValueInternalLink *tailLink_;
      BucketIndex bucketsSize_;
      BucketIndex itemCount_;
   };

   /** \brief A simplified deque implementation used internally by Value.
   * \internal
   * It is based on a list of fixed "page", each page contains a fixed number of items.
   * Instead of using a linked-list, a array of pointer is used for fast item look-up.
   * Look-up for an element is as follow:
   * - compute page index: pageIndex = itemIndex / itemsPerPage
   * - look-up item in page: pages_[pageIndex][itemIndex % itemsPerPage]
   *
   * Insertion is amortized constant time (only the array containing the index of pointers
   * need to be reallocated when items are appended).
   */
   class WJSON_API ValueInternalArray
   {
      friend class Value;
      friend class ValueIteratorBase;
   public:
      enum { itemsPerPage = 8 };    // should be a power of 2 for fast divide and modulo.
      typedef Value::ArrayIndex ArrayIndex;
      typedef unsigned int PageIndex;

# ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION
      struct IteratorState // Must be a POD
      {
         IteratorState() 
            : array_(0)
            , currentPageIndex_(0)
            , currentItemIndex_(0) 
         {
         }
         ValueInternalArray *array_;
         Value **currentPageIndex_;
         unsigned int currentItemIndex_;
      };
# endif // ifndef JSONCPP_DOC_EXCLUDE_IMPLEMENTATION

      ValueInternalArray();
      ValueInternalArray( const ValueInternalArray &other );
      ValueInternalArray &operator =( const ValueInternalArray &other );
      ~ValueInternalArray();
      void swap( ValueInternalArray &other );

      void clear();
      void resize( ArrayIndex newSize );

      Value &resolveReference( ArrayIndex index );

      Value *find( ArrayIndex index ) const;

      ArrayIndex size() const;

      int compare( const ValueInternalArray &other ) const;

   private:
      static bool equals( const IteratorState &x, const IteratorState &other );
      static void increment( IteratorState &iterator );
      static void decrement( IteratorState &iterator );
      static Value &dereference( const IteratorState &iterator );
      static Value &unsafeDereference( const IteratorState &iterator );
      static int distance( const IteratorState &x, const IteratorState &y );
      static ArrayIndex indexOf( const IteratorState &iterator );
      void makeBeginIterator( IteratorState &it ) const;
      void makeEndIterator( IteratorState &it ) const;
      void makeIterator( IteratorState &it, ArrayIndex index ) const;

      void makeIndexValid( ArrayIndex index );

      Value **pages_;
      ArrayIndex size_;
      PageIndex pageCount_;
   };

   /** \brief Experimental: do not use. Allocator to customize Value internal array.
    * Below is an example of a simple implementation (actual implementation use
    * memory pool).
      \code
class DefaultValueArrayAllocator : public ValueArrayAllocator
{
public: // overridden from ValueArrayAllocator
   virtual ~DefaultValueArrayAllocator()
   {
   }

   virtual ValueInternalArray *newArray()
   {
      return new ValueInternalArray();
   }

   virtual ValueInternalArray *newArrayCopy( const ValueInternalArray &other )
   {
      return new ValueInternalArray( other );
   }

   virtual void destruct( ValueInternalArray *array )
   {
      delete array;
   }

   virtual void reallocateArrayPageIndex( Value **&indexes, 
                                          ValueInternalArray::PageIndex &indexCount,
                                          ValueInternalArray::PageIndex minNewIndexCount )
   {
      ValueInternalArray::PageIndex newIndexCount = (indexCount*3)/2 + 1;
      if ( minNewIndexCount > newIndexCount )
         newIndexCount = minNewIndexCount;
      void *newIndexes = realloc( indexes, sizeof(Value*) * newIndexCount );
      if ( !newIndexes )
         throw std::bad_alloc();
      indexCount = newIndexCount;
      indexes = static_cast<Value **>( newIndexes );
   }
   virtual void releaseArrayPageIndex( Value **indexes, 
                                       ValueInternalArray::PageIndex indexCount )
   {
      if ( indexes )
         free( indexes );
   }

   virtual Value *allocateArrayPage()
   {
      return static_cast<Value *>( malloc( sizeof(Value) * ValueInternalArray::itemsPerPage ) );
   }

   virtual void releaseArrayPage( Value *value )
   {
      if ( value )
         free( value );
   }
};
      \endcode
    */ 
   class WJSON_API ValueArrayAllocator
   {
   public:
      virtual ~ValueArrayAllocator();
      virtual ValueInternalArray *newArray() = 0;
      virtual ValueInternalArray *newArrayCopy( const ValueInternalArray &other ) = 0;
      virtual void destructArray( ValueInternalArray *array ) = 0;
      /** \brief Reallocate array page index.
       * Reallocates an array of pointer on each page.
       * \param indexes [input] pointer on the current index. May be \c NULL.
       *                [output] pointer on the new index of at least 
       *                         \a minNewIndexCount pages. 
       * \param indexCount [input] current number of pages in the index.
       *                   [output] number of page the reallocated index can handle.
       *                            \b MUST be >= \a minNewIndexCount.
       * \param minNewIndexCount Minimum number of page the new index must be able to
       *                         handle.
       */
      virtual void reallocateArrayPageIndex( Value **&indexes, 
                                             ValueInternalArray::PageIndex &indexCount,
                                             ValueInternalArray::PageIndex minNewIndexCount ) = 0;
      virtual void releaseArrayPageIndex( Value **indexes, 
                                          ValueInternalArray::PageIndex indexCount ) = 0;
      virtual Value *allocateArrayPage() = 0;
      virtual void releaseArrayPage( Value *value ) = 0;
   };
#endif // #ifdef WJSON_VALUE_USE_INTERNAL_MAP


   /** \brief base class for Value iterators.
    *
    */
   class ValueIteratorBase
   {
   public:
      typedef unsigned int size_t;
      typedef int difference_type;
      typedef ValueIteratorBase SelfType;

      ValueIteratorBase();
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
      explicit ValueIteratorBase( const Value::ObjectValues::iterator &current );
#else
      ValueIteratorBase( const ValueInternalArray::IteratorState &state );
      ValueIteratorBase( const ValueInternalMap::IteratorState &state );
#endif

      bool operator ==( const SelfType &other ) const
      {
         return isEqual( other );
      }

      bool operator !=( const SelfType &other ) const
      {
         return !isEqual( other );
      }

      difference_type operator -( const SelfType &other ) const
      {
         return computeDistance( other );
      }

      /// Return either the index or the member name of the referenced value as a Value.
      Value key() const;

      /// Return the index of the referenced Value. -1 if it is not an arrayValue.
      UInt index() const;

      /// Return the member name of the referenced Value. "" if it is not an objectValue.
      const wchar_t *memberName() const;

   protected:
      Value &deref() const;

      void increment();

      void decrement();

      difference_type computeDistance( const SelfType &other ) const;

      bool isEqual( const SelfType &other ) const;

      void copy( const SelfType &other );

   private:
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
      Value::ObjectValues::iterator current_;
      // Indicates that iterator is for a null value.
      bool isNull_;
#else
      union
      {
         ValueInternalArray::IteratorState array_;
         ValueInternalMap::IteratorState map_;
      } iterator_;
      bool isArray_;
#endif
   };

   /** \brief const iterator for object and array value.
    *
    */
   class ValueConstIterator : public ValueIteratorBase
   {
      friend class Value;
   public:
      typedef unsigned int size_t;
      typedef int difference_type;
      typedef const Value &reference;
      typedef const Value *pointer;
      typedef ValueConstIterator SelfType;

      ValueConstIterator();
   private:
      /*! \internal Use by Value to create an iterator.
       */
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
      explicit ValueConstIterator( const Value::ObjectValues::iterator &current );
#else
      ValueConstIterator( const ValueInternalArray::IteratorState &state );
      ValueConstIterator( const ValueInternalMap::IteratorState &state );
#endif
   public:
      SelfType &operator =( const ValueIteratorBase &other );

      SelfType operator++( int )
      {
         SelfType temp( *this );
         ++*this;
         return temp;
      }

      SelfType operator--( int )
      {
         SelfType temp( *this );
         --*this;
         return temp;
      }

      SelfType &operator--()
      {
         decrement();
         return *this;
      }

      SelfType &operator++()
      {
         increment();
         return *this;
      }

      reference operator *() const
      {
         return deref();
      }
   };


   /** \brief Iterator for object and array value.
    */
   class ValueIterator : public ValueIteratorBase
   {
      friend class Value;
   public:
      typedef unsigned int size_t;
      typedef int difference_type;
      typedef Value &reference;
      typedef Value *pointer;
      typedef ValueIterator SelfType;

      ValueIterator();
      ValueIterator( const ValueConstIterator &other );
      ValueIterator( const ValueIterator &other );
   private:
      /*! \internal Use by Value to create an iterator.
       */
#ifndef WJSON_VALUE_USE_INTERNAL_MAP
      explicit ValueIterator( const Value::ObjectValues::iterator &current );
#else
      ValueIterator( const ValueInternalArray::IteratorState &state );
      ValueIterator( const ValueInternalMap::IteratorState &state );
#endif
   public:

      SelfType &operator =( const SelfType &other );

      SelfType operator++( int )
      {
         SelfType temp( *this );
         ++*this;
         return temp;
      }

      SelfType operator--( int )
      {
         SelfType temp( *this );
         --*this;
         return temp;
      }

      SelfType &operator--()
      {
         decrement();
         return *this;
      }

      SelfType &operator++()
      {
         increment();
         return *this;
      }

      reference operator *() const
      {
         return deref();
      }
   };


} // namespace WJson


#endif // CPPTL_WJSON_H_INCLUDED

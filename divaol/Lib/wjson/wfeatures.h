#ifndef CPPTL_WJSON_FEATURES_H_INCLUDED
# define CPPTL_WJSON_FEATURES_H_INCLUDED

# include "wforwards.h"

namespace WJson {

   /** \brief Configuration passed to reader and writer.
    * This configuration object can be used to force the Reader or Writer
    * to behave in a standard conforming way.
    */
   class WJSON_API Features
   {
   public:
      /** \brief A configuration that allows all features and assumes all strings are UTF-8.
       * - C & C++ comments are allowed
       * - Root object can be any WJson value
       * - Assumes Value strings are encoded in UTF-8
       */
      static Features all();

      /** \brief A configuration that is strictly compatible with the WJson specification.
       * - Comments are forbidden.
       * - Root object must be either an array or an object value.
       * - Assumes Value strings are encoded in UTF-8
       */
      static Features strictMode();

      /** \brief Initialize the configuration like JsonConfig::allFeatures;
       */
      Features();

      /// \c true if comments are allowed. Default: \c true.
      bool allowComments_;

      /// \c true if root must be either an array or an object value. Default: \c false.
      bool strictRoot_;
   };

} // namespace WJson

#endif // CPPTL_WJSON_FEATURES_H_INCLUDED

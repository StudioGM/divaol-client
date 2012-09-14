#ifndef WJSON_CONFIG_H_INCLUDED
# define WJSON_CONFIG_H_INCLUDED

/// If defined, indicates that WJson library is embedded in CppTL library.
//# define WJSON_IN_CPPTL 1

/// If defined, indicates that WJson may leverage CppTL library
//#  define WJSON_USE_CPPTL 1
/// If defined, indicates that cpptl vector based map should be used instead of std::map
/// as Value container.
//#  define WJSON_USE_CPPTL_SMALLMAP 1
/// If defined, indicates that WJson specific container should be used
/// (hash table & simple deque container with customizable allocator).
/// THIS FEATURE IS STILL EXPERIMENTAL!
//#  define WJSON_VALUE_USE_INTERNAL_MAP 1
/// Force usage of standard new/malloc based allocator instead of memory pool based allocator.
/// The memory pools allocator used optimization (initializing Value and ValueInternalLink
/// as if it was a POD) that may cause some validation tool to report errors.
/// Only has effects if WJSON_VALUE_USE_INTERNAL_MAP is defined.
//#  define WJSON_USE_SIMPLE_INTERNAL_ALLOCATOR 1

/// If defined, indicates that WJson use exception to report invalid type manipulation
/// instead of C assert macro.
# define WJSON_USE_EXCEPTION 1

# ifdef WJSON_IN_CPPTL
#  include <cpptl/config.h>
#  ifndef WJSON_USE_CPPTL
#   define WJSON_USE_CPPTL 1
#  endif
# endif

# ifdef WJSON_IN_CPPTL
#  define WJSON_API CPPTL_API
# elif defined(WJSON_DLL_BUILD)
#  define WJSON_API __declspec(dllexport)
# elif defined(WJSON_DLL)
#  define WJSON_API __declspec(dllimport)
# else
#  define WJSON_API
# endif

#endif // WJSON_CONFIG_H_INCLUDED

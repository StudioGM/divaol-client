#ifndef WJSON_AUTOLINK_H_INCLUDED
# define WJSON_AUTOLINK_H_INCLUDED

# include "wconfig.h"

# ifdef WJSON_IN_CPPTL
#  include <cpptl/cpptl_autolink.h>
# endif

# if !defined(WJSON_NO_AUTOLINK)  &&  !defined(WJSON_DLL_BUILD)  &&  !defined(WJSON_IN_CPPTL)
#  define CPPTL_AUTOLINK_NAME "wjson"
#  undef CPPTL_AUTOLINK_DLL
#  ifdef WJSON_DLL
#   define CPPTL_AUTOLINK_DLL
#  endif
#  include "wautolink.h"
# endif

#endif // WJSON_AUTOLINK_H_INCLUDED

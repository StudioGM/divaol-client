#ifndef WJSON_JSON_H_INCLUDED
# define WJSON_JSON_H_INCLUDED

#ifdef _UNICODE	
#define tstring std::wstring
#define tistream std::istream
#else	
#define tstring std::string	
#define tistream std::wistream
#endif

# include "autolink.h"
# include "value.h"
# include "reader.h"
# include "writer.h"

#endif // JSON_JSON_H_INCLUDED

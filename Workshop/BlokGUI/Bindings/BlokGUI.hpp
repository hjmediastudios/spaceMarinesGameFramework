#ifndef _Header_BlokGUI_
#define _Header_BlokGUI_

#ifndef DLL
#	if defined( WIN32 ) || defined( _WINDOWS )
#		define DLL extern "C" __declspec( dllimport )
#	else
#  if defined( __GNUC__ ) && __GNUC__ >= 4
#   define DLL extern "C" __attribute__ ((visibility("default")))
#  else
#		define DLL extern "C"
#  endif
#	endif
#endif

namespace BlokGUI
{
DLL const char* Name()
{
	return "Name";
}

}
#endif

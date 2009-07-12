#ifndef _MACBUNDLEPATH_H_
#define _MACBUNDLEPATH_H_

#include <string>

////////////////////////////////////////////////////////////////////////// 
// macBundlePath
// -------------
// This function will return the directory of your application bundle.
// The applications that you see on OS X are actually folders containing
// the executable, and any resources or frameworks your application uses 
//////////////////////////////////////////////////////////////////////////

std::string macBundlePath();

#endif

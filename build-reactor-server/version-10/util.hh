/******************************
*  Github:yichenyiyan  QAQ    *
*******************************/


#pragma once 

#include <cstdio>
#include <cstdlib>

namespace yichen
{
#define DEBUG(expr, errmsg)  if ((expr)) { perror(errmsg); exit(EXIT_FAILURE); }
}



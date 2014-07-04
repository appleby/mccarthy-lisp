#include "init.h"
#include "alloc.h"
#include "cons.h"
#include "env.h"

namespace mclisp
{

void InitLisp()
{
  static bool initialized = false;
  
  if (initialized)
    return;

  alloc::Init();
  cons::Init();
  env::Init();

  initialized = true;
}

} // mclisp

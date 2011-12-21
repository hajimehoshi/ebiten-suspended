#include "coroutine.hpp"

#define reenter(c) \
  switch (c.value) \
    if (true)      \
    case 0:

#define yield(v)        \
  do {                  \
    c.value = __LINE__; \
    return v;           \
  case __LINE__:        \
    break;              \
  } while(false);

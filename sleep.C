#include <time.h>

inline void _sleep(const unsigned int millisec)
{
  struct timespec tv;
  tv.tv_sec = millisec/1000;
  tv.tv_nsec = (millisec%1000)*1000000;
  nanosleep(&tv,0);
  return;
}

#include "format.h"

#include <string>

using std::string;
using namespace std;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string HH, MM, SS;
  long h = seconds / 3600.0;
  h < 10 ? HH = "0" + to_string(h) : HH = to_string(h);

  long m = (seconds % 3600) / 60.0;
  m < 10 ? MM = "0" + to_string(m) : MM = to_string(m);

  long s = seconds % 60;
  s < 10 ? SS = "0" + to_string(s) : SS = to_string(s);

  return HH + ":" + MM + ":" + SS;
}
#include "processor.h"
#include <string>
#include "linux_parser.h"

using namespace std;

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> utilvect = LinuxParser::CpuUtilization();
  int const timeidle = stoi(utilvect[3]) + stoi(utilvect[4]);  // idle + iowait

  int timetot{0};
  for (auto val : utilvect) {  // sum all values
    timetot += stoi(val);
  }

  if (timetot != 0) return (timetot - timeidle) * 1.0 / timetot;
  return -1;
}
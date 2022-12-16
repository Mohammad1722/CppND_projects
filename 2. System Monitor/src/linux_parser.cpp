#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::ifstream;
using std::istringstream;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  string line, _, memstring;
  long totmem{1}, freemem{1};

  if (filestream.is_open()) {
    getline(filestream, line);
    replace(line.begin(), line.end(), ':', ' ');
    istringstream linestream1(line);
    linestream1 >> _ >> memstring;
    totmem = stol(memstring);

    getline(filestream, line);
    replace(line.begin(), line.end(), ':', ' ');
    istringstream linestream2(line);
    linestream2 >> _ >> memstring;
    freemem = stol(memstring);
  }
  return (totmem - freemem) * 1.0 / totmem;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  ifstream filestream(kProcDirectory + kUptimeFilename);
  string line, uptime{"-1"}, idletime;
  if (filestream.is_open()) {
    if (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> uptime >> idletime;
    }
  }
  return stol(uptime);
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID

// return total runtime of a process
long LinuxParser::TotalTime(int pid) {
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  string line, temp;
  vector<string> vals;

  if (filestream.is_open()) {
    if (getline(filestream, line)) {
      istringstream linestream(line);
      for (int i = 0; i < 18; i++) {
        linestream >> temp;  // get the first 18 values
        vals.push_back(temp);
      }
      return stoi(vals[13]) + stoi(vals[14]) + stoi(vals[15]) + stoi(vals[16]);
    }
  }
  return 1;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> utilvect = LinuxParser::CpuUtilization();
  return stol(utilvect[0]) + stol(utilvect[1]) + stol(utilvect[2]);
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> utilvect = LinuxParser::CpuUtilization();
  return stol(utilvect[3]) + stol(utilvect[4]);
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  ifstream filestream(kProcDirectory + kStatFilename);
  string line, key;

  string user, nice, system, idle, iowait, irq, softirq;
  vector<string> utilvect{};

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);

      linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >>
          softirq;

      if (key == "cpu") {
        utilvect.push_back(user);
        utilvect.push_back(nice);
        utilvect.push_back(system);
        utilvect.push_back(idle);
        utilvect.push_back(iowait);
        utilvect.push_back(irq);
        utilvect.push_back(softirq);
        return utilvect;
      }
    }
  }
  utilvect.push_back("-1");
  return utilvect;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  ifstream filestream(kProcDirectory + kStatFilename);
  string line, key, value;

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;

      if (key == "processes") return stoi(value);
    }
  }
  return -1;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  ifstream filestream(kProcDirectory + kStatFilename);
  string line, key, value;

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;

      if (key == "procs_running") return stoi(value);
    }
  }
  return -1;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line;
  if (filestream.is_open())
    if (getline(filestream, line)) return line;
  return "";
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, value;

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);

      linestream >> key >> value;
      if (key == "VmSize:")
        return to_string(stoi(value) / 1024);  // converted to MBs
    }
  }
  return "0";
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, value;

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> value;

      if (key == "Uid:") return value;
    }
  }
  return "-1";
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  ifstream filestream(kPasswordPath);
  string line, user, x, uid_lhs;
  string uid_rhs{LinuxParser::Uid(pid)};

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);

      linestream >> user >> x >> uid_lhs;
      if (uid_lhs == uid_rhs) return user;
    }
  }
  return "-1";
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  string line, starttime;

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);

      for (int i = 0; i < 22; i++) {
        linestream >> starttime;  // get 22nd value in line
      }
      return UpTime() - stol(starttime) / sysconf(_SC_CLK_TCK);  // convert to seconds
    }
  }
  return -1;
}
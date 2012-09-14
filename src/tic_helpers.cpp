// Copyright 2012 Patrick K. Notz
#include <tic_helpers.hpp>
#include <iostream>
#include <string>

using namespace std;
namespace fs = boost::filesystem;

namespace tic {

void system_cmd(const string& cmd) {
  system(cmd.c_str());
}

string raw_input(const string& prompt) {
  cout << prompt;
  cout.flush();
  string response;
  getline(cin, response);
  return response;
}

fs::path project_root() {
  fs::path pr = fs::current_path();
  const fs::path root = pr.root_path();
  while (pr != root) {
  const fs::path gitdir = pr / ".git";
    if (fs::exists(gitdir) )
      return pr;
    pr = pr.parent_path();
  }
  return fs::path();
}

}  // namespace tic

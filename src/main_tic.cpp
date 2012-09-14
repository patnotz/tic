// Copyright 2012 Patrick K. Notz
#include <fstream>
#include <string>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <pstream.h>
#include <tic_code.hpp>
#include <tic_config.hpp>
#include <tic_log.hpp>
#include <tic_timers.hpp>
#include <tic_version.hpp>

using namespace tic;

namespace po = boost::program_options;

int main(
  int argc,
  char* argv[]) {
  log() << "Version: " << version() << std::endl;

  std::string filename;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("file,f",
	   po::value<std::string>(&filename)->default_value("lines.cpp"),
	   "set file to read")
	 ;
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    log() << desc << std::endl;
    return 1;
  }

  if( ! boost::filesystem::exists(filename) ) {
    log() << boost::format("error: filename does not exist: %s") % filename << std::endl;
    return 1;
  }

  std::ifstream is(filename.c_str());
  std::string str;
  while (std::getline(is, str)) {
    log() << str << std::endl;
  }

  redi::pstream in("ls -ld " + filename);
  while (std::getline(in, str)) {
    log() << str << std::endl;
  }

  return 0;
}

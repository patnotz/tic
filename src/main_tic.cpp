// Copyright 2012 Patrick K. Notz
#include <fstream>
#include <string>
#include <vector>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
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

  std::string makefile;
  std::string target;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("makefile,m",
     po::value<std::string>(&makefile)->default_value("../aria_release_linux-gcc-4.4.4"),
     "Makefile for parsing build commands")
    ("target,t",
     po::value<std::string>(&target)->default_value("Aria_Expression.o"),
     "Target file to inspect")
	 ;
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    log() << desc << std::endl;
    return 1;
  }

  if( ! boost::filesystem::exists(makefile) ) {
    log() << boost::format("error: makefile does not exist: %s") % makefile << std::endl;
    return 1;
  }

  std::ifstream i_makefile(makefile.c_str());
  std::string comp_line;
  boost::regex r_target(target + ":");
  while (std::getline(i_makefile, comp_line)) {
    if(boost::regex_search(comp_line.c_str(), r_target)) {
      // The next comp_line contains the compilation command, with all the yummy -D and -I flags
      std::getline(i_makefile, comp_line);
      log() << comp_line << std::endl;
      break;
    }
  }
  i_makefile.close();

  std::vector<std::string> include_paths;
  include_paths.reserve(128);
  boost::regex r_path("-I\"(.*?)\"");
  for(boost::sregex_iterator i(comp_line.begin(), comp_line.end(), r_path), end; i != end; ++i) {
    const std::string path = (*i)[1];
    bool valid = boost::filesystem::is_directory(path);
    if(valid)
      include_paths.push_back(path);
    else
      log() << "invalid path: " << path << std::endl;
  }
  log() << boost::format("found %d valid include paths") % include_paths.size() << std::endl;

  boost::regex r_src("^(.*)\\.o$");
  boost::cmatch m_src;
  std::string source;
  if(boost::regex_match(target.c_str(), m_src, r_src)) {
    source = "/Users/pknotz/workspace/code/aria/src/" + m_src[1] + ".C";
  }
  log() << "source file: " << source << std::endl;

  std::ifstream i_source(source.c_str());
  boost::regex r_include("#include [<\"](.*)[>\"]");
  boost::cmatch m_include;
  std::vector<std::string> includes;
  std::string src_line;
  while (std::getline(i_source, src_line)) {
    if(boost::regex_match(src_line.c_str(), m_include, r_include)) {
      log() << "found " << m_include[1] << std::endl;
      includes.push_back(m_include[1]);
    }
  }
  i_source.close();

  /*
  redi::pstream in("ls -ld " + makefile);
  while (std::getline(in, str)) {
    log() << str << std::endl;
  }
  */

  return 0;
}

// Copyright 2012 Patrick K. Notz
#ifndef TIC_HELPERS_HPP_
#define TIC_HELPERS_HPP_

#include <boost/filesystem.hpp>
#include <string>

namespace tic {

void system_cmd(const std::string& cmd);

std::string raw_input(const std::string& prompt);

boost::filesystem::path project_root();

}  // namespace tic

#endif  /* TIC_HELPERS_HPP_ */

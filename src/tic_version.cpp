// Copyright 2012 Patrick K. Notz
#include <tic_autoversion.hpp>
#include <tic_version.hpp>

namespace tic {

std::string version() {
  std::string v = "v";
  return v + TIC_VERSION;
}

}  // namespace tic

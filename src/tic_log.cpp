// Copyright 2012 Patrick K. Notz
#include <tic_log.hpp>

namespace tic {

static std::ostream * s_log = &std::cout;

void set_log(std::ostream &os) {
  s_log = &os;
}

std::ostream &log() {
  return *s_log;
}

}  // namespace tic

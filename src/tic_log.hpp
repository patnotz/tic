// Copyright 2012 Patrick K. Notz
#ifndef TIC_LOG_HPP_
#define TIC_LOG_HPP_

#include <iostream>

namespace tic {

void set_log(std::ostream & os);

std::ostream & log();

}  // namespace tic

#endif  /* TIC_LOG_HPP_ */

// Copyright 2012 Patrick K. Notz
#ifndef TIC_TIMERS_HPP_
#define TIC_TIMERS_HPP_
#include <string>
#include <iostream>
#include <boost/chrono.hpp>
#include <boost/format.hpp>

namespace tic {

typedef boost::chrono::high_resolution_clock timer_clock;
typedef timer_clock::time_point timer_point;

// simple duration as floating point number of seconds
typedef boost::chrono::duration<double> timer_duration;

// alternative for integral nanoseconds
// typedef timer_clock::duration timer_duration;

//
// A simple RAII-based timer that prints
// the timer duration to an ostream on destruction
//
class simple_output_timer {
public:
  explicit simple_output_timer(
      const std::string & format = "timer: time = %s",
      std::ostream & os = std::cout)
    : m_format(format),
      m_os(os),
      m_start(timer_clock::now()) {}

  ~simple_output_timer() {
	 timer_duration dt = timer_clock::now() - m_start;
	 m_os << boost::format(m_format) % dt << std::endl;
  }

private:
  const std::string m_format;
  std::ostream & m_os;
  const timer_point m_start;
};

//
// A simple RAII-based timer that captures
// the timer duration in a user supplied variable
//
class scope_timer {
public:
  explicit scope_timer(timer_duration & dt)
    : m_dt(dt),
      m_start(timer_clock::now()) {}

  ~scope_timer() {
	 m_dt = timer_clock::now() - m_start;
  }

  timer_duration so_far() {
    const timer_duration dt_so_far = timer_clock::now() - m_start;
	 return dt_so_far;
  }

private:
  timer_duration & m_dt;
  const timer_point m_start;
};

}  // namespace tic

#endif  /* TIC_TIMERS_HPP_ */

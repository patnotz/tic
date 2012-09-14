// Copyright 2012 Patrick K. Notz
#include <gtest/gtest.h>
#include <tic_code.hpp>
#include <tic_config.hpp>
#include <tic_log.hpp>
#include <tic_timers.hpp>

TEST(tic, timers) {
  tic::timer_duration dt;
  {
    tic::scope_timer t2(dt);
    tic::simple_output_timer t1("tic::simple_output_timer dt = %s");
    sleep(1);
  }
  tic::log() << "tic::scope_timer dt = " << dt << std::endl;
}

TEST(tic, pass) {
}

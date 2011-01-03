check_PROGRAMS = \
  manager.test \
  profiler.test \
  statistical_profiler.test \
  default_initialization.test \
  debug_mode.test \
  env.test

manager_test_SOURCES = \
  manager_test.cpp
manager_test_CXXFLAGS = \
  -I$(top_srcdir)/src
manager_test_LDADD = \
  -lgtest_main $(top_builddir)/src/libmtkw.la

profiler_test_SOURCES = \
  profiler_test.cpp
profiler_test_CXXFLAGS = \
  -I$(top_srcdir)/src
profiler_test_LDADD = \
  -lgtest $(top_builddir)/src/libmtkw.la

statistical_profiler_test_SOURCES = \
  profiler_test.cpp
statistical_profiler_test_CXXFLAGS = \
  -I$(top_srcdir)/src -DSTATISTICAL_PROFILER_TEST
statistical_profiler_test_LDADD = \
  -lgtest $(top_builddir)/src/libmtkw.la

default_initialization_test_SOURCES = \
  default_initialization_test.cpp
default_initialization_test_CXXFLAGS = \
  -I$(top_srcdir)/src
default_initialization_test_LDADD = \
  -lgtest_main $(top_builddir)/src/libmtkw.la

debug_mode_test_SOURCES = \
  debug_mode_test.cpp
debug_mode_test_CXXFLAGS = \
  -I$(top_srcdir)/src
debug_mode_test_LDADD = \
  -lgtest_main $(top_builddir)/src/libmtkw.la

env_test_SOURCES = \
  env_test.cpp
env_test_CXXFLAGS = \
  -I$(top_srcdir)/src
env_test_LDADD = \
  -lgtest_main $(top_builddir)/src/libmtkw.la

TESTS = $(check_PROGRAMS)
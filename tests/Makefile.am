check_PROGRAMS = \
  manager.test \
  profiler.test \
  statistical_profiler.test

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

TESTS = $(check_PROGRAMS)
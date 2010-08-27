check_PROGRAMS = \
  thread_local_manager.test

thread_local_manager_test_SOURCES = \
  thread_local_manager_test.cpp
thread_local_manager_test_CXXFLAGS = \
  -I$(top_srcdir)/src -DMTKW_PFI_INTERNAL
thread_local_manager_test_LDADD = \
  -lgtest_main $(top_builddir)/src/libmtkw.la

TESTS = $(check_PROGRAMS)
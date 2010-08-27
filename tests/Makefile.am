check_PROGRAMS = \
  manager.test

manager_test_SOURCES = \
  manager_test.cpp
manager_test_CXXFLAGS = \
  -I$(top_srcdir)/src -DMTKW_PFI_INTERNAL
manager_test_LDADD = \
  -lgtest_main $(top_builddir)/src/libmtkw.la

TESTS = $(check_PROGRAMS)
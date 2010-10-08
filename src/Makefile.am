lib_LTLIBRARIES = libmtkw.la

libmtkw_la_includedir = $(includedir)/mtkw
libmtkw_la_include_HEADERS = \
  profile.hpp \
  profiler.hpp \
  manager.hpp \
  util.hpp \
  thread.hpp

libmtkw_la_SOURCES = \
  profile.hpp \
  profile.cpp \
  profiler.hpp \
  profiler.cpp \
  thread_local_storage.hpp \
  thread_local_storage.cpp \
  thread_local_manager.hpp \
  thread_local_manager.cpp \
  manager.hpp \
  manager.cpp \
  time.hpp \
  time.cpp \
  util.hpp \
  thread.hpp

libmtkw_la_CXXFLAGS =
libmtkw_la_LIBADD =

if MTKW_WITH_GLOG
libmtkw_la_LIBADD += -lglog
endif

libmtkw_la_SOURCES += \
  pthread_local_storage.hpp \
  pthread_local_storage.cpp
libmtkw_la_LIBADD += -lpthread

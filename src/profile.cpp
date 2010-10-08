#include "profile.hpp"

#include <sstream>

namespace mtkw {

void Profile::getStatistics(std::map<std::string, ProfileStatistics>& statistics) const {
  if (generate_statistics) statistics[name].add(*this);
  for (size_t i = 0; i < subprofiles.size(); i++) {
    subprofiles[i]->getStatistics(statistics);
  }
}

void Profile::simpleFormat(std::ostream& out,
                           const std::string& indent,
                           const std::string& initial_indent) const {
  out << initial_indent << name;
  out << " (" << time() << " sec)" << std::endl;

  std::string next_indent = initial_indent;
  next_indent += indent;
  for (size_t i = 0; i < subprofiles.size(); i++) {
    subprofiles[i]->simpleFormat(out, indent, next_indent);
  }
}

std::string Profile::simpleFormat(const std::string& indent,
                                  const std::string& initial_indent) const {
  std::ostringstream out;
  simpleFormat(out, indent, initial_indent);
  return out.str();
}

ProfileStatistics::ProfileStatistics()
  : _called(0), _total(0), _max(0), _min(0) {
}

ProfileStatistics::ProfileStatistics(const ProfileStatistics& s) {
  thread::rlock lk(s._mutex);
  _called = s._called;
  _total = s._total;
  _max = s._max;
  _min = s._min;
}

ProfileStatistics::~ProfileStatistics() {
}

ProfileStatistics& ProfileStatistics::operator =(const ProfileStatistics& s) {
  if (this == &s) return *this;
  ProfileStatistics tmp(s);

  thread::wlock lk(_mutex);
  tmp.swapImpl(*this); // tmp does not need lock
  return *this;
}

void ProfileStatistics::add(const Profile& prof) {
  thread::wlock lk(_mutex);
  _called++;

  const double t = prof.time();
  _total += t;
  _max = std::max(_max, t);
  _min = std::min(_min, t);
}

size_t ProfileStatistics::called() const {
  thread::rlock lk(_mutex);
  return _called;
}

double ProfileStatistics::average() const {
  thread::rlock lk(_mutex);
  return _called ? _total / _called : 0;
}

double ProfileStatistics::max() const {
  thread::rlock lk(_mutex);
  return _max;
}

double ProfileStatistics::min() const {
  thread::rlock lk(_mutex);
  return _min;
}

void ProfileStatistics::swapImpl(ProfileStatistics& s) {
  std::swap(_called, s._called);
  std::swap(_total, s._total);
  std::swap(_max, s._max);
  std::swap(_min, s._min);
}

void ProfileStatistics::swap(ProfileStatistics& s) {
  if (this == &s) return;

  // two locks must be acquired in the consistent order for any pair of locks
  if (this < &s) {
    thread::wlock this_lk(_mutex);
    thread::wlock other_lk(s._mutex);
    swapImpl(s);

  } else {
    thread::wlock other_lk(s._mutex);
    thread::wlock this_lk(_mutex);
    swapImpl(s);
  }
}

} // namespace mtkw

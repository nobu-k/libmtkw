#include "profile.hpp"

#include <sstream>

namespace mtkw {

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

ProfileStatistics::ProfileStatistics(const std::string& name)
  : _name(name), _called(0), _total(0), _max(0), _min(0) {
}

ProfileStatistics::~ProfileStatistics() {
}

void ProfileStatistics::add(const Profile& prof) {
  if (_name != prof.name) return; // not counted

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

} // namespace mtkw

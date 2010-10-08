#include "profile.hpp"

#include <sstream>
#include <limits>

namespace mtkw {

void Profile::getStatistics(ProfileStatistics& statistics) const {
  if (generate_statistics) statistics.add(*this);
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

SingleProfileStatistics::SingleProfileStatistics()
  : _called(0), _total(0)
  ,  _max(-std::numeric_limits<double>::infinity())
  ,  _min(std::numeric_limits<double>::infinity()) {
}

SingleProfileStatistics::~SingleProfileStatistics() {
}

void SingleProfileStatistics::add(const Profile& prof) {
  _called++;

  const double t = prof.time();
  _total += t;
  _max = std::max(_max, t);
  _min = std::min(_min, t);
}

void SingleProfileStatistics::swap(SingleProfileStatistics& s) {
  std::swap(_called, s._called);
  std::swap(_total, s._total);
  std::swap(_max, s._max);
  std::swap(_min, s._min);
}

ProfileStatistics::ProfileStatistics() {
}

ProfileStatistics::~ProfileStatistics() {
}

size_t ProfileStatistics::size() const {
  thread::rlock lk(_mutex);
  return _statistics.size();
}

void ProfileStatistics::addImpl(Statistics& st, const Profile& prof) {
  thread::wlock lk(st.mutex);
  st.stat.add(prof);
}

void ProfileStatistics::add(const Profile& prof) {
  { // check if name already exists
    thread::rlock lk(_mutex);
    StatisticsMap::iterator it = _statistics.find(prof.name);
    if (it != _statistics.end()) {
      return addImpl(it->second, prof);
    }
  }

  // Other threads may create a new entry for prof.name
  // earlier than this thread.
  // boost::upgrade_lock may be good to avoid this redundant write-lock.

  { // create a new entry
    thread::wlock lk(_mutex);
    addImpl(_statistics[prof.name], prof);
  }
}

int ProfileStatistics::get(const std::string& name, SingleProfileStatistics& result) const {
  thread::rlock lk(_mutex);
  StatisticsMap::const_iterator it = _statistics.find(name);
  if (it == _statistics.end()) return -1; // no entry

  const Statistics& st = it->second;
  thread::rlock stlk(st.mutex);
  result = st.stat;
  return 0;
}

void ProfileStatistics::getAll(std::map<std::string, SingleProfileStatistics>& result) const {
  thread::rlock lk(_mutex);
  StatisticsMap::const_iterator it = _statistics.begin(), end = _statistics.end();
  for (; it != end; ++it) {
    const std::string& name = it->first;
    const Statistics& st = it->second;
    thread::rlock stlk(st.mutex);
    result[name] = st.stat;
  }
}

void ProfileStatistics::clear() {
  thread::wlock lk(_mutex);
  _statistics.clear();
}

} // namespace mtkw

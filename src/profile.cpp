#include "profile.hpp"

#include <sstream>
#include <limits>

namespace mtkw {

void Profile::accept(ProfileVisitor& visitor) const {
  visitor.visit(*this);
  for (size_t i = 0; i < subprofiles.size(); i++) {
    subprofiles[i]->accept(visitor);
  }
}

void Profile::accept(ProfileTraversingVisitor& visitor) const {
  visitor.visit(*this);
  // subprofiles are traversed by visitor
}

void Profile::iterateSubprofiles(ProfileTraversingVisitor& visitor) const {
  for (size_t i = 0; i < subprofiles.size(); i++) {
    subprofiles[i]->accept(visitor);
  }
}

namespace {
class SimpleFormattingVisitor : public ProfileTraversingVisitor {
private:
  std::ostream& _out;
  const std::string& _indent_delta;
  std::string _cur_indent;

public:
  SimpleFormattingVisitor(std::ostream& out, const std::string& delta,
                          const std::string& cur)
    : _out(out), _indent_delta(delta), _cur_indent(cur) {}

  virtual void visit(const Profile& profile) {
    _out << _cur_indent << profile.name;
    _out << " (" << profile.time() << " sec)" << std::endl;

    std::string prev_indent = _cur_indent;
    _cur_indent += _indent_delta;
    profile.iterateSubprofiles(*this);
    _cur_indent = prev_indent;
  }
};
} // namespace

void Profile::simpleFormat(std::ostream& out,
                           const std::string& indent,
                           const std::string& initial_indent) const {
  SimpleFormattingVisitor v(out, indent, initial_indent);
  accept(v);
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

namespace {
struct StatisticsProfileVisitor : public ProfileVisitor {
private:
  ProfileStatistics& _stat;

public:
  StatisticsProfileVisitor(ProfileStatistics& s) : _stat(s) {}
  virtual void visit(const Profile& prof) {
    _stat.add(prof);
  }
};
} // namespace

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

void ProfileStatistics::addAll(const Profile& prof) {
  StatisticsProfileVisitor v(*this);
  prof.accept(v);
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

void ProfileStatistics::getAll(std::map<std::string, SingleProfileStatistics>& results) const {
  thread::rlock lk(_mutex);
  StatisticsMap::const_iterator it = _statistics.begin(), end = _statistics.end();
  for (; it != end; ++it) {
    const std::string& name = it->first;
    const Statistics& st = it->second;
    thread::rlock stlk(st.mutex);
    results[name] = st.stat;
  }
}

void ProfileStatistics::clear() {
  thread::wlock lk(_mutex);
  _statistics.clear();
}

} // namespace mtkw

#include "profile.hpp"

#include <sstream>

namespace mtkw {

void Profile::simpleFormat(std::ostream& out,
                           const std::string& indent,
                           const std::string& initial_indent) const {
  out << initial_indent << name;
  out << "(" << elapsed() << " sec)" << std::endl;

  std::string next_indent = initial_indent;
  next_indent += indent;
  for (size_t i = 0; i < subprofiles.size(); i++) {
    subprofiles[i]->simpleFormat(indent, next_indent);
  }
}

std::string Profile::simpleFormat(const std::string& indent,
                                  const std::string& initial_indent) const {
  std::ostringstream out;
  simpleFormat(out, indent, initial_indent);
  return out.str();
}

} // namespace mtkw

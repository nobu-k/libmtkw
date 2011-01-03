#ifndef LIBMTKW_SRC_FLAGS_HPP_
#define LIBMTKW_SRC_FLAGS_HPP_

namespace mtkw {

struct Flags {
  bool enabled;
  bool debug_mode;

  Flags() : enabled(false), debug_mode(false) {}
};

} // namespace mtkw

#endif

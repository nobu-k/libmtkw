#include "manager.hpp"

namespace mtkw {

Manager* Manager::_instance;

Manager::Manager() {
}

Manager::~Manager() {
}

int Manager::createProfile(const std::string& root_name) {
  return _tl_manager->get()->createProfile(root_name);
}

int Manager::beginProfile(const std::string& name) {
  return _tl_manager->get()->beginProfile(name);
}

int Manager::endProfile(const std::string& name) {
  return _tl_manager->get()->endProfile(name);
}

int Manager::appendProfile(const Profile& p) {
  return _tl_manager->get()->appendProfile(p);
}

} // namespace mtkw

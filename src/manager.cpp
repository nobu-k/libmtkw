#include "manager.hpp"

#include "thread_local_storage.hpp"
#include "thread_local_manager.hpp"

namespace mtkw {

Manager* Manager::_instance;

Manager::Manager() {
}

Manager::~Manager() {
}

int Manager::initialize(ThreadLocalStorage* storage) {
  if (storage == NULL) {
    LOG(ERROR) << "storage is NULL.";
    return -1;
  }
  if (_tls) {
    LOG(ERROR) << "Manager is already initialized.";
    return -1;
  }

  _tls.reset(storage);
  return -1;
}

ThreadLocalManager* Manager::getTlsMgr() {
  ThreadLocalManager* mgr = _tls->get();
  if (mgr == NULL) {
    LOG(ERROR) << "Failed to get thread local manager.";
    return NULL;
  }
  return mgr;
}

int Manager::enable(bool e) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return -1;
  return mgr->enable(e);
}

int Manager::disable() {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return -1;
  return mgr->disable();
}

int Manager::isEnabled() const {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return -1;
  return mgr->isEnabled();
}

int Manager::beginProfile(const std::string& name) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return -1;
  return mgr->beginProfile(name);
}

int Manager::endProfile(const std::string& name) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return -1;
  return mgr->endProfile(name);
}

int Manager::appendProfile(const ProfilePtr& p) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return -1;
  return mgr->appendProfile(p);
}

ProfilePtr Manager::getLastProfile() const {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return ProfilePtr();
  return mgr->getLastProfile();
}

int Manager::setMessage(const std::string& msg) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return ProfilePtr();
  return mgr->setMessage(msg);
}

int Manager::appendMessage(const std::string& msg) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (mgr == NULL) return ProfilePtr();
  return msg->appendMessage(msg);
}

} // namespace mtkw

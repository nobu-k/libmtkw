#include "manager.hpp"

#include "profiler.hpp"
#include "thread_local_storage.hpp"
#include "thread_local_manager.hpp"

namespace mtkw {

Manager* Manager::_instance;

Manager::Manager() {
}

Manager::~Manager() {
}

int Manager::initialize(ThreadLocalStorage* storage) {
  if (!storage) {
    LOG(ERROR) << "storage is NULL.";
    return -1;
  }

  if (_instance) {
    LOG(WARNING) << "Manager is already initialized. "
                 << "Given ThreadLocalStorage will be deleted.";
    delete storage;
    return 0;
  }

  Manager* ist = new Manager();
  ist->_tls.reset(storage);
  _instance = ist;
  return 0;
}

int Manager::uninitializedCreate() {
  LOG(WARNING) << "Call initialize() before calling instance(). "
               << "libmtkw is initialized with the default configuration.";
  return ::mtkw::initialize();
}

int Manager::resetThreadLocalStorage(ThreadLocalStorage* storage) {
  if (!_instance) {
    LOG(ERROR) << "Manager is not initialized yet.";
    return -1;
  }
  _instance->_tls.reset(storage);
  return 0;
}

ThreadLocalManager* Manager::getTlsMgr() const {
  ThreadLocalManager* mgr = _tls->get();
  if (!mgr) {
    LOG(ERROR) << "Failed to get thread local manager.";
    return NULL;
  }
  return mgr;
}

int Manager::enable(bool e) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return -1;
  return mgr->enable(e);
}

int Manager::disable() {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return -1;
  return mgr->disable();
}

bool Manager::isEnabled() const {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return false;
  return mgr->isEnabled();
}

int Manager::beginProfile(const std::string& name, bool gen_stat) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return -1;
  return mgr->beginProfile(name, gen_stat);
}

int Manager::endProfile() {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return -1;
  if (mgr->endProfile() != 0) return -1;

  // The last profile was the root. Update statistics.
  if (!mgr->getCurrentProfile()) {
    ProfilePtr ptr = mgr->getLastProfile();
    if (ptr) ptr->getStatistics(_statistics);
  }
  return 0;
}

int Manager::appendProfile(const ProfilePtr& p) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return -1;
  return mgr->appendProfile(p);
}

int Manager::setDebugLog(const std::string& msg) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return -1;
  return mgr->setDebugLog(msg);
}

int Manager::appendDebugLog(const std::string& msg) {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return -1;
  return mgr->appendDebugLog(msg);
}

ProfilePtr Manager::getCurrentProfile() const {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return ProfilePtr();
  return mgr->getCurrentProfile();
}

ProfilePtr Manager::getLastProfile() const {
  ThreadLocalManager* mgr = getTlsMgr();
  if (!mgr) return ProfilePtr();
  return mgr->getLastProfile();
}

void Manager::addStatistics(const ProfilePtr& p) {
  p->getStatistics(_statistics);
}

void Manager::clearStatistics() {
  _statistics.clear();
}

} // namespace mtkw

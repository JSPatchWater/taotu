/**
 * @file spin_lock.h
 * @author Sigma711 (sigma711 at foxmail dot com)
 * @brief  // TODO:
 * @date 2021-12-16
 *
 * @copyright Copyright (c) 2021 Sigma711
 *
 */

#ifndef TAOTU_TAOTU_SPIN_LOCK_H_
#define TAOTU_TAOTU_SPIN_LOCK_H_

#include <atomic>

#include "non_copyable_movable.h"

namespace taotu {

/**
 * @brief  // TODO:
 *
 */
class MutexLock : NonCopyableMovable {
 public:
  MutexLock() : val_(true) {}
  ~MutexLock() { Unlock(); }

  void Lock() {
    bool exp = true;
    while (!val_.compare_exchange_weak(exp, false)) {
      exp = true;
    }
  }
  void Unlock() { val_.store(true); }

 private:
  std::atomic_bool val_;
};

/**
 * @brief  // TODO:
 *
 */
class LockGuard : NonCopyableMovable {
 public:
  LockGuard(MutexLock& mutex_lock) : mutex_lock_(mutex_lock) {
    mutex_lock_.Lock();
  }
  ~LockGuard() { mutex_lock_.Unlock(); }

 private:
  MutexLock& mutex_lock_;
};

}  // namespace taotu

#endif  // !TAOTU_TAOTU_SPIN_LOCK_H_
/**
 * @file event_manager.h
 * @author Sigma711 (sigma711 at foxmail dot com)
 * @brief  // TODO:
 * @date 2021-12-03
 *
 * @copyright Copyright (c) 2021 Sigma711
 *
 */

#ifndef TAOTU_TAOTU_EVENT_MANAGER_H_
#define TAOTU_TAOTU_EVENT_MANAGER_H_

#include <stdint.h>

#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include "connecting.h"
#include "net_address.h"
#include "non_copyable_movable.h"
#include "poller.h"
#include "spin_lock.h"
#include "time_point.h"
#include "timer.h"

namespace taotu {

/**
 * @brief  // TODO:
 *
 */
class EventManager : NonCopyableMovable {
 public:
  EventManager();
  ~EventManager();

  void Loop();

  void InsertNewConnection(int socket_fd, const NetAddress& local_address,
                           const NetAddress& peer_address);

  Poller* GetPoller() { return poller_.get(); }

  // For the Balancer to pick a EventManager with lowest load
  uint32_t GetEventerAmount() { return eventer_amount_; }

  void RunAt(TimePoint time_point, Timer::TimeCallback TimeTask);
  void RunAfter(int64_t delay_microseconds, Timer::TimeCallback TimeTask);
  void RunEveryUntil(
      int64_t interval_microseconds, Timer::TimeCallback TimeTask,
      std::function<bool()> IsContinue = std::function<bool()>{});

 private:
  typedef std::unordered_map<int, std::unique_ptr<Connecting>> EventerMap;

  void DoExpiredTimeTasks();
  void DestroyClosedConnections();

  std::unique_ptr<Poller> poller_;
  EventerMap eventer_map_;
  std::unique_ptr<std::thread> thread_;
  Timer timer_;

  MutexLock eventer_map_mutex_lock_;

  // For the Balancer to pick a EventManager with lowest load
  uint32_t eventer_amount_;

  bool is_looping_;
  bool should_quit_;
  bool is_doing_with_tasks_;

  Poller::EventerList active_events_;
  std::vector<int> closed_fds;
};

}  // namespace taotu

#endif  // !TAOTU_TAOTU_EVENT_MANAGER_H_

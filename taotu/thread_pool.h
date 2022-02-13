/**
 * @file thread_pool.h
 * @author Sigma711 (sigma711 at foxmail dot com)
 * @brief  // TODO:
 * @date 2022-02-13
 *
 * @copyright Copyright (c) 2022 Sigma711
 *
 */

#ifndef TAOTU_TAOTU_THREAD_POOL_H_
#define TAOTU_TAOTU_THREAD_POOL_H_

#include <stdlib.h>

#include <array>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "non_copyable_movable.h"

namespace taotu {

/**
 * @brief  // TODO:
 *
 */
class ThreadPool : NonCopyableMovable {
 public:
  ThreadPool(int thread_amount);
  ~ThreadPool();

  void AddTask(std::function<void()> task);

 private:
  std::vector<std::unique_ptr<std::thread>> threads_;
  std::array<std::queue<std::function<void()>>, 2> task_queues_;

  size_t cur_que_idx_;

  std::mutex pdt_csm_mutex_;
  std::condition_variable pdt_csm_cond_var_;

  std::mutex que_exc_mutex_;

  bool should_stop_;
};

}  // namespace taotu

#endif  // !TAOTU_TAOTU_THREAD_POOL_H_
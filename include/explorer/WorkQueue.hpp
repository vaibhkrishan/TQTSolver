#pragma once

#include <queue>

#include "explorer/WorkItem.hpp"

class WorkQueue {
  public:

    void push(
        const WorkItem& item
        );

    bool pop(
        WorkItem& item
        );

    bool empty() const;

  private:

    std::queue<WorkItem> queue_;
}; // class WorkQueue

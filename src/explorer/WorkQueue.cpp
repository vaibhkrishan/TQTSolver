#include "explorer/WorkQueue.hpp"

void
WorkQueue::push(
    const WorkItem& item
    ) {
  queue_.push(item);
}

bool
WorkQueue::pop(
    WorkItem& item
    ) {

  if (queue_.empty()) {
    return false;
  }

  item = queue_.front();

  queue_.pop();

  return true;
}

bool
WorkQueue::empty() const {

  return queue_.empty();
}

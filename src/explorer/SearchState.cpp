#include "explorer/SearchState.hpp"

NodeID
SearchState::allocate_node_id() {

  return next_node_id++;
}

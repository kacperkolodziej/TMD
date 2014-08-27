#include "debug_gui.hpp"
#ifdef DEBUG
std::recursive_mutex debug::write_lock_;
#endif

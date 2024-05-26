#include "state_abstract.hpp"

uint64_t StateAbstract::ms_since_boot = 0;
system_state_t StateAbstract::current_state = sm_system_off;
system_state_t StateAbstract::previous_state = sm_system_off;

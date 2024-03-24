#pragma once

#include <ostream>

#include "utils/alloc.hpp"
#include "utils/time.hpp"

void test_lazy_matrix (std::ostream &os);

class tester_t
{
        private:
                mem_tracker_t  &mem_tracker;
                time_tracker_t time_tracker {};

        public:
                tester_t (mem_tracker_t &mem_tracker_):
                        mem_tracker(mem_tracker_) {}

                using duration = std::chrono::duration<double, std::milli>;

                void enable_mem_tracker  () { mem_tracker.enable();  }
                void disable_mem_tracker () { mem_tracker.disable(); }
                void start_timer         () { time_tracker.start();  }
                void end_timer           () { time_tracker.end();    }
                duration get_delta_time  () { return time_tracker.get_delta_time(); }
                void dump (std::ostream &os, const std::string &msg = "")
                { mem_tracker.dump(os, msg); time_tracker.dump(os, msg); os << std::endl; }
                void dump_mem (std::ostream &os, const std::string &msg = "")
                { mem_tracker.dump(os, msg); os << std::endl; }
                void dump_time (std::ostream &os, const std::string &msg = "")
                { time_tracker.dump(os, msg); os << std::endl; }

                void reset_used_mem_cnt ()      { mem_tracker.reset_used_mem_cnt(); }
                void* tracked_new (size_t size) { mem_tracker.tracked_new(size); }
                void tracked_delete (void* ptr) { mem_tracker.tracked_delete(ptr); }
};

inline tester_t TESTER {MEMORY_TRACKER};
#pragma once

#include <chrono>

class time_tracker_t
{
        public:
                using hr_time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
                using hr_clock      = std::chrono::high_resolution_clock;
                using duration      = std::chrono::duration<double, std::milli>;
                using milliseconds  = std::chrono::milliseconds;
        private:
                hr_time_point t1 {};
                hr_time_point t2 {};
                duration delta_time {};
        public:
                void start () { t1 = hr_clock::now(); }
                void end   () { t2 = hr_clock::now(); delta_time = t2 - t1; }
                duration get_delta_time () { return delta_time; }
                void dump (std::ostream &os, const std::string &msg = "")
                {
                        os << msg << std::endl;
                        os << "Duration: " << delta_time.count() << "ms" << std::endl;
                }
};
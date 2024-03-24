#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>

class mem_tracker_t 
{
        private:
                std::unordered_map<size_t, size_t> news {};
                size_t mem_alloced = 0;
                size_t mem_used = 0;

                bool enable_tracking = false;

        public:
                void enable  () { enable_tracking = true; }
                void disable () { enable_tracking = false; }
                size_t get_mem_alloced () { return mem_alloced; }
                size_t get_used_mem () { return mem_used; }

                size_t reset_used_mem_cnt () { return std::exchange(mem_used, 0); }

                void fixate_new (const void *ptr, size_t size)
                {
                        enable_tracking = false;

                        auto id = calc_id(ptr);
                        news[id] = size;
                        mem_alloced += size;
                        mem_used += size;
                        
                        enable_tracking = true;
                }

                void fixate_delete (const void *ptr)
                {
                        enable_tracking = false;

                        mem_alloced -= news[reinterpret_cast<size_t>(ptr)];
                        news.erase(calc_id(ptr));
                        
                        enable_tracking = true;
                }

                void* tracked_new (size_t size)
                {
                        void *ptr = nullptr;

                        if (enable_tracking) {
                                ptr = malloc(size);
                                fixate_new(ptr, size);
                        } else {
                                ptr = malloc(size);
                        }

                        if (!ptr)
                                throw std::bad_alloc();
                        return ptr;
                }

                void tracked_delete (void *ptr)
                {
                        free(ptr);
                        if (enable_tracking)
                                fixate_delete(ptr);
                }

                size_t calc_id (const void *ptr)
                {
                    // maybe some hashfunc?
                    return reinterpret_cast<size_t>(ptr);
                }

                void dump (std::ostream &os, const std::string &msg = "")
                {
                        os << msg                                 << std::endl;
                        os << "Allocated memory: " << mem_alloced << std::endl;
                        os << "Used memory: "      << mem_used    << std::endl;
                }
};

// --------------------------------------- GLOBAL ---------------------------------------------------------------------

inline mem_tracker_t MEMORY_TRACKER {};

inline void* operator new (size_t size)
{
        return MEMORY_TRACKER.tracked_new(size);
}

inline void* operator new[] (size_t size)
{
        return MEMORY_TRACKER.tracked_new(size);
}

inline void operator delete (void* ptr)
{
        MEMORY_TRACKER.tracked_delete(ptr);
}

inline void operator delete[] (void* ptr)
{
        MEMORY_TRACKER.tracked_delete(ptr);
}

inline void operator delete (void* ptr, size_t size)
{
        MEMORY_TRACKER.tracked_delete(ptr);
}

inline void operator delete[] (void* ptr, size_t size)
{
        MEMORY_TRACKER.tracked_delete(ptr);
}

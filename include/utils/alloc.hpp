#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

class mem_tracker_t 
{
        private:
                std::unordered_map<size_t, size_t> news {};
                size_t mem_alloced = 0;

                bool enable_tracking = false;

        public:
                void enable  () { enable_tracking = true; }
                void disable () { enable_tracking = false; }
                size_t get_mem_alloced () { return mem_alloced; }

                void fixate_new (const void *ptr, size_t size)
                {
                        enable_tracking = false;

                        auto id = calc_id(ptr);
                        news[id] = size;
                        mem_alloced += size;
                        
                        enable_tracking = true;
                }

                void fixate_delete (const void *ptr, size_t size)
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

                void tracked_delete (void *ptr, size_t size = 0)
                {
                        free(ptr);
                        if (enable_tracking)
                                fixate_delete(ptr, size);
                }

                size_t calc_id (const void *ptr)
                {
                    // maybe some hashfunc?
                    return reinterpret_cast<size_t>(ptr);
                }

                void dump (const std::string &msg, std::ostream &os)
                {
                        os << msg << mem_alloced << std::endl;
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
        MEMORY_TRACKER.tracked_delete(ptr, size);
}

inline void operator delete[] (void* ptr, size_t size)
{
        MEMORY_TRACKER.tracked_delete(ptr, size);
}

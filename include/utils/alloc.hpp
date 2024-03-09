#pragma once

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class mem_obj_t 
{
        private:
                std::vector<size_t> sizes {};
        public:
                void push_size (size_t size) { sizes.push_back(size); }

                size_t& operator[] (size_t index) 
                {
                        if (index >= sizes.size())
                                throw std::out_of_range("?");
                        return sizes[index];
                }
};

class mem_tracker_t 
{
        private:
                std::unordered_map<int, mem_obj_t> news    {};
                std::unordered_map<int, mem_obj_t> deletes {};

                bool enable_tracking = false;

        public:
                void enable () { enable_tracking = true; }

                void fixate_new (const void *ptr, size_t size)
                {
                        auto id = create_id(ptr);
                        news.try_emplace(id);
                        news[id].push_size(size);
                }

                void fixate_delete (const void *ptr)
                {
                        // deletes.
                }

                void* tracked_new (size_t size)
                {
                        void *ptr = nullptr;

                        if (enable_tracking) {
                                ptr = malloc(size);

                                enable_tracking = false;
                                fixate_new(ptr, size);
                                enable_tracking = true;
                        } else {
                                ptr = malloc(size);
                        }

                        if (!ptr)
                                throw std::bad_alloc();
                        return ptr;
                }

                void tracked_delete (void *ptr)
                {
                        if (enable_tracking)
                                fixate_delete(ptr);

                        free(ptr);
                }

                int create_id (const void *ptr)
                {
                    // some hashfunc
                    return 10;
                }
};

inline mem_tracker_t MEMORY_TRACKER {};

void* operator new (size_t size)
{
    return MEMORY_TRACKER.tracked_new(size);
}

void* operator new[] (size_t size)
{
    return MEMORY_TRACKER.tracked_new(size);
}

void operator delete(void* ptr)
{
        MEMORY_TRACKER.tracked_delete(ptr);
}

void operator delete[](void* ptr)
{
        MEMORY_TRACKER.tracked_delete(ptr);
}

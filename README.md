## Project overview:

Many database management systems (DBMSs) are designed to handle databases that are too large to fit entirely in main memory. The buffer manager is the component of a DBMS responsible for transferring pages between disk and memory as needed. The buffer manager also decides which pages should be replaced when the buffer pool is full. Because the buffer manager handles these details, higher-level DBMS code can be written without worrying about whether pages are in memory or not.

In this project, I extended SQLite buffer manager with LRU (least recently used) algorithm.

## Main files:

page_cache_lru.cpp, page_cache_lru.hpp, page_cache.cpp, and page_cache.hpp

## Main coding language:

C++ 17

## Attributions:

I (Zelong Jiang) wrote page_cache_lru.cpp.

My instructor (Kevin Gaffney) wrote page_cache_lru.hpp, page_cache.cpp, and page_cache.hpp

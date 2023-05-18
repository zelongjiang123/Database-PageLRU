Project overview:
Many database management systems (DBMSs) are designed to handle databases that are too large to fit entirely in main memory. The buffer manager is the component of a DBMS responsible for transferring pages between disk and memory as needed. The buffer manager also decides which pages should be replaced when the buffer pool is full. Because the buffer manager handles these details, higher-level DBMS code can be written without worrying about whether pages are in memory or not.

In this project, you will be customizing the buffer manager of SQLite. SQLite is the most widely deployed DBMS in existence. It is found on nearly every smartphone, computer, internet browser, and automobile. There are likely over 1 trillion SQLite databases in active use. Specifically, you will extend SQLite with two custom page replacement algorithms.

SQLite's buffer manager is known as the pager. An important component of the pager is the page cache. Whenever the pager needs a page, it first checks the page cache to see if the page is already in memory. If the page is already in memory, the page cache provides a pointer to the page. If the page is not in memory, the page cache allocates space for a new page by either allocating memory or replacing an existing page.

SQLite allows you to design your own custom page cache by implementing a small set of C functions. SQLite's page cache API has a few differences from a typical buffer manager. First, the page cache is allowed to grow and shrink as needed. At any time, SQLite may set the maximum number of pages in the page cache. While this constraint is more of a guideline than a rule, the page cache should try to honor it. Second, the page cache does not maintain a pin count for pages. When a page is unpinned, it is safe to discard, regardless of the number of prior requests. Third, SQLite may change the page ID associated with a page. If a different page with the new page ID is already in the cache, it is discarded. Finally, SQLite may instruct the page cache to discard all pages in a certain range of page IDs.

Main files:
page_cache_lru.cpp, page_cache_lru.hpp, page_cache.cpp, and page_cache.hpp

Main coding language: C++ 17

Attributions:

I (Zelong Jiang) wrote page_cache_lru.cpp.

My instructor (Kevin Gaffney) wrote page_cache_lru.hpp, page_cache.cpp, and page_cache.hpp

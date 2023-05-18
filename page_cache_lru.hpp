#ifndef CS564_PROJECT_PAGE_CACHE_LRU_HPP
#define CS564_PROJECT_PAGE_CACHE_LRU_HPP

#include "page_cache.hpp"
#include <list>
#include <unordered_map>
using namespace std;


class LRUReplacementPageCache : public PageCache {
public:
  LRUReplacementPageCache(int pageSize, int extraSize);

  ~LRUReplacementPageCache();

  void setMaxNumPages(int maxNumPages) override;

  [[nodiscard]] int getNumPages() const override;

  Page *fetchPage(unsigned int pageId, bool allocate) override;

  void unpinPage(Page *page, bool discard) override;

  void changePageId(Page *page, unsigned int newPageId) override;

  void discardPages(unsigned int pageIdLimit) override;

private:
  // TODO: Declare class members as needed.
  
    struct LRUReplacementPage : public Page {
        LRUReplacementPage(int pageSize, int extraSize, unsigned pageId,
            bool pinned);

        unsigned pageId;
        bool pinned;
    };
      list<unsigned int> page_unpinned_queue; // store all unpined pages

  // store references of key in cache
	unordered_map<unsigned int, list<unsigned int>::iterator> map_unpinned; // store the unpined pages and their position in the list
	unordered_map<unsigned int, LRUReplacementPage*> page_map;  // store every pages with their pageids
};

#endif // CS564_PROJECT_PAGE_CACHE_LRU_HPP

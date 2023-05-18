#include "page_cache_lru.hpp"
#include "utilities/exception.hpp"
#include <algorithm>
#include <iostream>


LRUReplacementPageCache::LRUReplacementPageCache(int pageSize, int extraSize)
	: PageCache(pageSize, extraSize) {
	// TODO: Implement.

	//throw NotImplementedException(
		//"LRUReplacementPageCache::LRUReplacementPageCache");
}

LRUReplacementPageCache::LRUReplacementPage::LRUReplacementPage(
	int argPageSize, int argExtraSize, unsigned argPageId, bool argPinned)
	: Page(argPageSize, argExtraSize), pageId(argPageId), pinned(argPinned) {}

LRUReplacementPageCache::~LRUReplacementPageCache() {
	for (auto it = page_map.begin(); it != page_map.end(); ++it)
		delete it->second;
}

void LRUReplacementPageCache::setMaxNumPages(int maxNumPages) {
	// TODO: Implement.
	maxNumPages_ = maxNumPages;
	//cout << "maxpages " << maxNumPages<<endl;
	// Discard unpinned pages until the number of pages in the cache is less than
	// or equal to `maxNumPages_` or only pinned pages remain.
	for (auto it = page_unpinned_queue.begin(); it != page_unpinned_queue.end();) {
		if (getNumPages() <= maxNumPages_) {
			break;
		}
		unsigned pageid = (*it);
		// remove page from both maps
		map_unpinned.erase(pageid);
		auto page = page_map[pageid];
		delete page; // delete actual page
		page_map.erase(pageid);
		// remove page from the queue
		it = page_unpinned_queue.erase(it);
	}
	//throw NotImplementedException("LRUReplacementPageCache::setMaxNumPages");
}

int LRUReplacementPageCache::getNumPages() const {
	// TODO: Implement.
	//cout << (int)page_map.size() << endl;
	return (int)page_map.size();
	//throw NotImplementedException("LRUReplacementPageCache::getNumPages");
}

Page* LRUReplacementPageCache::fetchPage(unsigned pageId, bool allocate) {
	// TODO: Implement.
	//cout << "fetch " << pageId << endl;
	Page* result = nullptr;
	numFetches_++;
	if (page_map.find(pageId) == page_map.end()) { // cannot find the page

		if (allocate)
		{
			//cout << "page num:"<< getNumPages()<< " max page:"<< maxNumPages_ << endl;
			if (getNumPages() >= maxNumPages_) {// cache is full
				//cout << "cache is full" << endl;
				if (page_unpinned_queue.size() == 0) // no unpined pages
					return nullptr;
				// delete least recently used unpined page
				unsigned last = page_unpinned_queue.back();
				//cout << "evict "<<last<<endl;
				
				// Pops the last unpined page
				page_unpinned_queue.pop_back();

				// Erase the last unpined page from both maps
				map_unpinned.erase(last);
				result = page_map[last];
				page_map.erase(last);

				//place the new page
				//page_map.emplace(pageId, new LRUReplacementPage(pageSize_, extraSize_, pageId, true));
				auto* current_page = (LRUReplacementPage*)result;
				current_page->pageId = pageId;
				page_map.emplace(pageId, current_page);
				//return result;
			}
			else
			{
				auto result = new LRUReplacementPage(pageSize_, extraSize_, pageId, true);
				page_map.emplace(pageId, result);
				return result;
			}
		}
	}
	// present in cache (cache hit)
	else
	{
		//cout << "hit" << endl;
		numHits_++;
		result = page_map[pageId]; //get the page in the page map
		
		// if page is unpinned, we have to delete the page from the unpinned queue
		auto it = std::find(page_unpinned_queue.begin(), page_unpinned_queue.end(), pageId);
		if (it != page_unpinned_queue.end())
		{
			it = page_unpinned_queue.erase(it);
			map_unpinned.erase(pageId);
		}
		page_map[pageId]->pinned = true;  // mark page to pinned
	}
	return result;
	//throw NotImplementedException("LRUReplacementPageCache::fetchPage");
}

void LRUReplacementPageCache::unpinPage(Page* page, bool discard) {
	// TODO: Implement.
	
	auto* current_page = (LRUReplacementPage*)page;
	
	//cout << "unpin "<< current_page->pageId << " " << discard << endl;

	// If discard is true or the number of pages in the cache is greater than the
  // maximum, discard the page. Otherwise, unpin the page.
	if (discard || getNumPages() > maxNumPages_) {
		//cout << "delete " << current_page->pageId << endl;
		page_map.erase(current_page->pageId);
		delete current_page;
	}
	else {
		//if (!current_page->pinned)
			//return;
		current_page->pinned = false;
		//add this page to the front of the queue and to the map for unpined page
		page_unpinned_queue.push_front(current_page->pageId);
		map_unpinned[current_page->pageId] = page_unpinned_queue.begin();
	}

	//throw NotImplementedException("LRUReplacementPageCache::unpinPage");
}

void LRUReplacementPageCache::changePageId(Page* page, unsigned newPageId) {
	// TODO: Implement.
	//cout << "change page id" << endl;
	auto* current_page = (LRUReplacementPage*)page;
	unsigned oldPageId = current_page->pageId;
	if (!current_page->pinned)
	{
		// page is unpinned, so we have to change the pageid in the queue and the map_unpined
		auto it = std::find(page_unpinned_queue.begin(), page_unpinned_queue.end(), oldPageId);
		if (it != page_unpinned_queue.end())
		{
			it = page_unpinned_queue.erase(it);
			page_unpinned_queue.insert(it, newPageId);
			map_unpinned.erase(oldPageId);
		}
	}
	// change the pageid in page_map
	page_map.erase(oldPageId);
	page_map.emplace(newPageId, current_page);
	//throw NotImplementedException("LRUReplacementPageCache::changePageId");
}

void LRUReplacementPageCache::discardPages(unsigned pageIdLimit) {
	// TODO: Implement.
	//cout << "now discarding pages that are bigger than or equal to "<<pageIdLimit << endl;
	// erase element in page_map
	auto it = page_map.begin();
	while(it != page_map.end())
	{
		if (it->first >= pageIdLimit)
		{
			//cout << "discard " << it->second->pageId<< endl;
			delete it->second;  // delete the actual page
			it = page_map.erase(it);
		}
		else
			++it;
	}

	// erase element in map_unpinned
    auto it2 = map_unpinned.begin();
	while (it2 != map_unpinned.end())
	{
		if (it2->first >= pageIdLimit)
		{
			it2 = map_unpinned.erase(it2);
		}
		else
			++it2;
	}

	// erase element in page_unpinned_queue
	auto it3 = page_unpinned_queue.begin();
	while (it3 != page_unpinned_queue.end())
	{
		if ((*it3) >= pageIdLimit)
		{
			it3 = page_unpinned_queue.erase(it3);
		}
		else
			++it3;
	}
	//throw NotImplementedException("LRUReplacementPageCache::discardPages");
}

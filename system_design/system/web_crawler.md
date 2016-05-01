http://flexaired.blogspot.com/2011/09/design-web-crawler.html

Web crawler is a program that traverses the hyperlinks structure of web pages. We crawler visits each web page by following the hyperlinks on previous page it visits. while it is crawling web pages to extract hyperlinks, a web crawler also save each page it has visited.
Crawler basic algorithm

  1. Remove a URL from the unvisited URL list
  2. Determine the IP Address of its host name
  3. Download the corresponding document
  4. Extract any links contained in it.
  5. If the URL is new, add it to the list of unvisited URLs
  6. Process the downloaded document
  7. Back to step 1

Single Crawler

Multithreaded Crawler

Parallel Crawler

Search Engine : architecture

Search Engine : major components

   * Crawlers Collects documents by recursively fetching links from a set of starting pages.Each crawler has different policies The pages indexed by various search engines are different
   * The Indexer Processes pages, decide which of them to index, build various data structures representing the pages (inverted index,web graph, etc), different representation among search engines. Might also build additional structure ( LSI )
   * The Query Processor Processes user queries and returns matching answers in an order determined by a ranking algorithm

Issues on crawler

   * General architecture
   * What pages should the crawler download ?
   * How should the crawler refresh pages ?
   * How should the load on the visited web sites be minimized ?
   * How should the crawling process be parallelized ?

Distributed crawler
The crawler system should consist of a number of crawler entities, which run on distributed sites and interact in peer-to-peer fashion. Each crawler entity has to have the knowledge to its URL subset, as well as mapping from URL subset to network address of corresponding peer crawler entity.
Whenever the crawler entity encounters a URL from a different URL subset, it should forward to the appropriate peer crawler entity based on URL subset to crawler entity lookup.
Each crawler entity should maintain its own database, which only stores the URL’s from the URL subset assigned to the particular entity. The databases are disjoint and can be combined offline when the crawling task is complete.

Crawler Entity Architecture

Each crawler entity consists of a number of crawler threads, a URL handling thread, a URL packet dispatcher thread and URL packet receiver thread. The URL set assigned to each crawler entity will be further divided into subsets for each crawler thread.

Each crawler thread should have its own pending URL list, a DNS cache. Each thread picks up an element from URL pending list, check to see if the IP corresponding to URL is available in the DNS cache, else does a DNS lookup, generates a HTTP fetch requests, gets the page and finally extracts more URL’s from the fetched page and puts them in the job pending queue of the URL handling thread.
URL handling thread will have a job queue. It will get a URL from the job queue, checks to see if the URL belongs to the URL set corresponding to the crawler entity, if it belongs to another entity it will put the URL on the dispatcher queue and get a new URL from the its job queue. If the URL belongs to its set, it firsts checks the URL-seen cache, if the test fails it queries the SQL database to check if the URL has been seen, and puts the URL in the URL-seen cache. It then puts the URL into URL pending list of one of the crawler threads. URLs are assigned to a crawler thread based on domain names. Each domain name will only be serviced by one thread; hence only one connection will be maintained with any given server. This will make sure that the crawler doesn’t overload a slow server.
URL dispatcher thread will communicate the URL’s corresponding crawler entity.
A URL receiver thread collects the URL’s received from other crawler entities i.e. communicated via dispatcher threads of those crawler entities and puts them on the job queue of the URL handling thread.

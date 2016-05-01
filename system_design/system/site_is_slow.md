# Site is slow

Draw the following system: A website with multiple webservers connected to a single relational database on a third server. Like this:
+------------+  +------------+| web server |  | web server |+------------+  +------------+ +----------+ | database | +----------+
The boss comes to you one day and says "The site is slow." Your job is to investigate what is going wrong. This is very relevant to what SDEs at this company frequently do. It is also very open-ended and there are many acceptable responses.
In general, you are looking for the candidate to attack the problem, compartmentalize the subsystems. This also allows you to see how much depth they have in a variety of problem domains, from networking to databases to distributed systems.
Good answers:

   * Connect to the site with a web browser and verify that it actually is slow.
   * Connect to each web server individually to determine if it is a problem with only one web server.
   * Look at the logs. People who expect logs are people who are used to writing maintainable software.
   * Run a test suite to see where there's trouble. Similar to looking for logs, this suggests that they have good practices for writing robust, maintainable software.
   * Look at web server logs to validate that each web server is receiving an appropriate share of the traffic. If not, something could be messed up with the upstream router/load-balancer.
   * Look at both the web server and database server at a system level. Are the machines constrained on disk I/O, memory, processor? Make sure you get specifics from the candidate as to which tools they would use.
   * See if the network pipe between the database server and the web server is flooded, and needed to be expanded.
   * Is there some sort of database connection pooling going on? Is it sufficient?
   * Take a look at the SQL statements to verify that they weren't doing anything that would thrash the server. If so, tune the statements.
   * Look into the DB schema, to see if there were proper indices.
   * Look at the application that accesses the database: is it locking up a whole table for each insert? Is it hitting the database only when it needs to, or is it hitting the database for every page load?
   * Verify that it's not some gnarly HTML that is causing a long render time.

Terrible answers (these are real):

   * We should just buy faster hardware. ("what if the hardware is already top of the line?") then it's fast enough.
   * I wouldn't trust any business owner to tell me that the system wasn't performing well enough.
   * Ask the system administrator to figure it out.

Once you are satisfied with their analysis, here are follow-up questions:
It turns out that the site got Dugg (old school: slashdotted). The traffic is immense and the bottleneck is the database (the web servers are largely idle waiting for the database to finish returning records.) What can be done in software to make the site more speedy?
If the databse is the scarce resource, you are looking for the candidate to propose some sort of caching scheme. In addition to seeing what the candidate proposes to cache (raw database records, "objects", web pages, etc), there are lots of drill-down questions regarding how the cache will work. For example: where will the data be cached? Will it be a write-back or write-through cache? The design of a cache depends to a large extend on the characteristics of the data: an ideal cache for read-only data will look quite different from one for read-write data, and likewise for relatively static data vs. real-time or constantly updated data. Once the candidate has fleshed out a cache design, vary the characteristics of the data and see how he/she modifies the cache design to fit.
This site sells products (what an idea!) and the database contains product records. Your sales department has landed a contract which will add 5 million products to the database. Without adding hardware, what needs to change in the design?
Retrieving individual product information should not be any different if there are 10 products or 10 million. However, the "search" feature of the website will be affected. See how the candidate makes that conclusion and what ideas they have for, possibly, separating the implementation of searching for records from retrieving them (like what we do.) Another idea is to change the web interface to require certain constraints to searches (such as by price) to avoid a full table scan. Another idea is to generate database views based on product type (shoes, books, etc) again with the express purpose of limiting full table scans of the enormous product table.

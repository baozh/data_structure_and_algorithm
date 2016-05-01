# URL Shortening Service

   * Check with interviewee whether they are familiar with URL shortening services such as http://tinyurl.com or http://bit.ly - if not, give example scenario
      * A full list of URL shortening services is available at http://searchengineland.com/analysis-which-url-shortening-service-should-you-use-17204

## Requirement Gathering
There are 3 major question topics I look for in the requirement gathering phase, and a few minor ones. The three major ones are:

  1. Volume (I'd really like them to ask for both the number of new URL mappings and the number of redirect/lookup requests)
  2. Data Retention / Expiration
  3. User Authentication

Minor ones are:

  1. What sort of data do we want to collect out of this system?
  2. What are the latency requirements for requests?
  3. What globalization / localization / internationalization should be done for this system?
  4. API vs. UI

I don't expect candidates to get all of these - if I score the major ones as 2 points each, and the minor ones as 1 point each (for a total of 10 possible points), I'll generally accept somebody who gets 5/10 or better (that usually works out to 2 of the major 3, and 1 of the minor ones). I'll allow them to come back and ask these questions during the design phase and give credit for asking them at that point, but only if I haven't done any prompting. For example, if I ask the question: "how do you determine the scalability model for your design", they get no credit for then asking about volume (though if they still don't, it's pretty much an automatic "not inclined", even if they otherwise scored high enough).

## Design
Ask for high level architecture / system components. I want to hear:

   * web server with 2 primary uses:
      * a section for entering in new URL mappings
      * a redirect/lookup service for incoming short URLs
   * some sort of data storage

Anything less or further needs some explanation (i.e. I'd want to know why somebody would use a middle-tier for this simple of a system)

## Questions
Q: How do you design the data storage for this scale of data and requests?

A: I want to see something along the lines of the following design:

N backend partitions, each with M replicas. Scale N to account for data storage and write volume, scale M to account for number of read requests. All writes should be sent to all M replicas of a given partition, while reads can go to a single replica. Use some function of the short URL to determine which partition to store the mapping in / retrieve the data from. Recognition that the storage on each machine should be something like a hash or b-tree is good - we don't need a relational database here. An answer which uses a BDB file for the storage is a great answer, but I'll accept MySQL, Postgres, etc. If candidate proposes a heavy duty relational database (Oracle, SQL Server, etc.), I'll push them for a low cost solution, but the tendency to go there is already a yellow flag. If the candidate proposes a "NoSQL" solution such as Cassandra, Voldemort, or Riak, I'll give bonus points, but still push them for how to solve the problem from scratch (I want to see that they know how to figure out an algorithm for this, not just that they know of a solution somebody else has designed).

If appropriate given the above solution, I will ask:

Q: Does the short URL need to be a function of the long URL?

A: no - anything but no is a red flag here. Hopefully the candidate will explain why they say no, if not, follow up with a question of why not.

Q: How would you implement the expiration policy (remind them of the policy again, making sure the "minimum" or "at least" portion is stated clearly, but without being obnoxious about it, as it is a hint)?

A: The standard answer is to update a last access timestamp on the record on each lookup request. I don't think anybody has ever realized up front the sort of write rates this would entail. This answer is particularly bad if they used a relational database, due to table locking, but is bad for any solution, given that writes must go to all replicas, and this would absolutely destroy the point of having replicas to distribute the request load among (though there would still be a point to having a replica for redundancy, etc.).
The answer I hope to hear is:

	if (now > record.last_access_timestamp + X_time){  
		record.last_access_timestamp = now
	}
	
coupled with a batch process which iterates through the records performing an equivalent of:

	expire_timestamp = now - expiration_policy_time - X_time;
	foreach record {  
		if (expire_timetamp > record.last_access_timestamp)  {    
			delete record  
		}
	}
	
I would be delighted if the candidate were to point out how X_time can then be tuned to balance the write rate and the data storage volume (higher write rate goes with lower data storage volume, lower write rate means you have to persist the data for longer, requiring higher data storage volume), but I don't think I've ever heard anybody state this yet.

Q: What happens when you bring a crashed / out of service replica back online?
A: At a minimum, the candidate at least recognizes that it may have missed a bunch of transactions, and shouldn't be put in rotation until up to sync. The naive answer here is that it should get a full copy of the data from one of the other replicas in its partition, but that has its own issues (by the time you've got all that data you're out of sync with what's happened since then).

The answer I hope to hear is:

Each replica should be keeping a record of monotonically increasing transaction ids for each write/delete to enable replay capabilities. When a replica comes back up, it should immediately begin queuing incoming transactions (but not necessarily applying them), then look at its most recent (highest) applied transaction id, and incrementally request chunks of transactions from other replicas in its partition greater than this transaction id and less than the first received queued transaction, and apply them, until no transactions remain in that range. The replica should then begin applying transactions from its queue, and once caught up to within X time behind real time (where X is defined as an availability SLA), should put itself back into the read rotation.

Bonus points if they clean up after themselves with a process which periodically scans for maximum applied transaction id across all M replicas (whether currently up or not, if not all are up, then no clean up possible), and delete transaction records less than the maximum found. If a replica is down for too long, it should be removed from consideration as a replica for the partition, so that clean up can proceed, and later added in as a new replica host for the partition.

New replica hosts for the partition should be introduced by taking another replica out of service (both reads and writes), allow any queued transactions to apply, and then fully copy its data (including transaction records) to the new replica host. Once the copy has been completed, both replicas should be reintroduced in the manner described above for out of service replicas.

I judge candidates based on their ability to come up with reasonable designs (not necessarily exact replicas of my desired solution), and on how many of the questions they get through. Most candidates do not make it past the expiration policy question before phone screen / interview time runs out. The few that have made it to the "out of service replica" problem have not produced answers better than the minimum problem recognition and naive solution.

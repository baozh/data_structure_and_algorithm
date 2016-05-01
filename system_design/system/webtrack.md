
Question: We need to track user activity on the site (page views, searches, invitations). For example when tracking a search we would probably want to know which user did the search, the time the search occurred, what their query was, what the results were shown, various ranking information, etc. There are various consumers of this data--online services that might do relevance processing, security processing (to prevent abuse or spam) and also offline consumers like the data warehouse or hadoop that only consume data once an hour (say).These consumers should be able to subscribe to data feeds by the type of event (say just searches, or just page views).
Design a system to do manage and serve these feeds. Some constraints: should be able to process 10 TB/day of data, should be easy to manage, and reasonably cheap. Ask any questions you like.
There are no constraints around fitting in with any other infrastructure you need to worry about.
The goal of this question is to let the candidate walk through the complete design process. It is suitable for reasonably senior candidates who would have experience designing from-scratch systems. As much as possible they should drive the questions rather than you providing them with data up front.
I break this down into three parts (1) single node, (2) distributed, (3) performance analysis. An ideal candidate will not need all these prompts, answering most of the subquestions on their own, but i find some prompting is usually needed to get depth.

## Single Node Questions

   * How are events represented? JSON? Some custom serialization? This is a practical but basic question they should be able to answer quickly.
   * What are the APIs we are providing? Generally people say something like publish(messages) and fetch(topic, timestamp) which is reasonable. Often people will have the servers push the data out to consumers which is also fine.
   * How do the clients communicate with the server--HTTP? Some custom TCP protocol? UDP? What are the advantages and disadvantages of each of these?
   * What is the data structure in which data is stored. There isn't really a right answer here, but "oracle" is a wrong answer. It should ideally be something that uses local storage and is more efficient than a general purpose database.
   * What is the data structure used to store the data? They should ideally come up with something that doesn't do too much random access and doesn't need to scan through too much data to answer a fetch request.
   * How/when does data get deleted? There are two approaches, clean up after some time or clean up after all consumers consume. Either is fine.

## Distributed Questions

   * How is data partitioned over machines. Any approach is fine here as long as it doesn't try to store 100% of a topic on one machine.
   * At some point they should start to think about the semantics they are providing. Any reasonable semantics are fine, since the requirements here are pretty lax.
   * How do producers balance load? Should be easy since they can probably just balance randomly.
   * How do consumers balance load? This is harder. There are many good ways to do this.
   * How do producers fail over? How do consumers fail over?
   * What would be approaches to replicating the data to tolerate server failure? This will just be high-level since it is fairly detailed.

## Performance Questions

   * There is only a single question for this part: Assuming 10TB/day how many servers do we need to buy?
   * They should ask something about peak load. Let them assume peak is 2x off-peak.
   * A correct answer here should outline which resources there are to consider (cpu, network io, IO bandwidth, IO Ops, memory, disk space, etc). They should have reasonable estimates for the capabilities of modern computers, and be able to estimate the performance of their system with respect to each of them. For most systems disk related things will be the bottleneck and they should be able to reason their way to some MB/sec performance and drive an estimate off of that.

Evaluation
This is frankly a hard question to evaluate.
A poor candidate will be lost with an unstructured question like this.
A good candidate should be able to

  1. Quickly make some reasonable assumptions and choices about how to proceed and have a good knowledge of the important choices:
    1. networking: udp vs tcp vs http vs whatever else
    2. storage
    3. message serialization
    4. push vs pull
  2. Come up with a reasonable persistent data structure that supports efficient appends
  3. Come up with something feasible around distribution
  4. Be able to reason through performance back-of-the-envelope estimates though perhaps needing some prompting to come up with the main criteria and bottlenecks. May require help getting reasonable estimates of performance of off-the-shelf parts.

A great candidate will

  1. Have reference to other data systems to compare to
  2. Will be able to characterize what the tradeoff is for each decision rather than just their choice
  3. Will come up with a crisp and efficient persistent data structure
  4. Will have a ready grasp of the trade-offs involved in distribution, for example:
    1. Think without prompting about sensible guarantees/semantics for producers and consumers
    2. Understand the difficulties of guaranteeing exact message consumption as a variant of classical distributed commit or consensus problems
  5. Easily understand the performance implications of their design along all the major resources (IOPS, I/O throughput, memory, cpu, network throughput, disk space, etc.). Be able to make reasonable assumptions about the available performance of commodity hardware along these dimensions. Comfortable quickly estimating through to a number of machines from this model.



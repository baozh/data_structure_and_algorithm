Hadoop Map/Reduce Implementation

http://horicky.blogspot.com/2010/08/designing-algorithmis-for-map-reduce.html



In my previous post, I talk about the methodology of transforming a sequential algorithm into parallel. After that, we can implement the parallel algorithm, one of the popular framework we can use is the Apache Opensource Hadoop Map/Reduce framework.

Functional Programming

Multithreading is one of the popular way of doing parallel programming, but major complexity of multi-thread programming is to co-ordinate the access of each thread to the shared data. We need things like semaphores, locks, and also use them with great care, otherwise dead locks will result.

If we can eliminate the shared state completely, then the complexity of co-ordination will disappear. This is the fundamental concept of functional programming. Data is explicitly passed between functions as parameters or return values which can only be changed by the active function at that moment. Imagine functions are connected to each other via a directed acyclic graph. Since there is no hidden dependency (via shared state), functions in the DAG can run anywhere in parallel as long as one is not an ancestor of the other. In other words, analyze the parallelism is much easier when there is no hidden dependency from shared state.

Map/Reduce functions

Map/reduce is a special form of such a DAG which is applicable in a wide range of use cases. It is organized as a “map” function which transform a piece of data into some number of key/value pairs. Each of these elements will then be sorted by their key and reach to the same node, where a “reduce” function is use to merge the values (of the same key) into a single result.

map(input_record) {…emit(k1, v1)…emit(k2, v2)…}reduce (key, values) {aggregate = initialize()while (values.has_next) {    aggregate = merge(values.next)}collect(key, aggregate)}
The Map/Reduce DAG is organized in this way.



A parallel algorithm is usually structure as multiple rounds of Map/Reduce




Distributed File Systems

The distributed file system is designed to handle large files (multi-GB) with sequential read/write operation. Each file is broken into chunks, and stored across multiple data nodes as local OS files.



There is a master “NameNode” to keep track of overall file directory structure and the placement of chunks. This NameNode is the central control point and may re-distributed replicas as needed.

To read a file, the client API will calculate the chunk index based on the offset of the file pointer and make a request to the NameNode. The NameNode will reply which DataNodes has a copy of that chunk. From this points, the client contacts the DataNode directly without going through the NameNode.

To write a file, client API will first contact the NameNode who will designate one of the replica as the primary (by granting it a lease). The response of the NameNode contains who is the primary and who are the secondary replicas. Then the client push its changes to all DataNodes in any order, but this change is stored in a buffer of each DataNode. After changes are buffered at all DataNodes, the client send a “commit” request to the primary, which determines an order to update and then push this order to all other secondaries. After all secondaries complete the commit, the primary will response to the client about the success.

All changes of chunk distribution and metadata changes will be written to an operation log file at the NameNode. This log file maintain an order list of operation which is important for the NameNode to recover its view after a crash. The NameNode also maintain its persistent state by regularly check-pointing to a file.

In case of the NameNode crash, all lease granting operation will fail and so any write operation is effectively fail also. Read operation should continuously to work as long as the clinet program has a handle to the DataNode. To recover from NameNode crash, a new NameNode can take over after restoring the state from the last checkpoint file and replay the operation log.

When a DataNode crashes, it will be detected by the NameNode after missing its hearbeat for a while. The NameNode removes the crashed DataNode from the cluster and spread its chunks to other surviving DataNodes. This way, the replication factor of each chunk will be maintained across the cluster.

Later when the DataNode recover and rejoin the cluster, it reports all its chunks to the NameNode at boot time. Each chunk has a version number which will advanced at each update. Therefore, the NameNode can easily figure out if any of the chunks of a DataNode becomes stale. Those stale chunks will be garbage collected at a later time.


Job Execution

Hadoop MapRed is based on a “pull” model where multiple “TaskTrackers” poll the “JobTracker” for tasks (either map task or reduce task).

The job execution starts when the client program uploading three files: “job.xml” (the job config including map, combine, reduce function and input/output data path, etc.), “job.split” (specifies how many splits and range based on dividing files into ~16 – 64 MB size), “job.jar” (the actual Mapper and Reducer implementation classes) to the HDFS location (specified by the “mapred.system.dir” property in the “hadoop-default.conf” file). Then the client program notifies the JobTracker about the Job submission. The JobTracker returns a Job id to the client program and starts allocating map tasks to the idle TaskTrackers when they poll for tasks.




Each TaskTracker has a defined number of "task slots" based on the capacity of the machine. There are heartbeat protocol allows the JobTracker to know how many free slots from each TaskTracker. The JobTracker will determine appropriate jobs for the TaskTrackers based on how busy thay are, their network proximity to the data sources (preferring same node, then same rack, then same network switch). The assigned TaskTrackers will fork a MapTask (separate JVM process) to execute the map phase processing. The MapTask extracts the input data from the splits by using the “RecordReader” and “InputFormat” and it invokes the user provided “map” function which emits a number of key/value pair in the memory buffer.



When the buffer is full, the output collector will spill the memory buffer into disk. For optimizing the network bandwidth, an optional “combine” function can be invoked to partially reduce values of each key. Afterwards, the “partition” function is invoked on each key to calculate its reducer node index. The memory buffer is eventually flushed into 2 files, the first index file contains an offset pointer of each partition. The second data file contains all records sorted by partition and then by key.

When the map task has finished executing all input records, it start the commit process, it first flush the in-memory buffer (even it is not full) to the index + data file pair. Then a merge sort for all index + data file pairs will be performed to create a single index + data file pair.

The index + data file pair will then be splitted into are R local directories, one for each partition. After all the MapTask completes (all splits are done), the TaskTracker will notify the JobTracker which keeps track of the overall progress of job. JobTracker also provide a web interface for viewing the job status.

When the JobTracker notices that some map tasks are completed, it will start allocating reduce tasks to subsequent polling TaskTrackers (there are R TaskTrackers will be allocated for reduce task). These allocated TaskTrackers remotely download the region files (according to the assigned reducer index) from the completed map phase nodes and concatenate (merge sort) them into a single file. Whenever more map tasks are completed afterwards, JobTracker will notify these allocated TaskTrackers to download more region files (merge with previous file). In this manner, downloading region files are interleaved with the map task progress. The reduce phase is not started at this moment yet.

Eventually all the map tasks are completed. The JobTracker then notifies all the allocated TaskTrackers to proceed to the reduce phase.  Each allocated TaskTracker will fork a ReduceTask (separate JVM) to read the downloaded file (which is already sorted by key) and invoke the “reduce” function, which collects the key/aggregatedValue into the final output file (one per reducer node). Note that each reduce task (and map task as well) is single-threaded. And this thread will invoke the reduce(key, values) function in assending (or descending) order of the keys assigned to this reduce task. This provides an interesting property that all
entries written by the reduce() function is sorted in increasing order. The output of each reducer is written to a temp output file in HDFS. When the reducer finishes processing all keys, the temp output file will be renamed atomically to its final output filename.

The Map/Reduce framework is resilient to crashes of any components.  TaskTracker nodes periodically report their status to the JobTracker which keeps track of the overall job progress. If the JobTracker hasn’t heard from any TaskTracker nodes for a long time, it assumes the TaskTracker node has been crashed and will reassign its tasks appropriately to other TaskTracker nodes. Since the map phase result is stored in the local disk, which will not be available when the TaskTracker node crashes. In case a map-phase TaskTracker node crashes, the crashed MapTasks (regardless of whether it is complete or not) will be reassigned to a different TaskTracker node, which will rerun all the assigned splits. However, the reduce phase result is stored in HDFS, which is available even the TaskTracker node crashes. Therefore, in case a reduce-phase TaskTracker node crashes, only the incomplete ReduceTasks need to be reassigned to a different TaskTracker node, where the incompleted reduce tasks will be re-run.

The job submission process is asynchronous. Client program can poll for the job status at any time by supplying the job id.

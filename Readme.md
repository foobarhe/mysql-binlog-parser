mysql binlog (row-based) parser

based on official api: mysql-replication-listener
// https://code.launchpad.net/~mysql/mysql-replication-listener/trunk

The API will make it is easy to do Mysql CDC (change data capture). 

The solution can be used for: 

1. replicate mysql data to other places (search engine index, hbase, redis, hdfs) ...
replacing the dual write solution which suffer lots of issues. 
using the following step to do the replications on the fly (with old db serving writes). 
a. mysqldump the old db and record the binlogfile + position;
b. using script to import the dump to new storage;
c. run the sync from the recorded position to replay the new changes to the new storage.  

2. used for shard the existing mysql table (e.g. split the existing 1 table into 2 new tables, and drop the old one)
a. mysqldump the old table and record the binlogfile + position;
b. process the dump, run hash to split and save data into 2 new tables;
c. run the sync from the recorded position and run hash to upate the new changes into 2 new tables.  
d. change traffic to the 2 new tables, but turn off the writes. 
e. when there is no new traffic on old table, turn on writes on the 2 new tables. 
The step d and e can be done in very short time, during this time the client is write-denied which is acceptable. 
 
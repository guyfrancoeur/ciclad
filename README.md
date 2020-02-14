# ciclad C++

## Based on Galicia-P by Petko Valtchev, PhD

# CICLAD: A Streaming Super Fast and Memory Lite Closed Itemset Miner

Today increasing amount of data demands appropriate algorithm to discover the unknown. Mining of frequent itemsets over a data stream still witholds some challenges. As a remedy, frequent closed itemsets (FCIs) were studied, yet they rise their own issues, especially with sparse datasets. We introduce `ciclad`, an efficient stream CI miner that leverages in-depth insights into the mathematics of FCI evolution into an intersection-based approach. It combines efficient storage and access techniques into a sliding-window method of a surprising simplicity. Experimental results indicate that ciclad~largely outperforms its main competitors by a large margin (up to 70 times faster) on sparse dataset whereas on dense ones its performance remains comparable.

Leaving data behind because they are not frequent is not reasonable. This may be rare and rare itemset could be useful to take appropriate decision.  Furthermore, some algorithms compute using min_supp to optimize the production of results.  Some may be using approximate computation.  These techniques are producing near acceptable results in some case, but why approximate when you can get the real picture in all cases.  Ciclad is by far the fastest algorithm for years.  Not only fast but also great on RAM usage.  Stay away from RAMovore algorithms.  This makes ciclad a superb solution for embedded devices, FPGA or any device.  

Keywords : frequent pattern mining, closed itemsets, data streams, sliding window, inverted lists

Contributors :
 - Tomas Martin 2015-2020
 - Guy Francoeur 2015-2020
 - Mickael Wajnberg 2018
 - Petko Valtchev :copyright:

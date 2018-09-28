# ciclad
ciclad cpp


Mining of frequent itemsets over a data stream still witholds some challenges, e.g. due to the huge size of the result. As a remedy, frequent closed itemsets (FCIs) were studied, yet they rise their own issues, especially with sparse datasets. We introduce \ciclad, an efficient stream FCI miner that leverages in-depth insights into the mathematics of FCI evolution into an intersection-based approach. It combines efficient storage and access techniques into a sliding-window method of a surprising simplicity. Experimental results indicate that \ciclad~largely outperforms its main competitors by a large margin (up to 70 times faster) on sparse dataset whereas on dense ones its performance remains comparable.

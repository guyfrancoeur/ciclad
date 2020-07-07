# ciclad Project C++

![ciclad v3](https://github.com/guyfrancoeur/ciclad/workflows/ciclad%20v3/badge.svg)

![gnuplot histogram](https://github.com/guyfrancoeur/ciclad/workflows/gnuplot%20histogram/badge.svg)

[![Maintenance](https://img.shields.io/badge/Active%3F-Yes-orange.svg)](https://github.com/guyfrancoeur/ciclad/blob/master/README.md)

# CICLAD: A super fast Streaming, Memory ultra-lite Closed Itemset Miner

Today increasing amount of data demands appropriate algorithm to discover the unknown. Mining of frequent itemsets over a data stream still witholds some challenges. As a remedy, frequent closed itemsets (FCIs) were studied, yet they rise their own issues, especially with sparse datasets. We introduce `ciclad`, an efficient stream CI miner that leverages in-depth insights into the mathematics of FCI evolution into an intersection-based approach. It combines efficient storage and access techniques into a sliding-window method of a surprising simplicity. Experimental results indicate that ciclad~largely outperforms its main competitors by a large margin (up to 70 times faster) on sparse dataset whereas on dense ones its performance remains comparable.

Leaving data behind because they are not frequent is not reasonable. Not frequent may be rare and rare itemsets could be useful to take appropriate decisions.  Furthermore, some algorithms compute using min_supp to optimize the production of results.  Some may be using approximate computation.  These techniques are producing near acceptable results in some case, but why approximate when you can get the real picture in all cases.  Ciclad is by far the fastest algorithm for years.  Not only fast but also great on RAM usage.  Stay away from RAMovore algorithms.  This makes ciclad a superb solution for embedded devices, FPGA or any device.  

Keywords : frequent pattern mining, closed itemsets, data streams, sliding window, inverted lists

#### Authors :
 - Tomas Martin 2015-2020
 - Guy Francoeur 2015-2020
 - Petko Valtchev :copyright:

[![Maintenance](https://img.shields.io/badge/Paper%20Link%20at%20arXiv-CICLAD-orange.svg)](https://arxiv.org/abs/2007.01946)

[![DOI:10.1145/3394486.3403232](https://zenodo.org/badge/DOI/10.1145/3394486.3403232.svg)](https://doi.org/10.1145/3394486.3403232)

NOTA: ciclad_nogen is the folder with the implementation used in the KDD'20 paper.

#### How to make citation :

- BibTeX :
~~~
@misc{martin2020ciclad,
    title={CICLAD: A Fast and Memory-efficient Closed Itemset Miner for Streams},
    author={Tomas Martin and Guy Francoeur and Petko Valtchev},
    year={2020},
    eprint={2007.01946},
    archivePrefix={arXiv},
    primaryClass={cs.DB}
}
~~~

Contributors :
 - Mickael Wajnberg 2018
 - see github contributors

#### More :
 + [experiments in images](./image/README.md)

Accepted paper fot KDD 2020 :
+ [CICLAD: A Fast and Memory-efficient Closed Itemset Miner for Streams](https://www.kdd.org/kdd2020/accepted-papers#:~:text=CICLAD)

Reference :
+ [A framework for incremental generation of closed itemsets](https://www.sciencedirect.com/science/article/pii/S0166218X07003472)
+ [Generating frequent itemsets incrementally: two novel approaches based on Galois lattice theory](https://www.tandfonline.com/doi/abs/10.1080/09528130210164198)

---
ciclad :copyright: 2015-2020

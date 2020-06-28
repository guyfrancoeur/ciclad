#!/bin/bash

# ---- annexe_gctree ----
# Enlever fin de ligne (des fichiers windows)
sed -i 's/\r//g' ./data/annexe_gctree/GC-Tree.txt
sed -i 's/\r//g' ./data/annexe_gctree/ciclad.txt
sed -i 's/\r//g' ./data/annexe_gctree/moment.txt
paste ./data/annexe_gctree/GC-Tree.txt ./data/annexe_gctree/ciclad.txt ./data/annexe_gctree/moment.txt > ./data/annexe_gctree/merge.txt
gnuplot -p -e "filename='./data/annexe_gctree/merge.txt'; fileout='./images/annexe_gctree.png'" annexe_gctree.dem

# ---- ciclad_vs_moment ----
# Enlever fin de ligne (des fichiers windows)
sed -i 's/\r//g' ./data/ciclad_vs_moment/ciclad.txt
sed -i 's/\r//g' ./data/ciclad_vs_moment/moment.txt
paste ./data/ciclad_vs_moment/ciclad.txt ./data/ciclad_vs_moment/moment.txt > ./data/ciclad_vs_moment/merge.txt
gnuplot -p -e "filename='./data/ciclad_vs_moment/merge.txt'; fileoutRUNTIME='./images/ciclad_vs_moment_runtime.png'; fileoutMEMORY='./images/ciclad_vs_moment_memory.png'" ciclad_vs_moment_memory.dem

# ---- moment_supports ----
# Enlever fin de ligne (des fichiers windows)
sed -i 's/\r//g' ./data/moment_supports/ciclad.txt
sed -i 's/\r//g' ./data/moment_supports/moment1.txt
sed -i 's/\r//g' ./data/moment_supports/moment2.txt
sed -i 's/\r//g' ./data/moment_supports/moment3.txt
sed -i 's/\r//g' ./data/moment_supports/moment5.txt
paste ./data/moment_supports/ciclad.txt ./data/moment_supports/moment1.txt ./data/moment_supports/moment2.txt ./data/moment_supports/moment3.txt ./data/moment_supports/moment5.txt > ./data/moment_supports/merge.txt
gnuplot -p -e "filename='./data/moment_supports/merge.txt'; fileoutMEMORY='./images/moment_supports_memory.png'; fileoutRUNTIME='./images/moment_supports_runtime.png'" moment_supports.dem

# ---- java_all_3 ----
# Enlever fin de ligne (des fichiers windows)
sed -i 's/\r//g' ./data/java_all_3/data.txt
gnuplot -p -e "filename='./data/java_all_3/data.txt'; fileout='./images/java_all_3.png'" java_all_3.dem

# ---- results_siam ----
# Enlever fin de ligne (des fichiers windows)
sed -i 's/\r//g' ./data/results_siam/retail/ciclad.txt
sed -i 's/\r//g' ./data/results_siam/retail/moment.txt
sed -i 's/\r//g' ./data/results_siam/retail/newmoment.txt
sed -i 's/\r//g' ./data/results_siam/retail/clostream.txt
sed -i 's/\r//g' ./data/results_siam/retail/cfi-stream.txt

paste ./data/results_siam/retail/ciclad.txt ./data/results_siam/retail/moment.txt ./data/results_siam/retail/newmoment.txt ./data/results_siam/retail/clostream.txt ./data/results_siam/retail/cfi-stream.txt > ./data/results_siam/retail/merge.txt
gnuplot -p -e "filename='./data/results_siam/retail/merge.txt'; fileoutCPU='./images/results_siam_cpu.png'; fileoutRAM='./images/results_siam_ram.png'" results_siam.dem
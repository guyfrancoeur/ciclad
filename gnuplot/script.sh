#!/bin/bash

# Enlever fin de ligne (des fichiers windows)
for elt1 in data/*/*; do
    if [ -d "$elt1" ]; then
        for elt2 in data/*/*/*; do
            sed -i 's/\r//g' "$elt2"
        done
    else
        sed -i 's/\r//g' "$elt1"
    fi
done

# ---- annexe_gctree ----
paste ./data/annexe_gctree/GC-Tree.txt ./data/annexe_gctree/ciclad.txt ./data/annexe_gctree/moment.txt > ./data/annexe_gctree/merge.txt
gnuplot -p -e "filename='./data/annexe_gctree/merge.txt'; fileout='./images/annexe_gctree.png'" annexe_gctree.dem

# ---- ciclad_vs_moment ----
paste ./data/ciclad_vs_moment/ciclad.txt ./data/ciclad_vs_moment/moment.txt > ./data/ciclad_vs_moment/merge.txt
gnuplot -p -e "filename='./data/ciclad_vs_moment/merge.txt'; fileoutRUNTIME='./images/ciclad_vs_moment_runtime.png'; fileoutMEMORY='./images/ciclad_vs_moment_memory.png'" ciclad_vs_moment_memory.dem

# ---- moment_supports ----
paste ./data/moment_supports/ciclad.txt ./data/moment_supports/moment1.txt ./data/moment_supports/moment2.txt ./data/moment_supports/moment3.txt ./data/moment_supports/moment5.txt > ./data/moment_supports/merge.txt
gnuplot -p -e "filename='./data/moment_supports/merge.txt'; fileoutMEMORY='./images/moment_supports_memory.png'; fileoutRUNTIME='./images/moment_supports_runtime.png'" moment_supports.dem

# ---- java_all_3 ----
gnuplot -p -e "filename='./data/java_all_3/data.txt'; fileout='./images/java_all_3.png'" java_all_3.dem

# ---- results_siam ---- Retail / BMSView / ChainStore / Mushroom / Synth / Synth2 / Net-Log ----
paste ./data/results_siam/Retail/ciclad.txt ./data/results_siam/Retail/moment.txt ./data/results_siam/Retail/newmoment.txt ./data/results_siam/Retail/clostream.txt ./data/results_siam/Retail/cfi-stream.txt > ./data/results_siam/Retail/merge.txt
paste ./data/results_siam/BMSView/ciclad.txt ./data/results_siam/BMSView/moment.txt ./data/results_siam/BMSView/newmoment.txt ./data/results_siam/BMSView/clostream.txt ./data/results_siam/BMSView/cfi-stream.txt > ./data/results_siam/BMSView/merge.txt
paste ./data/results_siam/ChainStore/ciclad.txt ./data/results_siam/ChainStore/moment.txt ./data/results_siam/ChainStore/newmoment.txt ./data/results_siam/ChainStore/clostream.txt ./data/results_siam/ChainStore/cfi-stream.txt > ./data/results_siam/ChainStore/merge.txt
paste ./data/results_siam/Mushroom/ciclad.txt ./data/results_siam/Mushroom/moment.txt ./data/results_siam/Mushroom/newmoment.txt ./data/results_siam/Mushroom/clostream.txt ./data/results_siam/Mushroom/cfi-stream.txt > ./data/results_siam/Mushroom/merge.txt
paste ./data/results_siam/Synth/ciclad.txt ./data/results_siam/Synth/moment.txt ./data/results_siam/Synth/newmoment.txt ./data/results_siam/Synth/clostream.txt ./data/results_siam/Synth/cfi-stream.txt > ./data/results_siam/Synth/merge.txt
paste ./data/results_siam/Synth2/ciclad.txt ./data/results_siam/Synth2/moment.txt ./data/results_siam/Synth2/newmoment.txt ./data/results_siam/Synth2/clostream.txt ./data/results_siam/Synth2/cfi-stream.txt > ./data/results_siam/Synth2/merge.txt
paste ./data/results_siam/NetLog/ciclad.txt ./data/results_siam/NetLog/moment.txt ./data/results_siam/NetLog/newmoment.txt ./data/results_siam/NetLog/clostream.txt ./data/results_siam/NetLog/cfi-stream.txt > ./data/results_siam/NetLog/merge.txt

# CPU
gnuplot -p -e "filenameRetail='./data/results_siam/retail/merge.txt'; fileoutRetailCPU='./images/retail_cpu.png';" \
-e "filenameBMSView='./data/results_siam/BMSView/merge.txt'; fileoutBMSViewCPU='./images/BMSView_cpu.png';" \
-e "filenameChainStore='./data/results_siam/ChainStore/merge.txt'; fileoutChainStoreCPU='./images/ChainStore_cpu.png';" \
-e "filenameMushroom='./data/results_siam/Mushroom/merge.txt'; fileoutMushroomCPU='./images/Mushroom_cpu.png';" \
-e "filenameSynth='./data/results_siam/Synth/merge.txt'; fileoutSynthCPU='./images/Synth_cpu.png';" \
-e "filenameSynth2='./data/results_siam/Synth2/merge.txt'; fileoutSynth2CPU='./images/Synth2_cpu.png';" \
-e "filenameNetLog='./data/results_siam/NetLog/merge.txt'; fileoutNetLogCPU='./images/NetLog_cpu.png'" results_siam_cpu.dem

# RAM
gnuplot -p -e "filenameRetail='./data/results_siam/retail/merge.txt'; fileoutRetailRAM='./images/retail_ram.png';" \
-e "filenameBMSView='./data/results_siam/BMSView/merge.txt'; fileoutBMSViewRAM='./images/BMSView_ram.png';" \
-e "filenameChainStore='./data/results_siam/ChainStore/merge.txt'; fileoutChainStoreRAM='./images/ChainStore_ram.png';" \
-e "filenameMushroom='./data/results_siam/Mushroom/merge.txt'; fileoutMushroomRAM='./images/Mushroom_ram.png';" \
-e "filenameSynth='./data/results_siam/Synth/merge.txt'; fileoutSynthRAM='./images/Synth_ram.png';" \
-e "filenameSynth2='./data/results_siam/Synth2/merge.txt'; fileoutSynth2RAM='./images/Synth2_ram.png';" \
-e "filenameNetLog='./data/results_siam/NetLog/merge.txt'; fileoutNetLogRAM='./images/NetLog_ram.png'" results_siam_ram.dem
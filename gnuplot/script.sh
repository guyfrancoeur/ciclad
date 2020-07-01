#!/bin/bash

# Enlever fin de ligne (des fichiers windows)
for elt1 in data/*/*; do
    if [ -d $elt1 ]; then
        for elt2 in data/*/*/*/*; do
            sed -i 's/\r//g' $elt2
            
        done
    else
        sed -i 's/\r//g' $elt1
    fi
done

# ---- annexe_gctree ----
paste ./data/annexe_gctree/GC-Tree.txt ./data/annexe_gctree/ciclad.txt ./data/annexe_gctree/moment.txt > ./data/annexe_gctree/merge.txt
gnuplot -p -e "filename='./data/annexe_gctree/merge.txt'; fileout='./images/annexe_gctree.png'" annexe_gctree.dem

# ---- java_all_3 ----
gnuplot -p -e "filename='./data/java_all_3/data.txt'; fileout='./images/java_all_3.png'" java_all_3.dem

# ---- results_siam ---- Retail / BMSView / ChainStore / Mushroom / Synth / Synth2 / Net-Log ----
# CPU
gnuplot -p -e "filenameRetail='./data/results_siam/cpu/retail/merge.txt'; fileoutRetailCPU='./images/retail_cpu.png';" \
-e "filenameBMSView='./data/results_siam/cpu/BMSView/merge.txt'; fileoutBMSViewCPU='./images/BMSView_cpu.png';" \
-e "filenameChainStore='./data/results_siam/cpu/ChainStore/merge.txt'; fileoutChainStoreCPU='./images/ChainStore_cpu.png';" \
-e "filenameMushroom='./data/results_siam/cpu/Mushroom/merge.txt'; fileoutMushroomCPU='./images/Mushroom_cpu.png';" \
-e "filenameSynth='./data/results_siam/cpu/Synth/merge.txt'; fileoutSynthCPU='./images/Synth_cpu.png';" \
-e "filenameSynth2='./data/results_siam/cpu/Synth2/merge.txt'; fileoutSynth2CPU='./images/Synth2_cpu.png';" \
-e "filenameNetLog='./data/results_siam/cpu/NetLog/merge.txt'; fileoutNetLogCPU='./images/NetLog_cpu.png'" results_siam_cpu.dem
paste ./data/results_siam/cpu/Retail/ciclad.txt ./data/results_siam/cpu/Retail/moment.txt ./data/results_siam/cpu/Retail/newmoment.txt ./data/results_siam/cpu/Retail/clostream.txt ./data/results_siam/cpu/Retail/cfi-stream.txt > ./data/results_siam/cpu/Retail/merge.txt
paste ./data/results_siam/cpu/BMSView/ciclad.txt ./data/results_siam/cpu/BMSView/moment.txt ./data/results_siam/cpu/BMSView/newmoment.txt ./data/results_siam/cpu/BMSView/clostream.txt ./data/results_siam/cpu/BMSView/cfi-stream.txt > ./data/results_siam/cpu/BMSView/merge.txt
paste ./data/results_siam/cpu/ChainStore/ciclad.txt ./data/results_siam/cpu/ChainStore/moment.txt ./data/results_siam/cpu/ChainStore/newmoment.txt ./data/results_siam/cpu/ChainStore/clostream.txt ./data/results_siam/cpu/ChainStore/cfi-stream.txt > ./data/results_siam/cpu/ChainStore/merge.txt
paste ./data/results_siam/cpu/Mushroom/ciclad.txt ./data/results_siam/cpu/Mushroom/moment.txt ./data/results_siam/cpu/Mushroom/newmoment.txt ./data/results_siam/cpu/Mushroom/clostream.txt ./data/results_siam/cpu/Mushroom/cfi-stream.txt > ./data/results_siam/cpu/Mushroom/merge.txt
paste ./data/results_siam/cpu/Synth/ciclad.txt ./data/results_siam/cpu/Synth/moment.txt ./data/results_siam/cpu/Synth/newmoment.txt ./data/results_siam/cpu/Synth/clostream.txt ./data/results_siam/cpu/Synth/cfi-stream.txt > ./data/results_siam/cpu/Synth/merge.txt
paste ./data/results_siam/cpu/Synth2/ciclad.txt ./data/results_siam/cpu/Synth2/moment.txt ./data/results_siam/cpu/Synth2/newmoment.txt ./data/results_siam/cpu/Synth2/clostream.txt ./data/results_siam/cpu/Synth2/cfi-stream.txt > ./data/results_siam/cpu/Synth2/merge.txt
paste ./data/results_siam/cpu/NetLog/ciclad.txt ./data/results_siam/cpu/NetLog/moment.txt ./data/results_siam/cpu/NetLog/newmoment.txt ./data/results_siam/cpu/NetLog/clostream.txt ./data/results_siam/cpu/NetLog/cfi-stream.txt > ./data/results_siam/cpu/NetLog/merge.txt


# RAM
paste ./data/results_siam/ram/Retail/ciclad.txt ./data/results_siam/ram/Retail/moment.txt ./data/results_siam/ram/Retail/newmoment.txt ./data/results_siam/ram/Retail/clostream.txt ./data/results_siam/ram/Retail/cfi-stream.txt > ./data/results_siam/ram/Retail/merge.txt
paste ./data/results_siam/ram/BMSView/ciclad.txt ./data/results_siam/ram/BMSView/moment.txt ./data/results_siam/ram/BMSView/newmoment.txt ./data/results_siam/ram/BMSView/clostream.txt ./data/results_siam/ram/BMSView/cfi-stream.txt > ./data/results_siam/ram/BMSView/merge.txt
paste ./data/results_siam/ram/ChainStore/ciclad.txt ./data/results_siam/ram/ChainStore/moment.txt ./data/results_siam/ram/ChainStore/newmoment.txt ./data/results_siam/ram/ChainStore/clostream.txt ./data/results_siam/ram/ChainStore/cfi-stream.txt > ./data/results_siam/ram/ChainStore/merge.txt
paste ./data/results_siam/ram/Mushroom/ciclad.txt ./data/results_siam/ram/Mushroom/moment.txt ./data/results_siam/ram/Mushroom/newmoment.txt ./data/results_siam/ram/Mushroom/clostream.txt ./data/results_siam/ram/Mushroom/cfi-stream.txt > ./data/results_siam/ram/Mushroom/merge.txt
paste ./data/results_siam/ram/Synth/ciclad.txt ./data/results_siam/ram/Synth/moment.txt ./data/results_siam/ram/Synth/newmoment.txt ./data/results_siam/ram/Synth/clostream.txt ./data/results_siam/ram/Synth/cfi-stream.txt > ./data/results_siam/ram/Synth/merge.txt
paste ./data/results_siam/ram/Synth2/ciclad.txt ./data/results_siam/ram/Synth2/moment.txt ./data/results_siam/ram/Synth2/newmoment.txt ./data/results_siam/ram/Synth2/clostream.txt ./data/results_siam/ram/Synth2/cfi-stream.txt > ./data/results_siam/ram/Synth2/merge.txt
paste ./data/results_siam/ram/NetLog/ciclad.txt ./data/results_siam/ram/NetLog/moment.txt ./data/results_siam/ram/NetLog/newmoment.txt ./data/results_siam/ram/NetLog/clostream.txt ./data/results_siam/ram/NetLog/cfi-stream.txt > ./data/results_siam/ram/NetLog/merge.txt
gnuplot -p -e "filenameRetail='./data/results_siam/ram/retail/merge.txt'; fileoutRetailRAM='./images/retail_ram.png';" \
-e "filenameBMSView='./data/results_siam/ram/BMSView/merge.txt'; fileoutBMSViewRAM='./images/BMSView_ram.png';" \
-e "filenameChainStore='./data/results_siam/ram/ChainStore/merge.txt'; fileoutChainStoreRAM='./images/ChainStore_ram.png';" \
-e "filenameMushroom='./data/results_siam/ram/Mushroom/merge.txt'; fileoutMushroomRAM='./images/Mushroom_ram.png';" \
-e "filenameSynth='./data/results_siam/ram/Synth/merge.txt'; fileoutSynthRAM='./images/Synth_ram.png';" \
-e "filenameSynth2='./data/results_siam/ram/Synth2/merge.txt'; fileoutSynth2RAM='./images/Synth2_ram.png';" \
-e "filenameNetLog='./data/results_siam/ram/NetLog/merge.txt'; fileoutNetLogRAM='./images/NetLog_ram.png'" results_siam_ram.dem
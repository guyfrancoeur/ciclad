#!/bin/bash

# Enlever fin de ligne (des fichiers windows)
#for elt1 in data/*/*; do
#  if [ -d $elt1 ]; then
#    for elt2 in data/*/*/*; do
#      if [ -d $elt2 ]; then
#        for elt3 in data/*/*/*/*; do
#          sed -i 's/\r//g' $elt3
#        done
#      else
#        sed -i 's/\r//g' $elt2
#      fi
#    done
#  else
#    sed -i 's/\r//g' $elt1
#  fi
#done

# ---- annexe_gctree ----
paste ./data/annexe_gctree/x-axis.txt ./data/annexe_gctree/GC-Tree.txt ./data/annexe_gctree/ciclad.txt ./data/annexe_gctree/moment.txt > ./data/annexe_gctree/merge.txt
gnuplot -p -e "filename='./data/annexe_gctree/merge.txt'; fileout='./images/annexe_gctree.png'" annexe_gctree.dem

# ---- ciclad_vs_moment ---- Runtime & Memory
paste ./data/ciclad_vs_moment/runtime/x-axis.txt ./data/ciclad_vs_moment/runtime/ciclad.txt ./data/ciclad_vs_moment/runtime/moment.txt > ./data/ciclad_vs_moment/runtime/merge.txt
paste ./data/ciclad_vs_moment/memory/x-axis.txt ./data/ciclad_vs_moment/memory/ciclad.txt ./data/ciclad_vs_moment/memory/moment.txt > ./data/ciclad_vs_moment/memory/merge.txt
gnuplot -p -e "filenameRUNTIME='./data/ciclad_vs_moment/runtime/merge.txt'; filenameMEMORY='./data/ciclad_vs_moment/memory/merge.txt'; fileoutRUNTIME='./images/ciclad_vs_moment_runtime.png'; fileoutMEMORY='./images/ciclad_vs_moment_memory.png'" ciclad_vs_moment_memory.dem

# ---- java_all_3 ----
gnuplot -p -e "filename='./data/java_all_3/data.txt'; fileout='./images/java_all_3.png'" java_all_3.dem

# ---- results_siam ---- Retail / BMSView / ChainStore / Mushroom / Synth / Synth2 / Net-Log ----
# CPU
paste ./data/results_siam/cpu/Retail/x-axis.txt ./data/results_siam/cpu/Retail/ciclad.txt ./data/results_siam/cpu/Retail/moment.txt ./data/results_siam/cpu/Retail/newmoment.txt ./data/results_siam/cpu/Retail/clostream.txt ./data/results_siam/cpu/Retail/cfi-stream.txt > ./data/results_siam/cpu/Retail/merge.txt
paste ./data/results_siam/cpu/BMSView/x-axis.txt ./data/results_siam/cpu/BMSView/ciclad.txt ./data/results_siam/cpu/BMSView/moment.txt ./data/results_siam/cpu/BMSView/newmoment.txt ./data/results_siam/cpu/BMSView/clostream.txt ./data/results_siam/cpu/BMSView/cfi-stream.txt > ./data/results_siam/cpu/BMSView/merge.txt
paste ./data/results_siam/cpu/ChainStore/x-axis.txt ./data/results_siam/cpu/ChainStore/ciclad.txt ./data/results_siam/cpu/ChainStore/moment.txt ./data/results_siam/cpu/ChainStore/newmoment.txt ./data/results_siam/cpu/ChainStore/clostream.txt ./data/results_siam/cpu/ChainStore/cfi-stream.txt > ./data/results_siam/cpu/ChainStore/merge.txt
paste ./data/results_siam/cpu/Mushroom/x-axis.txt ./data/results_siam/cpu/Mushroom/ciclad.txt ./data/results_siam/cpu/Mushroom/moment.txt ./data/results_siam/cpu/Mushroom/newmoment.txt ./data/results_siam/cpu/Mushroom/clostream.txt ./data/results_siam/cpu/Mushroom/cfi-stream.txt > ./data/results_siam/cpu/Mushroom/merge.txt
paste ./data/results_siam/cpu/Synth/x-axis.txt ./data/results_siam/cpu/Synth/ciclad.txt ./data/results_siam/cpu/Synth/moment.txt ./data/results_siam/cpu/Synth/newmoment.txt ./data/results_siam/cpu/Synth/clostream.txt ./data/results_siam/cpu/Synth/cfi-stream.txt > ./data/results_siam/cpu/Synth/merge.txt
paste ./data/results_siam/cpu/Synth2/x-axis.txt ./data/results_siam/cpu/Synth2/ciclad.txt ./data/results_siam/cpu/Synth2/moment.txt ./data/results_siam/cpu/Synth2/newmoment.txt ./data/results_siam/cpu/Synth2/clostream.txt ./data/results_siam/cpu/Synth2/cfi-stream.txt > ./data/results_siam/cpu/Synth2/merge.txt
paste ./data/results_siam/cpu/NetLog/x-axis.txt ./data/results_siam/cpu/NetLog/ciclad.txt ./data/results_siam/cpu/NetLog/moment.txt ./data/results_siam/cpu/NetLog/newmoment.txt ./data/results_siam/cpu/NetLog/clostream.txt ./data/results_siam/cpu/NetLog/cfi-stream.txt > ./data/results_siam/cpu/NetLog/merge.txt
gnuplot -p -e "filename='./data/results_siam/cpu/retail/merge.txt'; fileout='./images/retail_cpu.png'; titleGiven='Retail CPU'" results_siam_cpu.dem
gnuplot -p -e "filename='./data/results_siam/cpu/BMSView/merge.txt'; fileout='./images/BMSView_cpu.png'; titleGiven='BMSView CPU'" results_siam_cpu.dem
gnuplot -p -e "filename='./data/results_siam/cpu/ChainStore/merge.txt'; fileout='./images/ChainStore_cpu.png'; titleGiven='ChainStore CPU'" results_siam_cpu.dem
gnuplot -p -e "filename='./data/results_siam/cpu/Mushroom/merge.txt'; fileout='./images/Mushroom_cpu.png'; titleGiven='Mushroom CPU'" results_siam_cpu.dem
gnuplot -p -e "filename='./data/results_siam/cpu/Synth/merge.txt'; fileout='./images/Synth_cpu.png'; titleGiven='Synth CPU'" results_siam_cpu.dem
gnuplot -p -e "filename='./data/results_siam/cpu/Synth2/merge.txt'; fileout='./images/Synth2_cpu.png'; titleGiven='Synth2 CPU'" results_siam_cpu.dem
gnuplot -p -e "filename='./data/results_siam/cpu/NetLog/merge.txt'; fileout='./images/NetLog_cpu.png'; titleGiven='NetLog CPU'" results_siam_cpu.dem

# RAM
paste ./data/results_siam/ram/Retail/x-axis.txt ./data/results_siam/ram/Retail/ciclad.txt ./data/results_siam/ram/Retail/moment.txt ./data/results_siam/ram/Retail/newmoment.txt ./data/results_siam/ram/Retail/clostream.txt ./data/results_siam/ram/Retail/cfi-stream.txt > ./data/results_siam/ram/Retail/merge.txt
paste ./data/results_siam/ram/BMSView/x-axis.txt ./data/results_siam/ram/BMSView/ciclad.txt ./data/results_siam/ram/BMSView/moment.txt ./data/results_siam/ram/BMSView/newmoment.txt ./data/results_siam/ram/BMSView/clostream.txt ./data/results_siam/ram/BMSView/cfi-stream.txt > ./data/results_siam/ram/BMSView/merge.txt
paste ./data/results_siam/ram/ChainStore/x-axis.txt ./data/results_siam/ram/ChainStore/ciclad.txt ./data/results_siam/ram/ChainStore/moment.txt ./data/results_siam/ram/ChainStore/newmoment.txt ./data/results_siam/ram/ChainStore/clostream.txt ./data/results_siam/ram/ChainStore/cfi-stream.txt > ./data/results_siam/ram/ChainStore/merge.txt
paste ./data/results_siam/ram/Mushroom/x-axis.txt ./data/results_siam/ram/Mushroom/ciclad.txt ./data/results_siam/ram/Mushroom/moment.txt ./data/results_siam/ram/Mushroom/newmoment.txt ./data/results_siam/ram/Mushroom/clostream.txt ./data/results_siam/ram/Mushroom/cfi-stream.txt > ./data/results_siam/ram/Mushroom/merge.txt
paste ./data/results_siam/ram/Synth/x-axis.txt ./data/results_siam/ram/Synth/ciclad.txt ./data/results_siam/ram/Synth/moment.txt ./data/results_siam/ram/Synth/newmoment.txt ./data/results_siam/ram/Synth/clostream.txt ./data/results_siam/ram/Synth/cfi-stream.txt > ./data/results_siam/ram/Synth/merge.txt
paste ./data/results_siam/ram/Synth2/x-axis.txt ./data/results_siam/ram/Synth2/ciclad.txt ./data/results_siam/ram/Synth2/moment.txt ./data/results_siam/ram/Synth2/newmoment.txt ./data/results_siam/ram/Synth2/clostream.txt ./data/results_siam/ram/Synth2/cfi-stream.txt > ./data/results_siam/ram/Synth2/merge.txt
paste ./data/results_siam/ram/NetLog/x-axis.txt ./data/results_siam/ram/NetLog/ciclad.txt ./data/results_siam/ram/NetLog/moment.txt ./data/results_siam/ram/NetLog/newmoment.txt ./data/results_siam/ram/NetLog/clostream.txt ./data/results_siam/ram/NetLog/cfi-stream.txt > ./data/results_siam/ram/NetLog/merge.txt
gnuplot -p -e "filename='./data/results_siam/ram/retail/merge.txt'; fileout='./images/retail_ram.png'; titleGiven='Retail RAM'" results_siam_ram.dem
gnuplot -p -e "filename='./data/results_siam/ram/BMSView/merge.txt'; fileout='./images/BMSView_ram.png'; titleGiven='BMSView RAM'" results_siam_ram.dem
gnuplot -p -e "filename='./data/results_siam/ram/ChainStore/merge.txt'; fileout='./images/ChainStore_ram.png'; titleGiven='ChainStore RAM'" results_siam_ram.dem
gnuplot -p -e "filename='./data/results_siam/ram/Mushroom/merge.txt'; fileout='./images/Mushroom_ram.png'; titleGiven='Mushroom RAM'" results_siam_ram.dem
gnuplot -p -e "filename='./data/results_siam/ram/Synth/merge.txt'; fileout='./images/Synth_ram.png'; titleGiven='Synth RAM'" results_siam_ram.dem
gnuplot -p -e "filename='./data/results_siam/ram/Synth2/merge.txt'; fileout='./images/Synth2_ram.png'; titleGiven='Synth2 RAM'" results_siam_ram.dem
gnuplot -p -e "filename='./data/results_siam/ram/NetLog/merge.txt'; fileout='./images/NetLog_ram.png'; titleGiven='NetLog RAM'" results_siam_ram.dem
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

#ou somme nous? dans gnuplot !
pwd

# annexe_gctree | java_all_3
paste ./data/annexe_gctree/x-axis.txt ./data/annexe_gctree/GC-Tree.txt ./data/annexe_gctree/ciclad.txt ./data/annexe_gctree/moment.txt > ./data/annexe_gctree/merge.txt
gnuplot -p -e "filenameGcTree='./data/annexe_gctree/merge.txt'; fileoutGcTree='./images/annexe_gctree.png';" \
  -e "filename_java_all_3='./data/java_all_3/data.txt'; fileout_java_all_3='./images/java_all_3.png'" others.dem

# ciclad_vs_moment >> Runtime | Memory
paste ./data/ciclad_vs_moment/runtime/x-axis.txt ./data/ciclad_vs_moment/runtime/ciclad.txt ./data/ciclad_vs_moment/runtime/moment.txt > ./data/ciclad_vs_moment/runtime/merge.txt
paste ./data/ciclad_vs_moment/memory/x-axis.txt ./data/ciclad_vs_moment/memory/ciclad.txt ./data/ciclad_vs_moment/memory/moment.txt > ./data/ciclad_vs_moment/memory/merge.txt
gnuplot -p -e "filenameRUNTIME='./data/ciclad_vs_moment/runtime/merge.txt'; filenameMEMORY='./data/ciclad_vs_moment/memory/merge.txt';" \
  -e "fileoutRUNTIME='./images/ciclad_vs_moment_runtime.png'; fileoutMEMORY='./images/ciclad_vs_moment_memory.png'" ciclad_vs_moment.dem

#  results_siam >> Retail | BMSView | ChainStore | Mushroom | Synth | Synth2 | Net-Log
# CPU
paste ./data/results_siam/cpu/Retail/x-axis.txt ./data/results_siam/cpu/Retail/ciclad.txt ./data/results_siam/cpu/Retail/moment.txt ./data/results_siam/cpu/Retail/newmoment.txt ./data/results_siam/cpu/Retail/clostream.txt ./data/results_siam/cpu/Retail/cfi-stream.txt > ./data/results_siam/cpu/Retail/merge.txt
paste ./data/results_siam/cpu/BMSView/x-axis.txt ./data/results_siam/cpu/BMSView/ciclad.txt ./data/results_siam/cpu/BMSView/moment.txt ./data/results_siam/cpu/BMSView/newmoment.txt ./data/results_siam/cpu/BMSView/clostream.txt ./data/results_siam/cpu/BMSView/cfi-stream.txt > ./data/results_siam/cpu/BMSView/merge.txt
paste ./data/results_siam/cpu/ChainStore/x-axis.txt ./data/results_siam/cpu/ChainStore/ciclad.txt ./data/results_siam/cpu/ChainStore/moment.txt ./data/results_siam/cpu/ChainStore/newmoment.txt ./data/results_siam/cpu/ChainStore/clostream.txt ./data/results_siam/cpu/ChainStore/cfi-stream.txt > ./data/results_siam/cpu/ChainStore/merge.txt
paste ./data/results_siam/cpu/Mushroom/x-axis.txt ./data/results_siam/cpu/Mushroom/ciclad.txt ./data/results_siam/cpu/Mushroom/moment.txt ./data/results_siam/cpu/Mushroom/newmoment.txt ./data/results_siam/cpu/Mushroom/clostream.txt ./data/results_siam/cpu/Mushroom/cfi-stream.txt > ./data/results_siam/cpu/Mushroom/merge.txt
paste ./data/results_siam/cpu/Synth/x-axis.txt ./data/results_siam/cpu/Synth/ciclad.txt ./data/results_siam/cpu/Synth/moment.txt ./data/results_siam/cpu/Synth/newmoment.txt ./data/results_siam/cpu/Synth/clostream.txt ./data/results_siam/cpu/Synth/cfi-stream.txt > ./data/results_siam/cpu/Synth/merge.txt
paste ./data/results_siam/cpu/Synth2/x-axis.txt ./data/results_siam/cpu/Synth2/ciclad.txt ./data/results_siam/cpu/Synth2/moment.txt ./data/results_siam/cpu/Synth2/newmoment.txt ./data/results_siam/cpu/Synth2/clostream.txt ./data/results_siam/cpu/Synth2/cfi-stream.txt > ./data/results_siam/cpu/Synth2/merge.txt
#pour rendre ca simple ************
cd ./data/results_siam/cpu/NetLog
#OLD paste ./data/results_siam/cpu/NetLog/x-axis.txt ./data/results_siam/cpu/NetLog/ciclad.txt ./data/results_siam/cpu/NetLog/moment.txt ./data/results_siam/cpu/NetLog/newmoment.txt ./data/results_siam/cpu/NetLog/clostream.txt ./data/results_siam/cpu/NetLog/cfi-stream.txt > ./data/results_siam/cpu/NetLog/merge.txt
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../../../../
# RAM
paste ./data/results_siam/ram/Retail/x-axis.txt ./data/results_siam/ram/Retail/ciclad.txt ./data/results_siam/ram/Retail/moment.txt ./data/results_siam/ram/Retail/newmoment.txt ./data/results_siam/ram/Retail/clostream.txt ./data/results_siam/ram/Retail/cfi-stream.txt > ./data/results_siam/ram/Retail/merge.txt
paste ./data/results_siam/ram/BMSView/x-axis.txt ./data/results_siam/ram/BMSView/ciclad.txt ./data/results_siam/ram/BMSView/moment.txt ./data/results_siam/ram/BMSView/newmoment.txt ./data/results_siam/ram/BMSView/clostream.txt ./data/results_siam/ram/BMSView/cfi-stream.txt > ./data/results_siam/ram/BMSView/merge.txt
paste ./data/results_siam/ram/ChainStore/x-axis.txt ./data/results_siam/ram/ChainStore/ciclad.txt ./data/results_siam/ram/ChainStore/moment.txt ./data/results_siam/ram/ChainStore/newmoment.txt ./data/results_siam/ram/ChainStore/clostream.txt ./data/results_siam/ram/ChainStore/cfi-stream.txt > ./data/results_siam/ram/ChainStore/merge.txt
paste ./data/results_siam/ram/Mushroom/x-axis.txt ./data/results_siam/ram/Mushroom/ciclad.txt ./data/results_siam/ram/Mushroom/moment.txt ./data/results_siam/ram/Mushroom/newmoment.txt ./data/results_siam/ram/Mushroom/clostream.txt ./data/results_siam/ram/Mushroom/cfi-stream.txt > ./data/results_siam/ram/Mushroom/merge.txt
paste ./data/results_siam/ram/Synth/x-axis.txt ./data/results_siam/ram/Synth/ciclad.txt ./data/results_siam/ram/Synth/moment.txt ./data/results_siam/ram/Synth/newmoment.txt ./data/results_siam/ram/Synth/clostream.txt ./data/results_siam/ram/Synth/cfi-stream.txt > ./data/results_siam/ram/Synth/merge.txt
paste ./data/results_siam/ram/Synth2/x-axis.txt ./data/results_siam/ram/Synth2/ciclad.txt ./data/results_siam/ram/Synth2/moment.txt ./data/results_siam/ram/Synth2/newmoment.txt ./data/results_siam/ram/Synth2/clostream.txt ./data/results_siam/ram/Synth2/cfi-stream.txt > ./data/results_siam/ram/Synth2/merge.txt

#OLD paste ./data/results_siam/ram/NetLog/x-axis.txt ./data/results_siam/ram/NetLog/ciclad.txt ./data/results_siam/ram/NetLog/moment.txt ./data/results_siam/ram/NetLog/newmoment.txt ./data/results_siam/ram/NetLog/clostream.txt ./data/results_siam/ram/NetLog/cfi-stream.txt > ./data/results_siam/ram/NetLog/merge.txt
cd ./data/results_siam/ram/NetLog
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../../../../
pwd

# Retail
gnuplot -p -e "filenameCPU='./data/results_siam/cpu/Retail/merge.txt'; fileoutCPU='./images/retail_cpu.png'" \
  -e "filenameRAM='./data/results_siam/ram/Retail/merge.txt'; fileoutRAM='./images/retail_ram.png'; title='Retail'" results_siam.dem
# BMSView
gnuplot -p -e "filenameCPU='./data/results_siam/cpu/BMSView/merge.txt'; fileoutCPU='./images/BMSView_cpu.png';" \
  -e "filenameRAM='./data/results_siam/ram/BMSView/merge.txt'; fileoutRAM='./images/BMSView_ram.png'; title='BMSView'" results_siam.dem
# ChainStore
gnuplot -p -e "filenameCPU='./data/results_siam/cpu/ChainStore/merge.txt'; fileoutCPU='./images/ChainStore_cpu.png';" \
  -e "filenameRAM='./data/results_siam/ram/ChainStore/merge.txt'; fileoutRAM='./images/ChainStore_ram.png'; title='ChainStore'" results_siam.dem
# Mushroom
gnuplot -p -e "filenameCPU='./data/results_siam/cpu/Mushroom/merge.txt'; fileoutCPU='./images/Mushroom_cpu.png';" \
  -e "filenameRAM='./data/results_siam/ram/Mushroom/merge.txt'; fileoutRAM='./images/Mushroom_ram.png'; title='Mushroom'" results_siam.dem
# Synth
gnuplot -p -e "filenameCPU='./data/results_siam/cpu/Synth/merge.txt'; fileoutCPU='./images/Synth_cpu.png';" \
  -e "filenameRAM='./data/results_siam/ram/Synth/merge.txt'; fileoutRAM='./images/Synth_ram.png'; title='Synth'" results_siam.dem
# Synth2
gnuplot -p -e "filenameCPU='./data/results_siam/cpu/Synth2/merge.txt'; fileoutCPU='./images/Synth2_cpu.png';" \
  -e "filenameRAM='./data/results_siam/ram/Synth2/merge.txt'; fileoutRAM='./images/Synth2_ram.png'; title='Synth2'" results_siam.dem
# NetLog
gnuplot -p -e "filenameCPU='./data/results_siam/cpu/NetLog/merge.txt'; fileoutCPU='./images/NetLog_cpu.png';" \
  -e "filenameRAM='./data/results_siam/ram/NetLog/merge.txt'; fileoutRAM='./images/NetLog_ram.png'; title='NetLog'" results_siam.dem

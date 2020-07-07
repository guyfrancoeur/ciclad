#!/bin/bash

#ou somme nous? dans gnuplot !
pwd

# annexe_gctree | java_all_3
cd ./data/annexe_gctree
paste x-axis.txt GC-Tree.txt ciclad.txt moment.txt > merge.txt
cd ../../
gnuplot -p -e "filenameGcTree='./data/annexe_gctree/merge.txt'; fileoutGcTree='./images/annexe_gctree.png';" \
  -e "filename_java_all_3='./data/java_all_3/data.txt'; fileout_java_all_3='./images/java_all_3.png'" others.dem

# ciclad_vs_moment >> Runtime | Memory
cd ./data/ciclad_vs_moment/runtime
paste x-axis.txt ciclad.txt moment.txt > merge.txt
cd ../memory
paste x-axis.txt ciclad.txt moment.txt > merge.txt
cd ../../../
gnuplot -p -e "filenameRUNTIME='./data/ciclad_vs_moment/runtime/merge.txt'; filenameMEMORY='./data/ciclad_vs_moment/memory/merge.txt';" \
  -e "fileoutRUNTIME='./images/ciclad_vs_moment_runtime.png'; fileoutMEMORY='./images/ciclad_vs_moment_memory.png'" ciclad_vs_moment.dem

#  results_siam >> Retail | BMSView | ChainStore | Mushroom | Synth | Synth2 | Net-Log
# CPU
cd ./data/results_siam/cpu/Retail
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../BMSView
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../ChainStore
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../Mushroom
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../Synth
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../Synth2
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../NetLog
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../../../../

# RAM
cd ./data/results_siam/ram/Retail
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../BMSView
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../ChainStore
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../Mushroom
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../Synth
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../Synth2
paste x-axis.txt ciclad.txt moment.txt newmoment.txt clostream.txt cfi-stream.txt > merge.txt
cd ../NetLog
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

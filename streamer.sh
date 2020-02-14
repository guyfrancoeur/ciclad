#!/bin/bash
echo "data streamer simulator 2019 (c) GF"
echo "-----------------------------------"

if [ -z $1 ]
then
  f="./testdb_gen2.txt"
else
  f=$1
fi
echo "processing file : $f"
row=0
fun=1
if [ -z $2 ]
then
  cnt=$(wc -l "$f" | awk '{print $1}')
  cnt=$(( $cnt / 2 ))
else
  cnt=$2
fi
echo "Sliding window size : $cnt"
rm -f ./stream.in
touch ./stream.in
rm -f ./window.current
touch window.current
echo "Launch NOW using : tail -f ./stream.in | ./ciclad_v3/ciclad_v3 -v 2> ./resultat.out"
echo -e "\nstarting in ...\n"
for (( i=20 ; i>0 ; --i ))
do
  sleep 1
  echo -n "$i "
done
#start ciclad
# tail -f ./stream.in | ./ciclad_v3/ciclad_v3 -v 2> ./resultat.out &
echo -e "Initialisation done.\n"
echo -e "\nStarted...\n"
fun=7
while read line
do
  row=$(wc -l "./window.current" | awk '{print $1}')
  if [ $row -gt $cnt ]
  then
    ligne=$(head -n 1 ./window.current)
    echo "${ligne}" >> ./stream.in
    sed -i '1d' ./window.current
  fi
  echo "add $line" >> ./stream.in
  echo "del $line" >> ./window.current
  if [ $fun -eq 7 ]
  then
    fun=1
  else
    (( fun++ ))
  fi

  str="working"
  i=1
  while [ $i -lt $fun ] ; do
    str="$str\b"
    (( i++ ))
  done
  echo -ne $str
  
  t=$(( ( RANDOM % 4 ) + 0 ))
  #sleep $t
done < $f
echo "end" >> ./stream.in
echo -e "\nFin.\n"

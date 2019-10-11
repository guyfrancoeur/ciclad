#!/bin/bash
if [ -z $1 ]
then
  f="./testdb_gen2.txt"
fi
echo "data streamer simulator 2019 (c) GF"
echo "-----------------------------------"
row=0
fun=1
cnt=$(wc -l "$f" | awk '{print $1}')
cnt=$(( $cnt / 2 ))
echo "Sliding window size : $cnt"
rm -f ./stream.in
touch ./stream.in
echo "Launch app using tail -f ./stream.in | ./ciclad_v3/ciclad_v3 2> ./resultats.out"
echo -e "\nstarting in ...\n"
for (( i=20 ; i>0 ; --i ))
do
  sleep 1
  echo -n "$i "
done
#start ciclad
# tail -f ./stream.in | ./ciclad_v3/ciclad_v3 < ./stream.in 2> ./resultat.out &
echo -e "Initialisation done.\n"
echo -e "\nStarted...\n"
while read line
do
  if [ $row -gt $cnt ]
  then
    ligne=$(head -n 1 ./stream.in)
    if [ ${ligne:0:3} == "add" ]
    then
      sed -i '1d' ./stream.in
    fi
    echo "del ${ligne:4}" >> ./stream.in
  fi
  echo "add $line" >> ./stream.in
  if [ $fun -eq 1 ]
  then
    echo -n "working "
    #sleep 1
    fun=9
  else
    (( fun-- ))
    echo -ne "\b"
  fi
  t=$(( ( RANDOM % 4 ) + 0 ))
  #sleep $t
  (( row++ ))
done < $f
echo -e "\nfin.\n"
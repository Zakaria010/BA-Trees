 #!/bin/bash
# mkdir ../outputDeter
# for n in FICO COMPAS-ProPublica HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
# do
# mkdir ../outputDeter/${n}
# done
for n in HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
#for u in {1..10}
#do
line1=$(sed -n '10p' ../resources/datasets/$n/$n.train2.csv)
trimmed_line1=$(echo "$line1" | sed 's/,[^,]*$//')
line2=$(sed -n '10p' ../resources/datasets/$n/$n.train2.csv)
trimmed_line2=$(echo "$line2" | sed 's/,[^,]*$//')
line3=$(sed -n '10p' ../resources/datasets/$n/$n.train3.csv)
trimmed_line3=$(echo "$line3" | sed 's/,[^,]*$//')
line4=$(sed -n '10p' ../resources/datasets/$n/$n.train4.csv)
trimmed_line4=$(echo "$line4" | sed 's/,[^,]*$//')
line5=$(sed -n '10p' ../resources/datasets/$n/$n.train5.csv)
trimmed_line5=$(echo "$line5" | sed 's/,[^,]*$//')
line6=$(sed -n '10p' ../resources/datasets/$n/$n.train6.csv)
trimmed_line6=$(echo "$line6" | sed 's/,[^,]*$//')
line7=$(sed -n '10p' ../resources/datasets/$n/$n.train7.csv)
trimmed_line7=$(echo "$line7" | sed 's/,[^,]*$//')
line8=$(sed -n '10p' ../resources/datasets/$n/$n.train8.csv)
trimmed_line8=$(echo "$line8" | sed 's/,[^,]*$//')
line9=$(sed -n '10p' ../resources/datasets/$n/$n.train9.csv)
trimmed_line9=$(echo "$line9" | sed 's/,[^,]*$//')
line10=$(sed -n '10p' ../resources/datasets/$n/$n.train10.csv)
trimmed_line10=$(echo "$line10" | sed 's/,[^,]*$//')
for d in 9 8 7 6 5 4
do
./bornAgain ../resources/forests/$n/${n}.RF1.txt ../outputDeter/${n}/${n}.BA1.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line1 &
./bornAgain ../resources/forests/$n/${n}.RF2.txt ../outputDeter/${n}/${n}.BA2.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line2 &
./bornAgain ../resources/forests/$n/${n}.RF3.txt ../outputDeter/${n}/${n}.BA3.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line3 &
./bornAgain ../resources/forests/$n/${n}.RF4.txt ../outputDeter/${n}/${n}.BA4.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line4 &
./bornAgain ../resources/forests/$n/${n}.RF5.txt ../outputDeter/${n}/${n}.BA5.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line5 &
./bornAgain ../resources/forests/$n/${n}.RF6.txt ../outputDeter/${n}/${n}.BA6.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line6 &
./bornAgain ../resources/forests/$n/${n}.RF7.txt ../outputDeter/${n}/${n}.BA7.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line7 &
wait
./bornAgain ../resources/forests/$n/${n}.RF8.txt ../outputDeter/${n}/${n}.BA8.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line8 &
./bornAgain ../resources/forests/$n/${n}.RF9.txt ../outputDeter/${n}/${n}.BA9.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line9 &
./bornAgain ../resources/forests/$n/${n}.RF10.txt ../outputDeter/${n}/${n}.BA10.D$d  -obj 0 -depth $d -method 1 -instance $trimmed_line10 &
wait
#done
done
done
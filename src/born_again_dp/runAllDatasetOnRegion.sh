 #!/bin/bash
mkdir ../outputDeter
for n in FICO COMPAS-ProPublica HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
mkdir ../outputDeter/${n}
done
for n in FICO
do
for u in {1..10}
do
line=$(sed -n '10p' ../resources/datasets/$n/$n.train$u.csv)
trimmed_line=$(echo "$line" | sed 's/,[^,]*$//')
#for d in 10 9 8 7 6 5
#do
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputDeter/${n}/${n}.BA$u.D9  -obj 0 -depth 9 -method 1 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputDeter/${n}/${n}.BA$u.D8  -obj 0 -depth 8 -method 1 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputDeter/${n}/${n}.BA$u.D7  -obj 0 -depth 7 -method 1 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputDeter/${n}/${n}.BA$u.D6  -obj 0 -depth 6 -method 1 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputDeter/${n}/${n}.BA$u.D5  -obj 0 -depth 5 -method 1 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputDeter/${n}/${n}.BA$u.D4  -obj 0 -depth 4 -method 1 -instance $trimmed_line &
wait
done
#done
done
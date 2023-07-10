 #!/bin/bash
mkdir ../outputHeuristic
for n in FICO COMPAS-ProPublica HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
mkdir ../outputHeuristic/${n}
done
for n in FICO COMPAS-ProPublica HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
for u in {1..10}
do
line=$(sed -n '10p' ../resources/datasets/$n/$n.train$u.csv)
trimmed_line=$(echo "$line" | sed 's/,[^,]*$//')
#for d in 10 9 8 7 6 5
#do
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputHeuristic/${n}/${n}.BA$u.D9  -obj 0 -depth 9 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputHeuristic/${n}/${n}.BA$u.D8  -obj 0 -depth 8 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputHeuristic/${n}/${n}.BA$u.D7  -obj 0 -depth 7 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputHeuristic/${n}/${n}.BA$u.D6  -obj 0 -depth 6 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputHeuristic/${n}/${n}.BA$u.D5  -obj 0 -depth 5 -instance $trimmed_line &
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../outputHeuristic/${n}/${n}.BA$u.D4  -obj 0 -depth 4 -instance $trimmed_line &
wait
done
#done
done
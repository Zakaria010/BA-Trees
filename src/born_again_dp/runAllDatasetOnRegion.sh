 #!/bin/bash
mkdir ../output
for n in COMPAS-ProPublica FICO HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
mkdir ../output/${n}
done
for n in COMPAS-ProPublica FICO HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
for u in {1..10}
do
line=$(sed -n '10p' ../resources/datasets/$n/$n.train$u.csv)
trimmed_line=$(echo "$line" | sed 's/,[^,]*$//')
for d in {3..10}
do
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../output/${n}/${n}.BA$u.D$d  -obj 0 -depth $d -instance $trimmed_line
done
done
done
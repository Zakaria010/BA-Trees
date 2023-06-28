 #!/bin/bash
mkdir ../new_output
for n in FICO COMPAS-ProPublica HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
mkdir ../new_output/${n}
done
for n in FICO COMPAS-ProPublica HTRU2 Pima-Diabetes Seeds Breast-Cancer-Wisconsin
do
for u in {1..10}
do
line=$(sed -n '10p' ../resources/datasets/$n/$n.train$u.csv)
trimmed_line=$(echo "$line" | sed 's/,[^,]*$//')
for d in 10 9 8 7 6 5
do
./bornAgain ../resources/forests/$n/${n}.RF$u.txt ../new_output/${n}/${n}.BA$u.D$d  -obj 0 -depth $d -instance $trimmed_line
done
done
done
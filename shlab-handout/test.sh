#! /bin/bash

touch tsh.out
make test01 > tsh.out
for i in 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
do
    make test$i >> tsh.out
done

sed -i 's|(\b[0-9]*)|(1000)|g;' tsh.out
sed -i 's|(\b[0-9]*)|(1000)|g;' tshref.out

touch diff.out
echo "蓝俊玮 PB20111689" > diff.out
whoami >> diff.out
diff tsh.out tshref.out >> diff.out
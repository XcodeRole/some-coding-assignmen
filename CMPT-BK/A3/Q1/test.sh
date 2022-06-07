for file in `ls ./A3Q1_Examples`
do
    rm $file'input'
    echo 'A3Q1_Examples'/$file > $file'input'
done

if [ -f "everyexample.log" ];then
    rm everyexample.log
fi


for file in `ls ./A3Q1_Examples`
do
    echo -e "=============$file===============" >> everyexample.log
    python a3q1.py  < $file'input' >> everyexample.log
    echo "" >> everyexample.log
done

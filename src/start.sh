set -ux
ulimit -c unlimited
export LD_LIBRARY_PATH=/usr/local/lib/:./lib

make

date=`date +%Y%m%d`

PID=`netstat -apn | grep 8808 | grep LISTEN | awk '{print $7}' | awk -F'/' '{print $1}'`

if [ -n "$PID" ]; then 
        echo "killing PID: $PID"
        kill $PID > /dev/null 2>&1 
        kill -9 $PID > /dev/null 2>&1 
fi

##############################

if [ ! -d "lib" ]; then
    mkdir "lib"
fi

rm -rf "lib/*"

cp common/json/.libs/libjson.so* lib
cp common/json/.libs/libjson.a lib



if [ ! -d "bin" ]; then
    mkdir "bin"
fi

rm -rf "bin/*"

cp Worker/.libs/worker_server bin 

#tar -zcvf yanzhi_server.tar.gz bin lib data/dmaiml data/dmaiml_wenfatang data/nlu data/nlu_wenfatang conf start_online.sh data/questions.txt data/surfix_unigram.txt data/prefix_unigram.txt
##############################
nohup ./bin/worker_server conf/worker.cfg 1>log 2>err.log &
#echo "start ok" 


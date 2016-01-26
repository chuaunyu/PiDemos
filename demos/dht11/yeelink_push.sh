#!/bin/bash
#push into yeelink

if [ $# -lt 5 ] ;then
    echo Parameters invalid!
    echo Usage:yeelink_push.sh yeelinkToken deviceIdOfHumidity sensorIdOfHumidity deviceIdOfTemperature sensorIdOfTemperature
    exit
fi

dir=$(cd "$(dirname "$0")/.."; pwd)
data=`${dir}/target/dht11`
echo '[INFO]'$data' - '`date "+%Y-%m-%d %H:%M:%S"` >> ${dir}/target/~dht11.log
suc=`echo $data | grep Success | wc -l`
if [ "$suc" = "1" ] ;then
    curr=`date '+%Y-%m-%dT%H:%M:%S'`
    hum=`echo ${data#*Humidity:*}`
    hum=`echo ${hum%\%*}`
    tem=`echo ${data#*Temperature:*}`
    tem=`echo ${tem%C*}`

    echo \{\"timestamp\":\"$curr\",\"value\":$hum\} > ${dir}/target/~dht11hum.txt
    echo \{\"timestamp\":\"$curr\",\"value\":$tem\} > ${dir}/target/~dht11tem.txt

    curl --request POST --data-binary @${dir}/target/~dht11hum.txt --header "U-ApiKey: $1" http://api.yeelink.net/v1.0/device/$2/sensor/$3/datapoints
    curl --request POST --data-binary @${dir}/target/~dht11tem.txt --header "U-ApiKey: $1" http://api.yeelink.net/v1.0/device/$4/sensor/$5/datapoints
fi
 docker run -d -p 8086:8086 \
       --name influxdb \
       -v /Users/john.read/influxdb:/var/lib/influxdb:rw \
         influxdb

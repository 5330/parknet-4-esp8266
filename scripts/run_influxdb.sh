 docker run -d -p 8086:8086 \
       --name influxdb \
       -v /var/lib/influxdb_prod:/var/lib/influxdb:rw \
         influxdb:1.5

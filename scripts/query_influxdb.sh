# example of how to query from command line

#influx -database 'pod001' -execute 'select * from sensor_array_readout' -format json -pretty
influx -database 'pod001' -execute 'select bmeTempF from sensor_array_readout' -format column


import urllib.request, json

with urllib.request.urlopen("http://sensor003.indybungalow.com") as url1:
    data1 = json.loads(url1.read())

bmeTempF = (data1['bmeTempF'])
bmeTempC = (data1['bmeTempC'])
bmeHumidity = (data1['bmeHumidity'])
bmeDewPointF = (data1['bmeDewPointF'])
bmeDewPointC = (data1['bmeDewPointC'])
bmePressurehPa = (data1['bmePressurehPa'])
bmeApproxAltitudeF = (data1['bmeApproxAltitudeF'])
bmeApproxAltitudeM = (data1['bmeApproxAltitudeM'])
tslLux = (data1['tslLUX'])

from datetime import datetime

# reading = (datetime.now())
reading = (datetime.now().isoformat(timespec='seconds'))

from influxdb import InfluxDBClient

json_body = [
    {
        "measurement": "sensor_array_readout",
        "tags": {
            "unit": "pod003",
            "location": "breadboard"
        },
        "time": (reading),
        "fields": {
            'bmeTempF': (bmeTempF),
            'bmeTempC': (bmeTempC),
            'bmeHumidity': (bmeHumidity),
            'bmeDewPointF': (bmeDewPointF),
            'bmeDewPointC': (bmeDewPointC),
            'bmePressurehPA': (bmePressurehPa),
            'bmeApproxAltitudeF': (bmeApproxAltitudeF),
            'bmeApproxAltitudeM': (bmeApproxAltitudeM),
            'tslLux': (tslLux),
        }
    }
]

client = InfluxDBClient('localhost', 8087, 'root', 'root', 'pod001')

#client.create_database('pod001')

client.write_points(json_body)

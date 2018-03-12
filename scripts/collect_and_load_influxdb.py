import urllib.request, json

with urllib.request.urlopen("http://sensor001.indybungalow.com") as url1:
    data1 = json.loads(url1.read())

with urllib.request.urlopen("http://sensor002.indybungalow.com") as url2:
    data2 = json.loads(url2.read())

bmeTempF = (data1['bmeTempF'])
mplTempF = (data1['mplTempF'])
bmeTempC = (data1['bmeTempC'])
mplTempC = (data1['mplTempC'])
bmeHumidity = (data1['bmeHumidity'])
bmeDewPointF = (data1['bmeDewPointF'])
bmeDewPointC = (data1['bmeDewPointC'])
bmePressurehPa = (data1['bmePressurehPa'])
bmeApproxAltitudeF = (data1['bmeApproxAltitudeF'])
mplAltitudeFeet = (data1['mplAltitudeFeet'])
bmeApproxAltitudeM = (data1['bmeApproxAltitudeM'])
mplAltitudeMeters = (data1['mplAltitudeMeters'])
mplInchesHg = (data1['mplInchesHg'])
vmlUV = (data1['vml_UV'])
tslLux = (data2['tslLUX'])
siIR = (data2['siIR'])
siVIS = (data2['siVis'])
siUVindex = (data2['siUVindex'])

from datetime import datetime

# reading = (datetime.now())
reading = (datetime.now().isoformat(timespec='seconds'))

from influxdb import InfluxDBClient

json_body = [
    {
        "measurement": "sensor_array_readout",
        "tags": {
            "unit": "pod001",
            "location": "breadboard"
        },
        "time": (reading),
        "fields": {
            'bmeTempF': (bmeTempF),
            'mplTempF': (mplTempF),
            'bmeTempC': (bmeTempC),
            'mplTempC': (mplTempC),
            'bmeHumidity': (bmeHumidity),
            'bmeDewPointF': (bmeDewPointF),
            'bmeDewPointC': (bmeDewPointC),
            'bmePressurehPA': (bmePressurehPa),
            'mplInchesHg': (mplInchesHg),
            'bmeApproxAltitudeF': (bmeApproxAltitudeF),
            'mplApproxAltitudeF': (mplAltitudeFeet),
            'bmeApproxAltitudeM': (bmeApproxAltitudeM),
            'mplApproxAltitudeM': (mplAltitudeMeters),
            'vmlUV': (vmlUV),
            'tslLux': (tslLux),
            'siIR': (siIR),
            'siVIS': (siVIS),
            'siUVindex': (siUVindex)
        }
    }
]

client = InfluxDBClient('localhost', 8086, 'root', 'root', 'pod001')

# client.create_database('pod001')

client.write_points(json_body)

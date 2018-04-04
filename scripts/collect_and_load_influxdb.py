import urllib.request, json

with urllib.request.urlopen("http://pod001.indybungalow.com") as url1:
    data001 = json.loads(url1.read())

with urllib.request.urlopen("http://pod002.indybungalow.com") as url2:
    data002 = json.loads(url2.read())

with urllib.request.urlopen("http://uvpod001.indybungalow.com") as url2:
    uvdata001 = json.loads(url2.read())

with urllib.request.urlopen("http://pod003.indybungalow.com") as url2:
    data003 = json.loads(url2.read())


pod001_bmeTempF = (data001['pod001_bmeTempF'])
pod001_bmeTempC = (data001['pod001_bmeTempC'])
pod001_bmeHumidity = (data001['pod001_bmeHumidity'])
pod001_bmeDewPointC = (data001['pod001_bmeDewPointC'])
pod001_bmeDewPointF = (data001['pod001_bmeDewPointF'])
pod001_bmePressurehPa = (data001['pod001_bmePressurehPa'])
pod001_bmeApproxAltitudeM = (data001['pod001_bmeApproxAltitudeM'])
pod001_bmeApproxAltitudeF = (data001['pod001_bmeApproxAltitudeF'])
pod001_tslLUX = (data001['pod001_tslLUX'])

pod002_bmeTempF = (data002['pod002_bmeTempF'])
pod002_bmeTempC = (data002['pod002_bmeTempC'])
pod002_bmeHumidity = (data002['pod002_bmeHumidity'])
pod002_bmeDewPointC = (data002['pod002_bmeDewPointC'])
pod002_bmeDewPointF = (data002['pod002_bmeDewPointF'])
pod002_bmePressurehPa = (data002['pod002_bmePressurehPa'])
pod002_bmeApproxAltitudeM = (data002['pod002_bmeApproxAltitudeM'])
pod002_bmeApproxAltitudeF = (data002['pod002_bmeApproxAltitudeF'])
pod002_tslLUX = (data002['pod002_tslLUX'])
pod002_mplInchesHg = (data002['pod002_mplInchesHg'])
pod002_mplAltitudeMeters = (data002['pod002_mplAltitudeMeters'])
pod002_mplAltitudeFeet = (data002['pod002_mplAltitudeFeet'])
pod002_mplTempC = (data002['pod002_mplTempC'])
pod002_mplTempF = (data002['pod002_mplTempF'])

uvpod001_tslms = (uvdata001['uvpod001_tslms'])
uvpod001_tslIR = (uvdata001['uvpod001_tslIR'])
uvpod001_tslFull = (uvdata001['uvpod001_tslFull'])
uvpod001_tslVisible = (uvdata001['uvpod001_tslVisible'])
uvpod001_tslLUX = (uvdata001['uvpod001_tslLUX'])
uvpod001_siVis = (uvdata001['uvpod001_siVis'])
uvpod001_siIR = (uvdata001['uvpod001_siIR'])
uvpod001_siUVindex = (uvdata001['uvpod001_siUVindex'])
uvpod001_vmlUV = (uvdata001['uvpod001_vmlUV'])
uvpod001_mcpTempF = (uvdata001['uvpod001_mcpTempF'])
uvpod001_mcpTempC = (uvdata001['uvpod001_mcpTempC'])

pod003_bmeTempF = data003['pod003_bmeTempF'])
pod003_bmeTempC = data003['pod003_bmeTempC'])
pod003_bmeHumidity = data003['pod003_bmeHumidity'])
pod003_bmeDewPointC = data003['pod003_bmeDewPointC'])
pod003_bmeDewPointF = data003['pod003_bmeDewPointF'])
pod003_bmePressurehPa = data003['pod003_bmePressurehPa'])
pod003_bmeApproxAltitudeM = data003['pod003_bmeApproxAltitudeM'])
pod003_bmeApproxAltitudeF = data003['pod003_bmeApproxAltitudeF'])
pod003_bmeGasResistanceKOhms = data003['pod003_bmeGasResistanceKOhms'])
pod003_tslLUX = data003['pod003_tslLUX'])
pod003_mplInchesHg = data003['pod003_mplInchesHg'])
pod003_mplAltitudeMeters = data003['pod003_mplAltitudeMeters'])
pod003_mplAltitudeFeet = data003['pod003_mplAltitudeFeet'])
pod003_mplTempC = data003['pod003_mplTempC'])
pod003_mplTempF = data003['pod003_mplTempF'])

from datetime import datetime

# reading = (datetime.now())
reading = (datetime.now().isoformat(timespec='seconds'))

from influxdb import InfluxDBClient

json_body_001 = [
    {
        "measurement": "sensor_arrays",
        "tags": {
            "unit": "pod001",
            "location": "breadboard001"
        },
        "time": (reading),
        "fields": {

        'pod001_bmeTempF': (pod001_bmeTempF),
        'pod001_bmeTempC': (pod001_bmeTempC),
        'pod001_bmeHumidity': (pod001_bmeHumidity),
        'pod001_bmeDewPointC': (pod001_bmeDewPointC),
        'pod001_bmeDewPointF': (pod001_bmeDewPointF),
        'pod001_bmePressurehPa': (pod001_bmePressurehPa),
        'pod001_bmeApproxAltitudeM': (pod001_bmeApproxAltitudeM),
        'pod001_bmeApproxAltitudeF': (pod001_bmeApproxAltitudeF),
        'pod001_tslLUX': (pod001_tslLUX),

        }
    }
]


json_body_002 = [
    {
        "measurement": "sensor_arrays",
        "tags": {
            "unit": "pod002",
            "location": "breadboard002"
        },
        "time": (reading),
        "fields": {

        'pod002_bmeTempF': (pod002_bmeTempF),
        'pod002_bmeTempC': (pod002_bmeTempC),
        'pod002_bmeHumidity': (pod002_bmeHumidity),
        'pod002_bmeDewPointC': (pod002_bmeDewPointC),
        'pod002_bmeDewPointF': (pod002_bmeDewPointF),
        'pod002_bmePressurehPa': (pod002_bmePressurehPa),
        'pod002_bmeApproxAltitudeM': (pod002_bmeApproxAltitudeM),
        'pod002_bmeApproxAltitudeF': (pod002_bmeApproxAltitudeF),
        'pod002_tslLUX': (pod002_tslLUX),
        'pod002_mplInchesHg': (pod002_mplInchesHg),
        'pod002_mplAltitudeMeters': (pod002_mplAltitudeMeters),
        'pod002_mplAltitudeFeet': (pod002_mplAltitudeFeet),
        'pod002_mplTempC': (pod002_mplTempC),
        'pod002_mplTempF': (pod002_mplTempF),
        }
    }
]

json_body_003 = [
    {
        "measurement": "sensor_arrays",
        "tags": {
            "unit": "uvpod001",
            "location": "breadboard003"
        },
        "time": (reading),
        "fields": {

        'uvpod001_tslms': (uvpod001_tslms),
        'uvpod001_tslIR': (uvpod001_tslIR),
        'uvpod001_tslFull': (uvpod001_tslFull),
        'uvpod001_tslVisible': (uvpod001_tslVisible),
        'uvpod001_tslLUX': (uvpod001_tslLUX),
        'uvpod001_siVis': (uvpod001_siVis),
        'uvpod001_siIR': (uvpod001_siIR),
        'uvpod001_siUVindex': (uvpod001_siUVindex),
        'uvpod001_vmlUV': (uvpod001_vmlUV),
        'uvpod001_mcpTempF': (uvpod001_mcpTempF),
        'uvpod001_mcpTempC': (uvpod001_mcpTempC),
        }
    }
]

json_body_004 = [
    {
        "measurement": "sensor_arrays",
        "tags": {
            "unit": "pod003",
            "location": "breadboard001"
        },
        "time": (reading),
        "fields": {

        'pod003_bmeTempF': (pod003_bmeTempF),
        'pod003_bmeTempC': (pod003_bmeTempC),
        'pod003_bmeHumidity': (pod003_bmeHumidity),
        'pod003_bmeDewPointC': (pod003_bmeDewPointC),
        'pod003_bmeDewPointF': (pod003_bmeDewPointF),
        'pod003_bmePressurehPa': (pod003_bmePressurehPa),
        'pod003_bmeApproxAltitudeM': (pod003_bmeApproxAltitudeM),
        'pod003_bmeApproxAltitudeF': (pod003_bmeApproxAltitudeF),
        'pod003_bmeGasResistanceKOhms': (pod003_bmeGasResistanceKOhms),
        'pod003_tslLUX': (pod003_tslLUX),
        'pod003_mplInchesHg': (pod003_mplInchesHg),
        'pod003_mplAltitudeMeters': (pod003_mplAltitudeMeters),
        'pod003_mplAltitudeFeet': (pod003_mplAltitudeFeet),
        'pod003_mplTempC': (pod003_mplTempC),
        'pod003_mplTempF': (pod003_mplTempF),
        }
    }
]



client = InfluxDBClient('localhost', 8086, 'root', 'root', 'sensor_arrays')

# client.create_database('pod001')

client.write_points(json_body_001)
client.write_points(json_body_002)
client.write_points(json_body_003)
client.write_points(json_body_004)

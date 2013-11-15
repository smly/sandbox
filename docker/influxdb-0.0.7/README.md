### Build InfluxDB container

````
$ git clone https://gist.github.com/7334560.git
$ cd 7334560
$ docker build .
[snip]
Successfully built bc16b7ab3dbd

$ docker run bc16b7ab3dbd
2013/11/06 13:46:49 Loading Config from config.json.sample
2013/11/06 13:46:49 Starting Influx Server...
2013/11/06 13:46:49 Opening database at  /tmp/influxdb/development/db
2013/11/06 13:46:49 Initializing Raft HTTP server
2013/11/06 13:46:49 Listening at: http://localhost:8090
2013/11/06 13:46:49 Initializing Raft Server: /tmp/influxdb/development/raft 8090
2013/11/06 13:46:49 Couldn't contact a leader so initializing new cluster for server on port:  8090
2013/11/06 13:46:50
2013/11/06 13:46:50 Starting admin interface on port 8083
2013/11/06 13:46:50 Starting Http Api server on port 8086
````

### Using InfluxDB's HTTP API with Python-client

````
$ git clone http://github.com/influxdb/influxdb-python && cd influxdb-python
$ git checkout v0.1.1

$ python --version
Python 2.7.3
$ virtualenv venv
$ source venv/bin/activate

$ python setup.py install
$ python examples/tutorial.py --host=172.17.0.57 --port=8086
Create database: example
Get list of database users: []
Add database user: smly
Get list of database users again: [{u'username': u'smly'}]
Swtich user: smly
Write points: [{'points': [['1', 1, 1.0], ['2', 2, 2.0]], 'name': 'foo', 'columns': ['column_one', 'column_two', 'column_three']}]
Queying data: select column_one from foo;
Result: [{"name":"foo","columns":["time","sequence_number","column_one"],"points":[[1383886725,2,"2"],[1383886725,1,"1"]]}]
Swtich user: root
Delete database: example
````

### Using InfluxDB's HTTP API with python-requests

````
$ virtualenv venv
$ source venv/bin/activate
$ pip install requests
$ python
Python 2.7.5+ (default, Sep 19 2013, 13:48:49)
[GCC 4.8.1] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import requests
>>> import json
>>>
>>> data = [
...   {
...     "name": "events",
...     "columns": ["state", "email", "type"],
...     "points": [
...       ["ny", "paul@influxdb.org", "follow"],
...       ["ny", "todd@influxdb.org", "open"]
...     ]
...   },
...   {
...     "name": "errors",
...     "columns": ["class", "file", "user", "severity"],
...     "points": [
...       ["DivideByZero", "example.py", "someguy@influxdb.org", "fatal"]
...     ]
...   }
... ]
>>> url = 'http://172.17.0.47:8086'
>>> headers={'Content-type': 'application/json', 'Accept': 'text/plain'}

>>> ########
>>> ### Create database
>>> resp = requests.post(url + '/db?u=root&p=root', data=json.dumps({'name': 'development'}), headers=headers)
>>> resp
<Response [201]>
>>> resp.content
''

>>> ########
>>> ### Get list of database users
>>> resp = requests.get(url + '/db/development/users?u=root&p=root')
>>> resp
<Response [200]>
>>> resp.content
'[]'

>>> ########
>>> ### Add database user ('user' is not expected, but this request is accepted...)
>>> resp = requests.post(url + '/db/development/users?u=root&p=root', data=json.dumps({'user': 'smly', 'password': 'standardml'}), headers=headers)
>>> resp
<Response [200]>
>>> resp.content

>>> ########
>>> ### Get list of database users. wow, a ghost user '' is listed.
>>> ### (Resolved after https://github.com/influxdb/influxdb/commit/d7628935a)
>>> resp = requests.get(url + '/db/development/users?u=root&p=root')
>>> resp.content
'[{"username":""}]'

>>> ########
>>> ### Add database user again. 'username' is expected.
>>> resp = requests.post(url + '/db/development/users?u=root&p=root', data=json.dumps({'username': 'smly', 'password': 'standardml'}), headers=headers)

>>> resp = requests.get(url + '/db/development/users?u=root&p=root')
>>> resp.content
'[{"username":""},{"username":"smly"}]'

>>> ### Can't delete the ghost user...
>>> ### (Resolved after https://github.com/influxdb/influxdb/commit/d7628935a)
>>> resp = requests.delete(url + '/db/development/users/?u=root&p=root')
>>> resp
<Response [404]>
>>> resp.content
'404 page not found\n'

>>> ### Add data points.
>>> resp = requests.post(url + '/db/development/series?u=smly&p=standardml', data=json.dumps(data), headers=headers)
>>> resp
<Response [200]>
>>> resp.content
''

````

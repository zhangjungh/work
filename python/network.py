#!/usr/bin/python3
# -*- coding: utf-8-*-

import http.client

try:
	import json
except ImportError:
	import simplejson as json
	
path = ('/maps/geo?q=207+N.+Defiance+St%2C+Archbold%2C+OH' 
		'&output=json&oe=utf8')
		
#connection = http.client.HTTPConnection('maps.google.com')
#connection.request('GET', path) 
#rawreply = connection.getresponse().read() 

#print(rawreply)
#reply = json.loads(rawreply) 
print(path)
#print reply['Placemark'][0]['Point']['coordinates'][:-1] 

import socket
hostname = 'www.google.com'
addr = socket.gethostbyname(hostname)
print(hostname, addr)
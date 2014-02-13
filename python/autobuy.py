#!/usr/bin/python3
# -*- coding: utf-8 -*-

import httplib2
import urllib.parse
import os
import sys
import re

baidu = 'http://www.baidu.com'
taobao = 'https://login.taobao.com/member/login.jhtml'

charset = re.compile(r'.*?charset=([a-zA-Z0-9-]+)', re.I)

#httplib2.debuglevel = 1

headers = {
	'User-Agent': 'Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)',
	'Accept': 'text/html, application/xhtml+xml, */*',
	'Accept-Encoding': 'gzip, deflate',
	'Accept-Language': 'zh-CN',
	'Cache-Control': 'no-cache',
	'Connection': 'keep-alive'
}

def login():
	pass

import random	
def rand5():
	return random.randint(1, 5);

def rand7():
	sum = rand5() + rand5() // 2
		
	return sum
	
def rand7_s():
	a = [rand5(), rand5()+1, rand5()+2, rand5(), rand5()+1]
	return a[rand5()-1]
	
if __name__ == '__main__':
	#h = httplib2.Http('.cache')
	#get cookie
	#r, c = h.request(baidu, headers=headers)
	#cs = charset.search(r['content-type']).group(1)
	#print(cs)
	#print(c.decode(cs))
	
	
	#post
	#h.add_credentials('name', 'password')

	
	#refresh
	#resp, content = h.request("http://bitworking.org/", 
    #headers={'cache-control':'no-cache'})
	
	#buy
	r = [0, 0, 0, 0, 0]
	for i in range(100000):
		r[rand5()-1] += 1
	
	r = [i/100000 for i in r]
	print(r)
	
	r = [0, 0, 0, 0, 0, 0, 0]
	for i in range(100000):
		r[rand7()-1] += 1
	
	r = [i/100000 for i in r]	
	print(r)

	r = [0, 0, 0, 0, 0, 0, 0]
	for i in range(100000):
		r[rand7_s()-1] += 1
	
	r = [i/100000 for i in r]	
	print(r)	
	#image
	
	
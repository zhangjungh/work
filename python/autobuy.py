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

if __name__ == '__main__':
	h = httplib2.Http('.cache')
	#get cookie
	r, c = h.request(baidu, headers=headers)
	cs = charset.search(r['content-type']).group(1)
	print(cs)
	#print(c.decode(cs))
	
	
	#post
	h.add_credentials('name', 'password')

	
	#refresh
	resp, content = h.request("http://bitworking.org/", 
    headers={'cache-control':'no-cache'})
	
	#buy
	
	
	#image
	
	
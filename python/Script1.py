print('hello world')
msg = r'http://jandan.net/1999/10/13/ipad-i-beg.html'

import re
import time

from datetime import date

print(abs(date(1981, 4, 12)-date.today()).days)

result = msg.split('/')

a = ['hello', 3, False]
result[0] = a

b = {'shen':20, 'zhang':30, 30:'g'}

import os
import subprocess
#subprocess.Popen(['notepad.exe'])

def resample():
	strlist = ["foo.bar", "autoexec.bat", "sendmail.cf", "printers.conf", "sample.batch"]
	strp = re.compile(r'.*[.](?!bat$).*$')

	for i in strlist:
		if strp.match(i):
			print i
		else:
			print 'not match', i
			
	test = r'ASDFabc10<abc10<abc1323<abc124<abc1284<'
	tp = re.compile(r'ASDF(abc(\d+)<){5}')
	tg = tp.findall(test)
	print tg
	
resample()

def fin():
	value = 1.06
	spent = 0;
	total = 0;
	for i in range(10):
		base = 20000
		spent += base
		for j in range(10-i):
			base *= value
		print(base)
		total += base;
	print(spent, total)
	
def htmlparser():
	first = []
	lefttop = re.compile(r'left:(\d+)px; top:(\d+)px; width:(\d+)px;.*?>(.*?)(?:$|<div)')
	fontsize = re.compile(r'font-size:(\d+)px">(.*?)(?:$|<)')
	all = re.compile(r'left:(\d+)px; top:(\d+)px; width:(\d+)px;.*?>(.*?font-size:(\d+)px">(.*?)(?:$|<))+(?:$|<div)')
	with open(r'd:\Python27\Scripts\first.html', 'rb') as f:
		for line in f:
			dg = lefttop.findall(line)
			for d in dg:
				mg = fontsize.findall(d[3])
				tlen = sum([len(m[1]) for m in mg]) if len(mg) > 1 else -1
				tcur = 0
				for m in mg:
					t = m[1].strip()
					if t != '':
						left = int(d[0])
						if tlen != -1: 
							#print mg
							left = int(float(d[2])*tcur/tlen) + int(d[0])
							tcur += len(m[1])
						first.append( (t, int(m[0]), left, int(d[1])) )	
			group = all.findall(line)
			if group: print group[0][5]
						
	for l in first: print(l[0].decode('utf-8', 'ignore'))
	
htmlparser()
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
#!/usr/bin/python2
# -*- coding: utf-8-*-

#two global constants
FILENAME = r'final_dicEntry_yourLogin.txt'
MENUMSG = '''
************MENU************ 
1. Add to Dict 
2. Delete by Key
3. Get Value by Key 
4. Sort by Key and Print 
5. Sort by Value and Print 
6. Print Dict
7. Save Dict 
8. Quit
'''

def read_dict(filename, d, c): #no value return
	with open(filename, 'rt') as f:
		print('dict read from %s' % filename)
		for line in f: #for loop
			k, v = line.split(',')#string function 1
			add_dict(k, v.rstrip('\n'), d, c) #string function 2
			
def write_dict(**kwlist): #keyword parameters
	fstr = 'filename'
	dstr = 'dict'
	if fstr in kwlist:
		filename = kwlist[fstr]
		if dstr in kwlist:
			d = kwlist[dstr]
			with open(filename, 'wt') as f:
				print('dict saved to %s' % filename)
				for i in d:
					for j in i:
						f.write('%s,%s\n' % (j[0], j[1]))	

def init_dict(c = 100): 
	return [[] for i in range(c)], c #function return two values

#use hashtable to build my own dictionary
def hash_func(k, c):
	h = 0
	key = str(k) #convert anytype to string
	for i in key:
		h = 31*h+ord(i)
	return h % c
	
def add_dict(k, v, d, c, p=False): #default parameters
	idx = hash_func(k, c)
	for i in d[idx]:
		if i[0] == k: #update
			i[1] = v
			if p: print('updated [%s, %s]' % (k, v))
			return
	#new key
	d[idx].append( [k, v] )
	if p: print('add new [%s, %s]' % (k, v))
	
def get_value(k, d, c):
	idx = hash_func(k, c)
	for i in d[idx]:
		if i[0] == k:
			print('value is %s' % i[1])
			return
	print('%s not found' % k)
	
def del_dict(k, d, c):
	idx = hash_func(k, c)
	for i in d[idx]:
		if i[0] == k:
			print('del [%s, %s]' % (i[0], i[1]))
			d[idx].remove(i)
			return
	print('%s not found' % k)

def print_slot(l):
	for i in l:
		print('key: %s, value: %s' % (i[0], i[1]))
		
def print_dict(d):
	map(print_slot, d) #map function
			
def sort_print(d, *other): #variable length and positional parameter
	idx = 0
	if len(other) != 0:
		idx = other[0]
		
	if idx == 0:
		print('sorted by key:')
	elif idx == 1:
		print('sorted by vaule:')
	else:
		return
		
	l = []
	for i in d:
		for j in i:
			l.append(j)
	
	for j in sorted(l, key=lambda v : v[idx]): #lambda func
		print('key: %s, value: %s' % (j[0], j[1]))
	
	
def input_dict(string):
	while True:
		try:
			data = input('Please input %s: ' % string)
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))	

	
def select_input():
	while True:
		try:
			data = input('Please make a selection: ')			
			#check if it is valid
			if type(data) != type(1) or data < 1 or data > 8:
				raise Exception, 'Not an valid selection between [1, 8]'
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))
					
if __name__ == '__main__':
	d = [] #list
	c = 0
	while True: #while loop
		try:
			if c == 0:
				d, c = init_dict()
				read_dict(FILENAME, d, c)
				
			print(MENUMSG)
			
			s = select_input()
			
			if s == 1: #if else statement
				k = input_dict('key')
				v = input_dict('value')
				add_dict(k, v, d, c, True)
			elif s == 2:
				k = input_dict('key')
				del_dict(k, d, c)
			elif s == 3:
				k = input_dict('key')
				v = get_value(k, d, c)
			elif s == 4:
				sort_print(d, 0, 'unused', 2, 3)
			elif s == 5:
				sort_print(d, 1)
			elif s == 6:
				print_dict(d)
			elif s == 7:
				write_dict(filename=FILENAME, dict=d)
			elif s == 8:
				print('Good Bye!')
				break
			else:
				assert(0)			
			
		except KeyboardInterrupt, e:
			print('Program ends by User')
			break
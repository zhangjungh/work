#!/usr/bin/python2
# -*- coding: utf-8-*-

def dataInput(string):
	loop = True
	while loop:
		try:
			data = input('Enter ' + string + ' number or float: ')			
			#check if it is valid
			if type(data) != type(1) and type(data) != type(1.1):
				raise Exception, 'Not an Integar or Float'
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))	

def opInput():
	loop = True
	while loop:
		try:
			op = input('Enter operator: ')
			#check if it is valid operator
			if op not in ['+', '-', '*', '/']:
				raise Exception, "Not an valid operator: '+', '-', '*', '/'"
			return op
		except Exception, e:
			print('Invalid input: ' + str(e))	
			
if __name__ == '__main__':
	loop = True
	while loop:
		try:
			#get inputs
			first  = dataInput('first')
			second = dataInput('second')
			op	   = opInput()
			
			#calculate:
			result = 0
			if op == '+':
				result = first + second
			elif op == '-':
				result = first - second
			elif op == '*':
				result = first * second
			elif op == '/':
				result = float(first) / second	
				
			print(str(first) + ' ' + op + ' ' + str(second) + ' is: ' + str(result))

		except KeyboardInterrupt, e:
			print('Program ends by User')
			loop = False
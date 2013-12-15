#!/usr/bin/python2
# -*- coding: utf-8-*-

#import re
#yearformat = re.compile(r'^\d{4}$')

season = ['Spring', 'Summer', 'Fall', 'Winter']
gradesym = '+ -'
grade = 'ABCDF'
gradebook = [('Math 101', 'calculus', 'Fall 2012', 'C'), 
			 ('CS 130', 'Python', 'Spring 2013', 'A'),
			 ('CS 132', 'C++', 'Summer 2013', 'A+'),
			 ('Chem 120', 'OChem', 'Winter 2012', 'A-'),
			 ('Phy 125', 'QCD', 'Spring 2013', 'A-')] 
			 
def printMenu():
	print(
'''************MENU************ 
1. Add class 
2. Drop class 
3. Sort gradebook by class name 
4. Sort gradebook in order of the semester 
5. Sort gradebook in order of the final grades 
6. Print gradebook 
7. Quit
''')

def printGradeBook(b):
	if len(b) == 0:
		print('Your gradebook is empty\n')
		return
		
	print('Your gradebook...')
	count = 1
	for i in b:
		assert(len(i) == 4)
		print('%d. %s...Subject: %s...Semester: %s...Grade: %s' % (count, i[0], i[1], i[2], i[3]))
		count += 1
	print
	
def selectInput():
	while True:
		try:
			data = input('Please make a selection: ')			
			#check if it is valid
			if type(data) != type(1) or data < 1 or data > 7:
				raise Exception, 'Not an valid selection between [1, 7]'
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))	

def dropInput():
	while True:
		try:
			data = input('Please input drop index between [1, %d]: ' % len(gradebook))
			#check if it is valid
			if type(data) != type(1) or data < 1 or data > len(gradebook):
				raise Exception, 'Not an valid selection between [1, %d]' % len(gradebook)
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))

def classInput():
	while True:
		try:
			data = input('Give class name: ')			
			#check if it is valid
			if type(data) != type('string'):
				raise Exception, 'Not string type'
			l = data.split()
			if len(l) != 2:
				raise Exception, 'Not fit the format [classname ID]'
			#elif l[0] not in classes:
			#	raise Exception, 'Not an valid class: ' + l[0]
			elif int(l[1]) < 100 or int(l[1]) > 999: 
				raise Exception, 'ID Not in range [100, 999]: ' + l[1]				
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))
	
def subjectInput():
	while True:
		try:
			data = input('Give subject: ')			
			#check if it is valid
			if type(data) != type('string'):
				raise Exception, 'Not string type'
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))

def semesterInput():
	while True:
		try:
			data = input('Give semester: ')			
			#check if it is valid
			if type(data) != type('string'):
				raise Exception, 'Not string type'
			l = data.split()
			if len(l) != 2:
				raise Exception, 'Not fit the format [season year]'
			elif l[0] not in season:
				raise Exception, 'Not an valid season: ' + l[0]
			elif int(l[1]) < 2000 or int(l[1]) > 2020: #re.match(yearformat, l[1]) == None: 
				raise Exception, 'Year Not in range [2000, 2020]: ' + l[1]			
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))
			
def gradeInput():
	while True:
		try:
			data = input('Give grade: ')			
			#check if it is valid
			if type(data) != type('string'):
				raise Exception, 'Not string type'
			elif len(data) == 0 or len(data) > 2:
				raise Exception, 'Not an valid grade: ' + data
			elif data[0] not in grade:
				raise Exception, 'Not an valid grade: ' + data
			elif len(data) == 2 and data[1] not in gradesym:
				raise Exception, 'Not an valid grade: ' + data
			return data
		except Exception, e:
			print('Invalid input: ' + str(e))
			
def semester_cmp(x, y):
	a1, a2 = x[2].split()
	b1, b2 = y[2].split()
	if a2 == b2:
		return season.index(a1) - season.index(b1)
	else:
		return int(a2) - int(b2)
		
def grade_cmp(x, y):
	a, b = x[3], y[3]
	if a[0] == b[0]:
		s1 = gradesym.index(a[1]) if len(a) == 2 else 1
		s2 = gradesym.index(b[1]) if len(b) == 2 else 1
		return s1 - s2
	else:
		return grade.index(a[0]) - grade.index(b[0])
		
if __name__ == '__main__':
	while True:
		try:
			printMenu()
			
			s = selectInput()
			
			if s == 1:
				cn = classInput()
				sn = subjectInput()
				sm = semesterInput()
				gd = gradeInput()
				gradebook.append( (cn, sn, sm, gd) )
			elif s == 2:
				if len(gradebook) == 0:
					print('Your gradebook is empty, no class to drop\n')
				else:
					del gradebook[dropInput() - 1]
			elif s == 3:
				gradebook.sort(key=lambda book : book[0].lower())
			elif s == 4:
				gradebook.sort(cmp=semester_cmp)
			elif s == 5:
				gradebook.sort(cmp=grade_cmp)
			elif s == 6:
				printGradeBook(gradebook)
			elif s == 7:
				print('Good Bye!')
				break
			else:
				assert(0)
		except KeyboardInterrupt, e:
			print('Program ends by User')
			break
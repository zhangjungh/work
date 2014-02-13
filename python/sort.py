#sort algorithm in python

import os
import sys
import random

#g_array = [1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 9, 10]
g_array = [8, 1, 5, 10, 13, 15, 4, 7, 2, 9, 5]

####################################################
def bubble_sort(arr):
	for i in range(1, len(arr)):
		for j in range(len(arr)-1, i-1, -1):
			#print(i, j)
			if (arr[j] < arr[j-1]):
				arr[j], arr[j-1] = arr[j-1], arr[j]
			print(arr)
		print('------------------------------------')

####################################################
def insert_sort(arr):
	for i in range(1, len(arr)):
		key = arr[i]
		j = i-1
		while j>=0 and arr[j]>key:
			arr[j+1] = arr[j]
			j = j-1
			#print(arr)
		arr[j+1] = key
		print(arr)
		print('------------------------------------')
				
####################################################	
def merge_sort(arr, p, r):
	if p < r:
		q = (p+r) // 2
		print(arr[p:q+1])
		merge_sort(arr, p, q)
		print(arr[q+1:r+1])
		merge_sort(arr, q+1, r)
		merge(arr, p, q, r)

def merge(arr, p, q, r):
	ai = arr[p:q+1]
	aj = arr[q+1:r+1]
	mi, mj = ai[-1]+1, aj[-1]+1
	ai.append(mj)
	aj.append(mi)
	i, j = 0, 0
	for k in range(p, r+1):
		if aj[j] >= ai[i]:
			arr[k] = ai[i]
			i += 1
		else:
			arr[k] = aj[j]
			j += 1
	print(arr[p:r+1])
	
####################################################
def heap_sort(arr):
	build_max_heap(arr)
	print(arr)
	length = len(arr)
	for i in range(len(arr)-1, 0, -1):
		arr[0], arr[i] = arr[i], arr[0]
		length = length-1
		heapify(arr, 0, length)
	print(arr)
	

def build_max_heap(arr):
	for i in range((len(arr)-1)//2, -1, -1):
		heapify(arr, i, len(arr))

def heapify(arr, n, length):
	l, r = 2*n, 2*n+1
	min = n
	if l < length and arr[l] < arr[min]:
		min = l
	if r < length and arr[r] < arr[min]:
		min = r
	if min != n:
		arr[n], arr[min] = arr[min], arr[n]
		heapify(arr, min, length)

####################################################
def quick_sort(arr, p, r):
	if p < r:
		q = portion(arr, p, r)
		print(q, arr[q])
		quick_sort(arr, p, q-1)
		quick_sort(arr, q+1, r)
	#print(arr)

def portion(arr, p, r):
	x = arr[r]
	i = p-1
	for j in range(p, r):
		if arr[j] <= x:
			i = i+1
			arr[i], arr[j] = arr[j], arr[i]
	arr[i+1], arr[r] = arr[r], arr[i+1]
	print('protion', arr[p:r+1])
	print('full', arr)
	return i+1

####################################################	
def fib(n):
	if n==0 or n==1: return n
	f0, f1 = 0, 1
	for i in range(1, n):
		t = f0 + f1
		f1, f0 = t, f1
	return f1
	
if __name__ == '__main__':
	random.shuffle(g_array)
	print(g_array)
	print('*************************************')
	#bubble_sort(g_array)
	#insert_sort(g_array)
	#merge_sort(g_array, 0, len(g_array)-1)
	#heap_sort(g_array)
	quick_sort(g_array, 0, len(g_array)-1)
	print(g_array)
	#print(fib(100))
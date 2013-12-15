import sys
import os

g_data1 = [ [0, 0, 7, 1, 3, 0, 0, 0, 6],
		   [0, 5, 0, 0, 9, 0, 3, 0, 0],
		   [6, 0, 0, 0, 0, 2, 1, 0, 0],
		   [0, 6, 0, 0, 0, 3, 0, 2, 1],
		   [8, 0, 0, 0, 4, 0, 0, 0, 9],
		   [5, 2, 0, 0, 0, 0, 0, 8, 0],
		   [0, 0, 6, 3, 0, 0, 0, 0, 2],
		   [0, 0, 9, 0, 6, 0, 0, 3, 0],
		   [3, 0, 0, 0, 2, 9, 6, 0, 0] ]

g_data = [ [0, 0, 9, 3, 0, 0, 0, 8, 0],
		   [0, 8, 0, 6, 0, 0, 2, 0, 0],
		   [1, 0, 0, 5, 4, 0, 0, 0, 9],
		   [0, 5, 0, 2, 0, 0, 0, 0, 0],
		   [0, 3, 0, 4, 0, 0, 0, 6, 0],
		   [0, 0, 0, 0, 0, 0, 0, 2, 0],
		   [5, 0, 0, 0, 9, 6, 0, 0, 1],
		   [0, 0, 8, 0, 0, 2, 0, 9, 0],
		   [0, 9, 0, 0, 0, 4, 7, 0, 0] ]

def isValid(value, col, row, block):
	if value in col or value in row or value in block:
		return False
	return True
	
def resolveRow(n, rows, cols, blocks):
	row = rows[n]
	for i in range(9): #len(row)
		if row[i] == 0:
			val = []
			for v in range(1, 10):
				if v not in row and v not in cols[i]: #not in row and col
					blk = blocks[3*(n//3) + i//3]
					if v not in blk: #not in block
						val.append(v)
						if (len(val) > 1):
							break
			if len(val) == 1: #only one possible value
				return [val[0], n, i, 'p']
	for v in range(1, 10):
		if v not in row:
			pos = []
			for i in range(9):
				if row[i] == 0:
					blk = blocks[3*(n//3) + i//3]
					if v not in cols[i] and v not in blk:
						pos.append([n, i])
						if (len(pos) > 1):
							break
			if len(pos) == 1:
				return [v, pos[0][0], pos[0][1], 'v']
	return [0, 0, 0]
	
def resolveCol(n, rows, cols, blocks):
	col = cols[n]
	for i in range(9):
		if col[i] == 0:
			val = []
			for v in range(1, 10):
				if v not in col and v not in rows[i]: #not in row and col
					blk = blocks[3*(i//3) + n//3]
					if v not in blk: #not in block
						val.append(v)
						if (len(val) > 1):
							break
			if len(val) == 1: #only one possible value
				return [val[0], i, n, 'p']
	for v in range(1, 10):
		if v not in col:
			pos = []
			for i in range(9):
				if col[i] == 0:
					blk = blocks[3*(i//3) + n//3]
					if v not in rows[i] and v not in blk:
						pos.append([i, n])
						if (len(pos) > 1):
							break
			if len(val) == 1:
				return [v, pos[0][0], pos[0][1], 'v']
	return [0, 0, 0]
	
def resolveBlock(n, rows, cols, blocks):
	blk = blocks[n]
	for i in range(9):
		if blk[i] == 0:
			val = []
			p = 3*(n//3)+i//3 
			q = 3*(n%3)+i%3
			for v in range(1, 10):
				if v not in blk: #not in block
					if v not in rows[p] and v not in cols[q]: #not in row and col
						val.append(v)
						#print('block', [v, p, q])
						if (len(val) > 1):
							break
			if len(val) == 1: #only one possible value
				return [val[0], p, q, 'p']
	for v in range(1, 10):
		if v not in blk:
			pos = []
			p, q = 0, 0
			for i in range(9):
				if blk[i] == 0:
					p = 3*(n//3)+i//3
					q = 3*(n%3)+i%3
					if v not in rows[p] and v not in cols[q]:
						pos.append([p, q])
						if (len(pos) > 1):
							break
			if len(pos) == 1: #only one possible position
				return [v, pos[0][0], pos[0][1], 'v']
	return [0, 0, 0]
	
def onePass(data):
	#row first
	c = [ [data[i][j] for i in range(9)] for j in range(9) ]
	b = [ [data[i+x*3][j+y*3] for i in range(3) for j in range(3)] for x in range(3) for y in range(3)]
	for i in range(9): #row
		r =  resolveRow(i, data, c, b)
		if r[0] != 0: #have a match and change the data immediately
			data[r[1]][r[2]] = r[0]
			print('row', r)
			return True
	#col second
	c = [ [data[i][j] for i in range(9)] for j in range(9) ]
	b = [ [data[i+x*3][j+y*3] for i in range(3) for j in range(3)] for x in range(3) for y in range(3)]
	for i in range(9): #row
		r =  resolveCol(i, data, c, b)
		if r[0] != 0: #have a match and change the data immediately
			data[r[1]][r[2]] = r[0]
			print('col', r)
			return True
	#block...
	c = [ [data[i][j] for i in range(9)] for j in range(9) ]
	b = [ [data[i+x*3][j+y*3] for i in range(3) for j in range(3)] for x in range(3) for y in range(3)]
	for i in range(9): #row
		r =  resolveBlock(i, data, c, b)
		if r[0] != 0: #have a match and change the data immediately
			data[r[1]][r[2]] = r[0]
			print('blk', r)
			return True			
	return False
	
if __name__ == '__main__':
	while(onePass(g_data)):
		pass
	for i in g_data: print(i)
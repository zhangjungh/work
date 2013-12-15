# -*- coding: GB2312 -*-
import random

'''
generate randome tetris algorithm:
in a 7*7 squared place, the first element of tetris in the center 3*3
thus left, right, up, down four cloest positions are the potential positions for second one
choose a random one from them and add more potential positions. then generate third and fourth element
rotate ninety degree is quite simple:
(x, y) * (0, -1) = y, -x
		 (1,  0)
'''

class Tetris():
	def __init__(self):
		self.reset()
	def reset(self):
		self.t = [ [-1, -1] for i in range(4) ] #represent the tetris
		self.n = [ [-1, -1] for i in range(4) ]	#represent next coming tetris
		self.b = [ [-1, -1] for i in range(3) ] #bounding box and center point
		self.m = [ 10 * [0] for i in range(15)] #15 * 10, the main window
		self.d = False
		self.f = False
		self.s = 0 								#1-100, 2-300 3-500, 4-700
	def _show(self):
		s = [ 7 * [0]  for i in range(7) ]
		for i in self.n: s[i[0]][i[1]] = 1
		#for i in s: print(i)
		for i in s:
			for j in i: print('*' if j==1 else '`', end='')
			print()
	def randomTetris(self):
		if self.n[0][0] == -1:
			for i in range(4): self._addPotential(self.n)
			self.n.sort(key=lambda i:i[0])
		if self.n[0][0] != -1:
			self.t = self.n
			self.n = [ [-1, -1] for i in range(4) ] #init
			for i in range(4): self._addPotential(self.n)
			self.n.sort(key=lambda i:i[0])
		self._centralize()
		self._zerolize()
		self.f = False
	def _addPotential(self, t):
		c, p = 0, []
		for i in t:
			if i[0] != -1: #valid point
				pt = [[ i[0]-1, i[1] ], [ i[0]+1, i[1] ], [ i[0], i[1]-1 ], [ i[0], i[1]+1 ]]
				for j in pt:
					if j not in p and j not in t: p.append(j)
				c += 1
		#print(p)
		if c == 0: t[0] = [3, 3]
		else: t[c] = random.choice(p)
	def _centralize(self):
		y = sorted(self.t, key=lambda i:i[1])
		self.b[0], self.b[1] = [self.t[0][0], y[0][1]], [self.t[3][0], y[3][1]]
		self.b[2] = [(self.b[0][0]+self.b[1][0])//2, (self.b[0][1]+self.b[1][1])//2]
		self.t, self.b = self._offset([4-self.b[2][0], -1-self.b[1][1]])
	def _zerolize(self):
		y = sorted(self.n, key=lambda i:i[1])
		self.n = [[i[0]-self.n[0][0], i[1]-y[0][1]] for i in self.n]
	def _offset(self, p):
		t = [[i[0]+p[0], i[1]+p[1]] for i in self.t]
		b = [[i[0]+p[0], i[1]+p[1]] for i in self.b]
		return t, b
	def rotateNinety(self):
		w = self.t + self.b
		w = [[i[1], -i[0]] for i in w]
		w = [[i[0]+self.b[2][0]-w[6][0], i[1]+self.b[2][1]-w[6][1]] for i in w]
		l, r = w[4][0], w[5][0]
		if l > r: l, r = r, l
		if l < 0: w = [[i[0]-l, i[1]] for i in w]
		elif r > 9: w = [[i[0]-r+9, i[1]] for i in w]
		return self._isPassCollision(w[:4], w[4:])
	def moveDown(self):
		if self.f: return False
		t, b = self._offset([0, 1])
		if not self._isPassCollision(t, b): 
			self._fixed(); 
			return False
		else: return True
	def moveLeft(self):
		if self.f: return False
		t, b = self._offset([-1, 0])
		return self._isPassCollision(t, b)
	def moveRight(self):
		if self.f: return False
		t, b = self._offset([1, 0])
		return self._isPassCollision(t, b)
	def _fixed(self):
		self.f = True
		for i in self.t:
			if i[1] < 0: self.d = True
			else: self.m[i[1]][i[0]] = 1
		self._removeLine() #after remove line, maybe valid again by dropping
	def _isPassCollision(self, t, b):
		for i in t:
			if i[0] < 0 or i[0] >= 10 or i[1] >= 15: return False #left, right, down
			elif i[1] >= 0:
				if self.m[i[1]][i[0]] == 1: return False
		self.t, self.b = t, b#new value
		return True
	def _removeLine(self):
		c, p = 0, []
		for i in self.m:
			if 0 in i: p.append(i)
			else: c += 1
		for i in range(c): p.insert(0, 10*[0])
		self.m = p
		self.t = [ [-1, -1] for i in range(4) ]
		if c != 0: self.s += (2*c-1) * 100
			
if __name__ == '__main__':
	t = Tetris()
	t.randomTetris()
	t._show()
	t.rotateNinety()
	t._show()
#!/usr/bin/python3
# -*- coding: utf-8-*-

import os, sys
from random import randint
from PIL import Image

print(sys.version)

def createImage(count):
	sz = 128
	im = Image.new('RGB', (sz, sz))
	for i in range(sz):
		for j in range(sz):
			im.putpixel((i, j), (randint(0, 255), randint(0, 255), randint(0, 255)))
	im.save('rand_%.2d.png' % count)


	c0, c1, c2 = randint(0, 255), randint(0, 255), randint(0, 255)
	for i in range(sz):
		for j in range(sz):
			im.putpixel((i, j), (c0, c1, c2))
	im.save('gray_%.2d.png' % count)
	
for i in range(10):
	createImage(i)
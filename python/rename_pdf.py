﻿# -*- coding: utf-8 -*-
#!D:\Python27\python.exe
import sys
import os
import re
import shutil
from pdfminer.pdfdocument import PDFDocument
from pdfminer.pdfparser import PDFParser
from pdfminer.pdfinterp import PDFResourceManager, PDFPageInterpreter
from pdfminer.pdfdevice import PDFDevice, TagExtractor
from pdfminer.pdfpage import PDFPage
from pdfminer.converter import XMLConverter, HTMLConverter, TextConverter
from pdfminer.cmapdb import CMapDB
from pdfminer.layout import LAParams
from pdfminer.image import ImageWriter

g_count = 0

def main(argv):
	import getopt
	def usage():
		print ('usage: %s [-i input_dir] [-o output_dir]' % argv[0])
	try:
		(opts, args) = getopt.getopt(argv[1:], 'i:o:')
	except getopt.GetoptError:
		return usage()
	if len(argv) == 1 and run_test(): return
	if not opts: return usage()
	
	idir = None
	odir = None
	for (k, v) in opts:
		if k == '-i': idir = v
		elif k == '-o': odir = v
		
	if idir and odir and os.path.exists(idir):
		folder_process(idir, odir)

def folder_process(idir, odir, reserve=False):
	names = []
	if not os.path.exists(odir):
		print('create output dir: %s' % odir)
		os.makedirs(odir)
	for root, dirs, files in os.walk(idir.decode('utf-8')):
		for file in files:
			if file.lower().endswith('.pdf'):
				try:
					path = os.path.join(root, file)
					outfile = pdf_process(path, reserve)
					if len(outfile) > 172: outfile = outfile[:64]+'.pdf'
					names.append(outfile)
					print('copy from %s to %s' % (file.encode('utf-8'), outfile))
					npath = os.path.join(odir, root[len(idir)+1:])
					if not os.path.exists(npath): os.mkdir(npath)
					shutil.copy(path, os.path.join(npath, outfile).decode('utf-8'))
				except Exception, e:
					print('error: %s -> %s' % (file, str(e)))
	return names

def pdf_gettext(filepath, reserve):
	# debug option
	debug = 0
	# input option
	password = ''
	pagenos = set()
	maxpages = 0
	# output option
	outfile = 'output.html'
	outtype = 'html'
	imagewriter = None
	layoutmode = 'normal'
	codec = 'utf-8'
	scale = 1
	caching = True
	laparams = LAParams()
	firstout = 'firstout.html'
	lastout = 'lastout.html'
	firstpage = None
	lastpage = None
	first = []
	last = []
	
	if reserve:
		firstout = filepath[:-3] + firstout
		lastout = filepath[:-3] + lastout
		if os.path.exists(firstout):					
			html_textparser(firstout, first)
			if os.path.exists(lastout):
				html_textparser(lastout, last)
			return first, last
	#
	PDFDocument.debug = debug
	PDFParser.debug = debug
	CMapDB.debug = debug
	PDFResourceManager.debug = debug
	PDFPageInterpreter.debug = debug
	PDFDevice.debug = debug
	#	
	rsrcmgr = PDFResourceManager(caching=caching)
	
	import StringIO
	outfp = StringIO.StringIO()
	#outfp = file(outfile, 'w')
	if outtype == 'text':
		device = TextConverterWithSize(rsrcmgr, outfp, codec=codec, laparams=laparams,
							   imagewriter=imagewriter)
	elif outtype == 'xml':
		device = XMLConverter(rsrcmgr, outfp, codec=codec, laparams=laparams,
							  imagewriter=imagewriter)
	elif outtype == 'html':
		device = HTMLConverter(rsrcmgr, outfp, codec=codec, scale=scale,
							   layoutmode=layoutmode, laparams=laparams,
							   imagewriter=imagewriter, showpageno=False)
	elif outtype == 'tag':
		device = TagExtractor(rsrcmgr, outfp, codec=codec)
	
	interpreter = PDFPageInterpreter(rsrcmgr, device)
	
	with open(filepath, 'rb') as fp:
		for page in PDFPage.get_pages(fp, pagenos,
									maxpages=maxpages, password=password,
									caching=caching, check_extractable=True):
			if not firstpage:
				firstpage = page
			else:
				lastpage = page
		
		if firstpage:
			interpreter.process_page(firstpage)
			with open(firstout, 'w') as f:
				f.write(outfp.getvalue())			
			html_textparser(firstout, first)
		if lastpage:
			outfp.truncate(0)
			interpreter.process_page(lastpage)
			with open(lastout, 'w') as f:
				f.write(outfp.getvalue())
			html_textparser(lastout, last)
		
	device.close()	
	outfp.close()

	return first, last
	
def html_textparser(filename, list):
	div = re.compile(r'<div.*?left:(\d+)px; top:(\d+)px; width:(\d+)px;.*?>(.*?)</div>', re.DOTALL)
	span = re.compile(r'<span.*?font-size:(\d+)px">(.*?)</span>', re.DOTALL)
	lowc = re.compile(r'[a-z]')
	with open(filename, 'rt') as f:
		for d in div.findall(f.read()):
			mg = span.findall(d[3])
			tlen = sum([len(m[1]) for m in mg]) if len(mg) > 1 else -1
			tcur, tcap = 0, False
			for m in mg:
				update = False
				t = m[1].replace('\n<br>', ' ').strip()
				if t != '':
					left = int(d[0])
					if tlen != -1: 
						left = int(float(d[2])*tcur/tlen) + int(d[0])
						tcur += len(m[1])
					if len(t) == 1:	tcap = t.isalpha() and not t.islower()						
					elif tcap:
						update = tcap = lowc.search(t) == None
					if update:
						list[-1] = ( list[-1][0] + t, list[-1][1], list[-1][2], list[-1][3] )
					else:
						list.append( (t, int(m[0]), left, int(d[1])) )
					

def pdf_process(filepath, reserve):
	first, last = pdf_gettext(filepath, reserve)	
	if is_NEJM(first):
		return NEJM_process(first, last)
	elif is_Spine(first):
		return Spine_process(first, last)
	elif is_semss(first):
		return semss_process(first, last)
	elif is_clineuro(first):
		return clineuro_process(first, last)
	elif is_wneu(first):
		return wneu_process(first, last)
	elif is_neurosurgery(first):
		return neurosurgery_process(first, last)
	elif is_lancet(first):
		return lancet_process(first, last)
	elif is_ncna(first):
		return ncna_process(first, last)
	elif is_neurosurg(first):
		return neurosurg_process(first, last)
	elif is_cn(first):
		return cn_process(first, last)
	elif is_nf(first):
		return nf_process(first, last)
	elif is_surg(first):
		return surg_process(first, last)
	else:
		print('treat as general')
		return general_process(first, last)

def text_gettitle(list, h1=0, h2=900, exclude=(), ss=None):
	l = sorted(list, key=lambda l : l[1], reverse=True)
	title = 'no-title'
	for i in l:
		if i[3] >= h1 and i[3] <= h2 and i[0] not in exclude and (not ss or ss not in i[0]):
			w = i[0].split(' ')
			if [c for c in w if len(c) >= 4]:
				title = i[0]
				break
				
	t = None
	for p in l:
		if t != None:
			if t[1] != p[1] or abs(t[3]-p[3]) > 20: break
			if not ss or ss not in p[0]:
				title += ' ' + p[0]
		if not t and title == p[0]:
			t = p
	
	#get valid title:
	title = re.sub(r'[\/:*?"<>|]', '--', title)
	#filter out non-ascii characters
	title = ''.join([i for i in title if ord(i) < 127])
	
	return title
			
def text_getnum(list, default, footer=True, h1=None, h2=None, el=None, er=None):
	l = sorted(list, key=lambda l : l[3], reverse=footer)
	
	lc, top, num = [], -1, default
	for i in l:
		if h1 and h2 and (i[3] < h1 or i[3] > h2): continue
		if el and er and (i[2] >= el and i[2] <= er): continue
		if top != -1 and abs(i[3]-top) > 20:
			break
		if i[0].isdigit():
			if top == -1: top = i[3]
			lc.append(i)
	edge = -1
	for i in lc:
		if edge < abs(i[2]-300):
			edge = abs(i[2]-300)
			num = i[0]			
	return num

def text_getlinebypattern(list, pattern, r=2):
	s, h = None, None
	for i in list:
		if pattern in i[0]:
			s, h = '', i[3]
			break	
	if h:
		for i in list:
			if abs(h-i[3]) < r:
				s += (' ' + i[0])
	return s
	
def general_process(first, last):
	title = text_gettitle(first)
	if title == 'no-title':
		global g_count
		g_count += 1
		return 'FFFF-LLL.TITLE[Failed-%d].pdf' % g_count
	else:
		start = text_getnum(first, 'FFFF')
		end = text_getnum(last, 'LLL')
		return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)
	
def is_NEJM(list):
	pattern = [[0, 'new england journal'], [0, 'www.nejm.org']]
	for p in list:
		for i in pattern:
			if i[1] in p[0].lower():
				print(i[1])
				return True
	return False
	
def NEJM_process(first, last):
	#get the title
	title = text_gettitle(first, exclude=('Special Article', 'Review Article', 'ABSTRACT'))
	
	#get the first pagenum
	start = text_getnum(first, 'FFFF')
	
	#get the last pagenum
	end = text_getnum(last, 'LLL')
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_Spine(list):
	pattern = 'The Spine Journal'
	for p in list:
		if pattern in p[0]:
			print(pattern)
			return True
	return False

def Spine_process(first, last):
	title = text_gettitle(first, exclude=('\xc3\x93', '\xc3\xbe'), ss='The Spine Journal')
	
	digit = re.compile(r'(\d+)')
	start, end = 'FFFF', 'LLL'
	pattern = 'The Spine Journal'
	for p in first:
		if pattern in p[0]:
			print(p[0])
			mg = digit.findall(p[0])
			if len(mg) >= 4:
				start, end = mg[-2], mg[-1]
			elif len(mg) == 3: 
				start = mg[-1]
			break
	
	if not last: start = end
	elif end == 'LLL': end = text_getnum(last, end, False)
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_semss(list):
	pattern = [[0, 'seminars in spine surgery'], [0, 'j.semss']]
	for p in list:
		for i in pattern:
			if i[1] in p[0].lower():
				print(i[1])
				return True
	return False
	
def semss_process(first, last):
	title = text_gettitle(first)
	
	digit = re.compile(r'(\d+)')
	start, end = 'FFFF', 'LLL'
	pattern = 'S E M I N A R S I N S P I N E S U R G E R Y'
	for p in first:
		if pattern in p[0]:
			mg = digit.findall(p[0].translate(None, ' '))
			if len(mg) >= 4:
				start, end = mg[-2], mg[-1]
			break
	
	if start == 'FFFF':
		start = text_getnum(first, start)
		end = text_getnum(last, end, False)
		
	if not last: end = start
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_clineuro(list):
	pattern = [[0, 'Clinical Neurology and Neurosurgery'], [0, 'j.clineuro']]
	for p in list:
		for i in pattern:
			if i[1] in p[0]:
				print(i[1])
				return True
	return False	

def clineuro_process(first, last):
	title = text_gettitle(first, exclude=('Clinical Neurology and Neurosurgery'))

	#s number
	def sdigit(list):
		ne = re.compile(r'(S\d+)')
		for p in sorted(list, key=lambda l : l[3], reverse=True):
			mg = ne.match(p[0])
			if mg: return mg.group(1)
		return None
		
	pattern = 'Clinical'
	digit = re.compile(r'(.*)Neurosurgery.*?(\d+).*?(\d+).*?(S?\d+)(.*?)(S?\d+)(.*)')
	start, end = 'FFFF', 'LLL'
	s = text_getlinebypattern(first, pattern)
	if s:
		mg = digit.match(s)
		if mg:
			start, end = mg.group(4), mg.group(6)
			if not mg.group(5): start += end
			if start[0] == 'S':
				t1, t2 = sdigit(first), sdigit(last)
				if t1 and t1 != start and t1 != end: start = t1
				if t2 and t2 != start and t2 != end: end = t2
			else:
				ll = mg.group(1).split()
				rl = mg.group(7).split()
				tmp = 'FFFF'
				if ll and ll[0].isdigit(): tmp = ll[0]
				elif rl and rl[0].isdigit(): tmp = rl[0]
				if tmp != 'FFFF' and tmp != start and tmp != end: start = tmp
	
	if start == 'FFFF':
		start, end = sdigit(first), sdigit(last)
		if not start:
			start = text_getnum(first, start, False)
			end = text_getnum(last, end, False)			
	
	if start[0] == 'S':
		start = 'S' + start[1:].zfill(3)
		end = end[1:] if end else start[-3:]
		
	if not last: end = start
		
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_wneu(list):
	pattern = 'j.wneu'
	for p in list:
		if pattern in p[0].lower():
			print(pattern)
			return True
	return False

def wneu_process(first, last):
	title = text_gettitle(first, exclude=('Perspectives', 'Peer-Review Reports', 'Education &amp; Training'))
	
	digit = re.compile(r':e?(\d+).*?(\d+)[.]')
	start, end = 'FFFF', 'LLL'
	pattern = 'World Neurosurg'
	for p in first:
		if pattern in p[0]:
			mg = digit.search(p[0])
			if mg: 
				start, end = mg.group(1), mg.group(2)
				break
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)
	
def is_lancet(list):
	pattern = [[0, 'the lancet'], [0, 'www.thelancet.com']]
	for p in list:
		for i in pattern:
			if i[1] in p[0].lower():
				print(i[1])
				return True
	return False

def lancet_process(first, last):
	title = text_gettitle(first, exclude=('CORRESPONDENCE'))
	
	start = text_getnum(first, 'FFFF')
	
	end = text_getnum(last, 'LLL')

	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_ncna(list):
	pattern = 'Neurosurg Clin N Am'
	for p in list:
		if pattern in p[0]:
			print(pattern)
			return True
	return False
	
def ncna_process(first, last):
	title = text_gettitle(first)
	
	digit = re.compile(r'(\d+)')
	start, end = 'FFFF', 'LLL'
	pattern = 'Neurosurg Clin N Am'
	for p in first:
		if pattern in p[0]:
			mg = digit.findall(p[0].translate(None, ' '))
			if len(mg) >= 4:
				start, end = mg[-2], mg[-1]
			break
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_neurosurg(list):
	pattern = 'J Neurosurg'
	for p in list:
		if pattern in p[0]:
			print(pattern)
			return True
	return False
	
def neurosurg_process(first, last):
	title = text_gettitle(first)
	
	pattern = re.compile(r'J Neurosurg \d+')
	digit = re.compile(r'J Neurosurg \d+.*?(\d+).*?(\d+),')
	start, end = 'FFFF', 'LLL'
	s, h = None, None
	for p in first:
		if h:
			if abs(h-p[3]) < 2:
				s += p[0]
		else:
			mg = pattern.search(p[0])
			if mg: s, h = p[0], p[3]
	
	if s:
		mg = digit.search(s)
		if mg: start, end = mg.group(1), mg.group(2)
	else:
		start = text_getnum(first, 'FFFF')
		end = text_getnum(last, 'LLL')
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_cn(list):
	pattern = 'Clinical Neurosurgery'
	for p in list:
		if pattern in p[0]:
			print(pattern)
			return True
	return False

def cn_process(first, last):
	title = text_gettitle(first)
	
	start = text_getnum(first, 'FFFF')
	
	end = text_getnum(last, 'LLL')
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)
	
def is_neurosurgery(list):
	pattern = 'neurosurgery-online'
	ptother = 'NEUROSURGERY'
	for p in list:
		if pattern in p[0] or ptother == p[0] or p[0] == 'Table of Contents by Topic':
			print(pattern)
			return True
	return False
	
def neurosurgery_process(first, last):
	title = text_gettitle(first, exclude=('TECHNIQUE ASSESSMENTS', 'OPERATIVE NUANCES', 'SCIENCE TIMES'))
	
	digit = re.compile(r'Neurosurgery.*?:.*?(\d+).*?(\d+),')
	start, end = 'FFFF', 'LLL'
	for p in first:
		mg = digit.search(p[0])
		if mg: 
			start, end = mg.group(1), mg.group(2)
			break
	
	def nedigit(list):
		ne = re.compile(r'([N|E])(\d+)')
		for p in sorted(list, key=lambda l : l[3], reverse=True):
			mg = ne.match(p[0])
			if mg: return mg.group(1), mg.group(2)
		return None, None
		
	if start == 'FFFF':
		ss, start = nedigit(first)
		_, end = nedigit(last)
		if not start:
			start = text_getnum(first, 'FFFF')
			end = text_getnum(last, 'LLL')
		else:
			start = ss + start.zfill(3)
			end = end.zfill(3) if end else start[-3:]
		
	if end == 'LLL': end = start
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)
	
def is_nf(list):
	pattern = 'Neurosurg Focus'
	for p in list:
		if pattern in p[0].lower():
			print(pattern)
			return True
	return False

def nf_process(first, last):
	title = text_gettitle(first)
	
	start = text_getnum(first, 'FFFF')
	
	end = text_getnum(last, 'LLL')

	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def is_surg(list):
	pattern = [[0, 'Surg Neurol'], [0, 'Surgical Neurology']]
	for p in list:
		for i in pattern:
			if i[1] in p[0]:
				print(i[1])
				return True
	return False

def surg_process(first, last):
	title = text_gettitle(first)

	pattern = 'Surgical Neurology'
	digit = re.compile(r'Surgical Neurology.*?(\d+).*?(\d+).*?(\d+).*?(\d+)')
	start, end = 'FFFF', 'LLL'
	s = text_getlinebypattern(first, pattern) 
	if s:
		mg = digit.search(s)
		if mg: start, end = mg.group(3), mg.group(4)
		
	if start == 'FFFF':	
		digit = re.compile(r'(\d+)')
		pattern = 'Surg Neurol'
		for p in first:
			if pattern in p[0]:
				print p[0]
				mg = digit.findall(p[0].translate(None, ' '))
				if len(mg) >= 4:
					start, end = mg[-2], str(int(mg[-2])+int(mg[-1]))
				break
		
	return '%s-%s.%s.pdf'%(start.zfill(4), end.zfill(3)[-3:], title)

def run_test():
	names = []
	path = r'C:\Users\jzhang\Downloads'
	test = path + r'\run_test'
	if os.path.exists(test):
		for i in range(21, 22):
			p = path + r'\input' + str(i)
			if os.path.exists(p):
				names += folder_process(p, p+'_O', True)
		update = False
		with open(test, 'rt') as f:
			all = f.read()
			for n in names:
				if n not in all:
					update = True
					print('differ from last: ' + n)
		if update:
			with open(test, 'w+') as f:
				for n in names:
					f.write(n + '\n')
		
	return names != []

if __name__ == '__main__':
	main(sys.argv)
# -*- coding: utf-8 -*-
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
	if not os.path.exists(odir):
		print('create output dir: %s' % odir)
		os.makedirs(odir)
	for root, dirs, files in os.walk(idir):
		for file in files:
			if file.lower().endswith('.pdf'):
				try:
					path = os.path.join(root, file)
					outfile = pdf_process(path, reserve)
					print('copy from %s to %s' % (file, outfile))
					shutil.copy(path, os.path.join(odir, outfile).decode('utf-8'))				
				except Exception, e:
					print('error: %s -> %s' % (file, str(e)))	

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
		if os.path.exists(firstout) and os.path.exists(lastout):					
			html_textparser(firstout, first)
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
		
		if not lastpage:
			raise Exception, 'only one page, disregarded'
			
		interpreter.process_page(firstpage)
		with open(firstout, 'w') as f:
			f.write(outfp.getvalue())
		outfp.truncate(0)
		interpreter.process_page(lastpage)
		with open(lastout, 'w') as f:
			f.write(outfp.getvalue())
		
	device.close()	
	outfp.close()
	
	html_textparser(firstout, first)
	html_textparser(lastout, last)

	return first, last
	
def html_textparser(filename, list):
	'''
	lefttop = re.compile(r'left:(\d+)px; top:(\d+)px; width:(\d+)px;.*?>(.*?)($|<div)')
	fontsize = re.compile(r'font-size:(\d+)px">(.*?)($|<)')
	with open(filename, 'rt') as f:
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
							left = int(float(d[2])*tcur/tlen) + int(d[0])
							tcur += len(m[1])
						list.append( (t, int(m[0]), left, int(d[1])) )
	'''
	div = re.compile(r'<div.*?left:(\d+)px; top:(\d+)px; width:(\d+)px;.*?>(.*?)</div>', re.DOTALL)
	span = re.compile(r'<span.*?font-size:(\d+)px">(.*?)</span>', re.DOTALL)
	with open(filename, 'rt') as f:
		for d in div.findall(f.read()):
			mg = span.findall(d[3])
			tlen = sum([len(m[1]) for m in mg]) if len(mg) > 1 else -1
			tcur = 0
			for m in mg:
				t = m[1].replace('\n<br>', ' ').strip()
				if t != '':
					left = int(d[0])
					if tlen != -1: 
						left = int(float(d[2])*tcur/tlen) + int(d[0])
						tcur += len(m[1])
					list.append( (t, int(m[0]), left, int(d[1])) )

def pdf_process(filepath, reserve):
	first, last = pdf_gettext(filepath, reserve)	
	if is_NEJM(first):
		return NEJM_process(first, last)
	elif is_Spine(first):
		return Spine_process(first)
	elif is_semss(first):
		return semss_process(first, last)
	else:
		global g_count
		g_count += 1
		return 'FFFF-LLL.TITLE[Failed-%d].pdf' % g_count

def text_gettitle(list):
	l = sorted(list, key=lambda l : l[1], reverse=True)
	title = 'no-title'
	for i in l:
		w = i[0].split(' ')
		if len(w) > 2 and filter(lambda c:len(c) >= 2, w) != []:
			title = i[0]
			break
			
	t = None
	for i, p in enumerate(list):
		if t != None:
			if t[1] != p[1]: break
			title += ' ' + p[0]
		if not t and title == p[0]:
			t = p
	
	#get valid title:
	title = re.sub(r'[\/:*?"<>|]', '--', title)
	
	return title
			
def text_getnum(list, footer=True):
	r = -20 if footer else 20
	l = sorted(list, key=lambda l : l[3], reverse=footer)
	lc, top, num = [], -1, '0000'
	for i in l:
		if top != -1 and i[3] < top+r:
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
	
def is_NEJM(list):
	pattern = [[0, 'new england journal'], [0, 'new  england  journal'], [0, 'www.nejm.org'], [0, 'n engl j med']]
	for p in list:
		for i in pattern:
			if p[0].lower().find(i[1]) != -1:
				return True
	return False
	
def NEJM_process(first, last):
	#get the title
	title = text_gettitle(first)
	
	#get the first pagenum
	start = text_getnum(first).zfill(4)
	
	#get the last pagenum
	end = text_getnum(last).zfill(3)
	
	return '%s-%s.%s.pdf'%(start, end[-3:], title)

def is_Spine(list):
	pattern = 'the spine journal'
	for p in list:
		if p[0].lower().find(pattern) != -1:
			return True
	return False

def Spine_process(first):
	title = text_gettitle(first)
	
	digit = re.compile(r'(\d+)')
	start, end = '0000', '000'
	pattern = 'the spine journal'
	for p in first:
		if p[0].lower().find(pattern) != -1:
			mg = digit.findall(p[0])
			if len(mg) >= 2:
				start, end = mg[-2].zfill(4), mg[-1].zfill(3)
			break
	
	return '%s-%s.%s.pdf'%(start, end[-3:], title)

def is_semss(list):
	pattern = [[0, 'seminars in spine surgery'], [0, 'j.semss']]
	for p in list:
		for i in pattern:
			if p[0].lower().find(i[1]) != -1:
				return True
	return False
	
def semss_process(first, last):
	title = text_gettitle(first)
	
	digit = re.compile(r'(\d+)')
	start, end = '0000', '000'
	pattern = 'S E M I N A R S I N S P I N E S U R G E R Y'
	for p in first:
		if p[0].find(pattern) != -1:
			mg = digit.findall(p[0].translate(None, ' '))
			if len(mg) >= 2:
				start, end = mg[-2].zfill(4), mg[-1].zfill(3)
			break
	
	if start == '0000':
		start = text_getnum(first).zfill(4)	
		end = text_getnum(last, False).zfill(3)
	
	return '%s-%s.%s.pdf'%(start, end[-3:], title)
	
def run_test():
	path = r'C:\Users\jzhang\Downloads'
	ret = False
	if os.path.exists(path + r'\run_test'):
		for i in '123':
			p = path + r'\input' + i
			if os.path.exists(p):
				ret = True
				folder_process(p, p+'_O', True)
	return ret
	
if __name__ == '__main__':
	main(sys.argv)
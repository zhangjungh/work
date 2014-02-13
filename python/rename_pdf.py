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
	if not opts: return usage()
	
	idir = None
	odir = None
	for (k, v) in opts:
		if k == '-i': idir = v
		elif k == '-o': odir = v
	if os.path.exists(idir):
		if not os.path.exists(odir):
			print('create output dir: %s' % odir)
			os.makedirs(odir)
		for root, dirs, files in os.walk(idir):
			for file in files:
				path = os.path.join(root, file)
				with open(path, 'rb') as f:
					outfile = pdf_process(f)
					print('copy from %s to %s' % (file, outfile))
					shutil.copy(path, os.path.join(odir, outfile).decode('utf-8'))

def pdf_gettext(fp):
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
	
	for page in PDFPage.get_pages(fp, pagenos,
								maxpages=maxpages, password=password,
								caching=caching, check_extractable=True):
		if not firstpage:
			firstpage = page
		else:
			lastpage = page
	
	interpreter.process_page(firstpage)
	with open(firstout, 'w') as f:
		f.write(outfp.getvalue())
	outfp.truncate(0)
	interpreter.process_page(lastpage)
	with open(lastout, 'w') as f:
		f.write(outfp.getvalue())
		
	device.close()	
	outfp.close()
	
	first = []
	last = []
	
	html_textparser(firstout, first)
	html_textparser(lastout, last)

	return first, last
	
def html_textparser(filename, list):
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

def pdf_process(fp):
	first, last = pdf_gettext(fp)	
	if is_NEJM(first):
		return NEJM_process(first, last)
	else:
		global g_count
		g_count += 1
		return 'FFFF-LLL.TITLE[Failed-%d].pdf' % g_count
	
def is_NEJM(list):
	pattern = [[0, 'new england journal'], [0, 'new  england  journal'], [0, 'www.nejm.org'], [0, 'n engl j med']]
	for p in list:
		for i in pattern:
			if p[0].lower().find(i[1]) != -1:
				#print (p)
				i[0] = 1
	return sum([i[0] for i in pattern]) >= 1
	
def NEJM_process(first, last):
	#get the title
	l = sorted(first, key=lambda list : list[1], reverse=True)
	title = 'no-title'
	for i in l:
		w = i[0].split(' ')
		if len(w) > 2 and filter(lambda c:len(c) >= 2, w) != []:
			title = i[0]
			break
			
	index = -1
	for i in range(len(first)):
		if index != -1:
			if first[i][1] != first[index][1]:
				break
			title += ' ' + first[i][0]
		if index == -1 and title == first[i][0]:
			index = i
	
	#get the first pagenum
	start = text_getnum(first)
	
	#get the last pagenum
	end = text_getnum(last)
	end = end.zfill(3)
	
	return '%s-%s.%s.pdf'%(start.zfill(4), end[ len(end)-3:len(end) ], title)
	
def text_getnum(list):
	l = sorted(list, key=lambda l : l[3], reverse=True)
	lc, top, num = [], -1, '0000'
	for i in l:
		if top != -1 and i[3] < top-20:
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
			
if __name__ == '__main__':
	main(sys.argv)
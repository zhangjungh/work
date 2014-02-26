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
	names = []
	if not os.path.exists(odir):
		print('create output dir: %s' % odir)
		os.makedirs(odir)
	for root, dirs, files in os.walk(idir):
		for file in files:
			if file.lower().endswith('.pdf'):
				try:
					path = os.path.join(root, file)
					outfile = pdf_process(path, reserve)
					if len(outfile) > 255: outfile = outfile[:64]+'.pdf'
					names.append(outfile)
					print('copy from %s to %s' % (file, outfile))
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
		return Spine_process(first, last)
	elif is_semss(first):
		return semss_process(first, last)
	elif is_clineuro(first):
		return clineuro_process(first, last)
	elif is_wneu(first):
		return wneu_process(first, last)
	elif is_lancet(first):
		return lancet_process(first, last)
	else:
		global g_count
		g_count += 1
		return 'FFFF-LLL.TITLE[Failed-%d].pdf' % g_count

def text_gettitle(list, h1=0, h2=900, exclude=()):
	l = sorted(list, key=lambda l : l[1], reverse=True)
	title = 'no-title'
	for i in l:
		if i[3] >= h1 and i[3] <= h2 and i[0] not in exclude:
			w = i[0].split(' ')
			if filter(lambda c:len(c) >= 2, w) != []:
				title = i[0]
				break
				
	t = None
	for i, p in enumerate(list):
		if t != None:
			if t[1] != p[1] or t[3]-p[3] > 20: break
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
	pattern = [[0, 'new england journal'], [0, 'www.nejm.org']]
	for p in list:
		for i in pattern:
			if i[1] in p[0].lower():
				return True
	return False
	
def NEJM_process(first, last):
	#get the title
	title = text_gettitle(first, exclude=('Special Article', 'Review Article'))
	
	#get the first pagenum
	start = text_getnum(first).zfill(4)
	
	#get the last pagenum
	end = text_getnum(last).zfill(3)
	
	return '%s-%s.%s.pdf'%(start, end[-3:], title)

def is_Spine(list):
	pattern = 'the spine journal'
	for p in list:
		if pattern in p[0].lower():
			return True
	return False

def Spine_process(first, last):
	title = text_gettitle(first)
	
	digit = re.compile(r'(\d+)')
	start, end = '0000', '000'
	pattern = 'the spine journal'
	for p in first:
		if pattern in p[0].lower():
			mg = digit.findall(p[0])
			if len(mg) >= 2:
				start, end = mg[-2].zfill(4), mg[-1].zfill(3)
			break
	
	if not last: start = end.zfill(4)
	
	return '%s-%s.%s.pdf'%(start, end[-3:], title)

def is_semss(list):
	pattern = [[0, 'seminars in spine surgery'], [0, 'j.semss']]
	for p in list:
		for i in pattern:
			if i[1] in p[0].lower():
				return True
	return False
	
def semss_process(first, last):
	title = text_gettitle(first)
	
	digit = re.compile(r'(\d+)')
	start, end = '0000', '000'
	pattern = 'S E M I N A R S I N S P I N E S U R G E R Y'
	for p in first:
		if pattern in p[0]:
			mg = digit.findall(p[0].translate(None, ' '))
			if len(mg) >= 2:
				start, end = mg[-2].zfill(4), mg[-1].zfill(3)
			break
	
	if start == '0000':
		start = text_getnum(first).zfill(4)	
		end = text_getnum(last, False).zfill(3)
		
	if not last: end = start
	
	return '%s-%s.%s.pdf'%(start, end[-3:], title)

def is_clineuro(list):
	pattern = 'j.clineuro'#[[0, 'Clinical Neurology and Neurosurgery'], [0, 'j.clineuro']]
	for p in list:
		if pattern in p[0].lower():
			return True
	return False	

def clineuro_process(first, last):
	pattern = 'c l i n e u r o'
	h1, h2 = 0, 300
	for p in first:
		if pattern in p[0]:
			h1 = p[3]
			break
	title = text_gettitle(first, h1, h2)

	nc = []
	digit = re.compile(r'(\d+)')
	start, end = '0000', '000'
	for p in first:
		if p[3] < h1:
			mg = digit.findall(p[0])
			if mg: nc += mg
	if len(nc) >= 2:
		start, end = nc[-2].zfill(4), nc[-1].zfill(3)
	
	if start == '0000':
		start = text_getnum(first, False).zfill(4)
		end = text_getnum(last, False).zfill(3)
		
	return '%s-%s.%s.pdf'%(start, end[-3:], title)

def is_wneu(list):
	pattern = 'j.wneu'
	for p in list:
		if pattern in p[0].lower():
			return True
	return False

def wneu_process(first, last):
	title = text_gettitle(first, exclude=('Perspectives', 'Peer-Review Reports', 'Education &amp; Training'))
	
	digit = re.compile(r':e?(\d+).*?(\d+)[.]')
	start, end = '0000', '000'
	pattern = 'World Neurosurg'
	nc = []
	for p in first:
		if pattern in p[0]:
			mg = digit.search(p[0])
			if mg: start, end = mg.group(1).zfill(4), mg.group(2).zfill(3)
			break
	return '%s-%s.%s.pdf'%(start, end[-3:], title)
	
def is_lancet(list):
	pattern = [[0, 'the lancet'], [0, 'www.thelancet.com']]
	for p in list:
		for i in pattern:
			if i[1] in p[0].lower():
				return True
	return False

def lancet_process(first, last):
	title = text_gettitle(first, exclude=('CORRESPONDENCE'))
	
	start = text_getnum(first).zfill(4)
	
	end = text_getnum(last).zfill(3)

	return '%s-%s.%s.pdf'%(start, end[-3:], title)
	
def run_test():
	names = []
	path = r'C:\Users\jzhang\Downloads'
	test = path + r'\run_test'
	if os.path.exists(test):
		for i in '123456':
			p = path + r'\input' + i
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
#!/usr/bin/python2
# -*- coding: utf-8-*-
from Crypto.Cipher import AES
from Crypto import Random
from pbkdf2 import PBKDF2, crypt, b
from struct import pack
from random import randint

BS = 16
pad = lambda s: s + (BS - len(s) % BS) * chr(BS - len(s) % BS) 
unpad = lambda s : s[0:-ord(s[-1])]

def nrPadBytes(blocksize, size):
	'Return number of required pad bytes for block of size.'
	if not (0 < blocksize < 255):
		raise Error('blocksize must be between 0 and 255')
	return blocksize - (size % blocksize)

def appendPadding(blocksize, s):
	'''Append rfc 1423 padding to string.

	RFC 1423 algorithm adds 1 up to blocksize padding bytes to string s. Each
	padding byte contains the number of padding bytes.
	'''
	n = nrPadBytes(blocksize, len(s))
	return s + (chr(n) * n)

def removePadding(blocksize, s):
	'Remove rfc 1423 padding from string.'
	n = ord(s[-1]) # last byte contains number of padding bytes
	if n > blocksize or n > len(s):
		raise Error('invalid padding')
	return s[:-n]
'''
def padding():	
	pad_len = 8 - (len(data) % self.block_size)
	if _pythonMajorVersion < 3:
		data += pad_len * chr(pad_len)
	else:
		data += bytes([pad_len] * pad_len)
	if _pythonMajorVersion < 3:
		pad_len = ord(data[-1])
	else:
		pad_len = data[-1]
	data = data[:-pad_len]
'''	
	
class AESCipher:
	def __init__( self, key ):
		"""
		Requires hex encoded param as a key
		"""
		self.key = key.decode("hex")

	def encrypt( self, raw ):
		"""
		Returns hex encoded encrypted value!
		"""
		raw = pad(raw)
		iv = Random.new().read(AES.block_size);
		cipher = AES.new( self.key, AES.MODE_CBC, iv )
		return ( iv + cipher.encrypt( raw ) ).encode("hex")

	def decrypt( self, enc ):
		"""
		Requires hex encoded param to decrypt
		"""
		enc = enc.decode("hex")
		iv = enc[:16]
		enc= enc[16:]
		cipher = AES.new(self.key, AES.MODE_CBC, iv )
		return unpad(cipher.decrypt( enc))

if __name__== "__main__":
	key = "140b41b22a29beb4061bda66b6747e14"
	ciphertext = "4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";
	key=key[:32]
	decryptor = AESCipher(key)
	plaintext = decryptor.decrypt(ciphertext)
	print "%s" % plaintext
	
with open(r'f:\downloads\TSREXPORT_20131217144602', 'rb') as f:
	data = f.read()
	
	initVector = "tsrvector9567482"
	salt = "akdirn%/=(+\"922xc&@s"
	pswdIterations = 128
	password = "_D?+!!%/\"weg112W"
	print(initVector)
	saltb = salt.encode('utf8')
	saltc = salt.decode('utf8')
	print(salt, saltb, saltc)
	print(type(salt),type(saltb),type(saltc))
	wb = initVector.encode('utf8')
	initVectorB = b("").join([pack("@H", randint(0, 0xffff)) for i in range(16)])
	print(type(saltb), type(wb), type(initVectorB))
	#for i in range(16):
	#	if i < len(wb): initVectorB[i] = wb[i]
	#	else: initVectorB[i] = -100-i
	#print(initVectorB)
	
	pbk = PBKDF2(password, saltb, pswdIterations)
	
	#k = pbk.read(256)
	#for i in k: print(ord(i))
	cipher = AES.new(pbk.read(32), AES.MODE_CBC, initVector)
	plaintext = cipher.decrypt(data)
	#for i in plaintext: print(ord(i))
	print(plaintext)
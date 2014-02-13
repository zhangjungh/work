#!/usr/bin/python2
# -*- coding: utf-8-*-

#please put aescbc.py and pbkdf2.py in path

from aescbc import AES_Decrypt


initVector = "tsrvector956748245542"
salt = "akdirn%/=(+\"922xc&@s"
pswdIterations = 128
password = "_D?+!!%/\"weg112W"

AES_Decrypt(r'f:\downloads\TSREXPORT_20131217144602', r'f:\downloads\output', initVector, salt, pswdIterations, password)
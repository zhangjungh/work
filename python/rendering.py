# -*- coding: GB2312 -*-

import sys
import time
from PyQt4 import QtCore, QtGui, uic
import tetris

class subWidget(QtGui.QWidget):
	def __init__(self, parent, geometry):
		QtGui.QWidget.__init__(self, parent)
		self.setGeometry(geometry)
		self.sz = 16; #should be calculated
		self.data = []
		self.main = []
	def updateData(self, d):
		self.data = d;
		self.update() #force rendering
	def updateMain(self, d, m):
		self.data, self.main = d, m
		self.update()
	def paintEvent(self, event):
		paint = QtGui.QPainter()
		paint.begin(self)
		brush = QtGui.QBrush(QtCore.Qt.Dense5Pattern)
		paint.setBrush(brush)
		for i in self.data:
			if i[0] != -1: paint.drawRect(i[0]*self.sz, i[1]*self.sz, self.sz, self.sz)
		for i in range(len(self.main)):
			for j in range(len(self.main[i])):
				if self.main[i][j] == 1: paint.drawRect(j*self.sz, i*self.sz, self.sz, self.sz)
		paint.end()
		
class TWindow(QtGui.QDialog):
	def __init__(self, parent=None):
		QtGui.QDialog.__init__(self, parent)
		uic.loadUi('tetrisui.ui', self)		
		#main drawing window
		self.main = subWidget(self, self.mholder.geometry())
		self.main.show()
		#next coming drawing window
		self.next = subWidget(self, self.dholder.geometry())
		self.next.show()
		#button message
		self.connect(self.start, QtCore.SIGNAL('clicked()'), self.onStart)
		self.connect(self.pause, QtCore.SIGNAL('clicked()'), self.onPause)
		#level & speed, speed means the dropping speed, next level would cut 10% of time
		self.lv = 0
		self.sp = 1000
		#timer to trigger the dropping
		self.timer = QtCore.QTimer()
		self.connect(self.timer, QtCore.SIGNAL('timeout()'), self.onTimeout)
		#tetris
		self.tetris = tetris.Tetris()
	def onTimeout(self):
		if not self.tetris.moveDown():
			self.score.setText(str(self.tetris.s))
			if self.tetris.s >= (self.lv+1) * 3000:
				self.lv += 1
				self.level.setText(str(self.lv))
				self.sp *= 0.9
			QtCore.QTimer.singleShot(0, self.onTimeout)
		if self.tetris.d:
			self.level.setText('dead')
		if self.tetris.f:
			self.tetris.randomTetris()
			self.next.updateData(self.tetris.n)
		self.main.updateMain(self.tetris.t, self.tetris.m)
	def keyPressEvent(self, event):
		if event.key() == QtCore.Qt.Key_Escape:
			self.close()
		elif event.key() == QtCore.Qt.Key_Up:
			self.tetris.rotateNinety()
			self.main.updateMain(self.tetris.t, self.tetris.m)
		elif event.key() == QtCore.Qt.Key_Down:
			self.tetris.moveDown()
			self.main.updateMain(self.tetris.t, self.tetris.m)
		elif event.key() == QtCore.Qt.Key_Left:
			self.tetris.moveLeft()
			self.main.updateMain(self.tetris.t, self.tetris.m)
		elif event.key() == QtCore.Qt.Key_Right:
			self.tetris.moveRight()
			self.main.updateMain(self.tetris.t, self.tetris.m)
	def onStart(self):
		if self.start.text() != 'continue':
			self.tetris.randomTetris()
			self.next.updateData(self.tetris.n)
		self.timer.start(self.sp)
		self.start.setDisabled(True)
	def onPause(self):
		if self.timer.isActive():
			self.timer.stop()
		self.start.setText('continue')
		self.start.setDisabled(False)

if __name__ == '__main__':
	print(QtCore.QT_VERSION_STR)
	
	app = QtGui.QApplication(sys.argv)

	widget = TWindow()
	widget.show()

	sys.exit(app.exec_())
	
# -*- coding: GB2312 -*-

'''
Tetris game - concept
1. random tetris generate
2. animation with speed control
3. rotate (central and boundary)
4. keyboard control <- move left, -> move right ^ rotate, | speed up
5. collision
6. fullfil row
7. score calculation
8. level / speed
9. start / stop ( die )
Tetris game - graphics
1. current moved tetris
2. existed tetris
3. fadeout effect
4. pre-render next tetris
'''

'''
milestones
      1h                    .5h                   1h              1h                 2h                  2h                3h              2h
------|----------------------|--------------------|---------------|------------------|-------------------|-----------------|---------------|------------
graphics library       keyboard message     draw tetris     random tetris      data structure         rotate          collision          other
'''

import sys
import tetris
from PyQt4 import QtCore, QtGui

if __name__ == '__main__':
	print(QtCore.QT_VERSION_STR)
	t = tetris.Tetris()
	t.randomTetris()
	t.show()
	
	app = QtGui.QApplication(sys.argv)

	widget = QtGui.QWidget()
	widget.resize(800, 600)
	widget.setWindowTitle('Trtris')
	widget.show()

	sys.exit(app.exec_())
	
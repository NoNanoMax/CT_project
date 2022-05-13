from PyQt5 import QtWidgets, QtGui
from lib.info import Ui_MainWindow
import sys
import os
from lib.drawer import draw_men_with_dumplings_lol 
import numpy as np

'''class Picture(QtWidgets.QMainWindow):
    def __init__(self, pic):
        super(Picture, self).__init__()
        self.figure = Figure(figsize=(5, 3))
        self.canvas = FigureCanvas(self.figure)
        self.ax = self.figure.  lots()
        self.pic = pic
        self.ax.imshow(self.pic)
        self.ax.set_axis_off()
        self.setCentralWidget(self.canvas) '''

class mywindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(mywindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.hide_all()
        self.ui.label.setText("")

        self.ui.label.setFont(
            QtGui.QFont('SansSerif', 10)
        )
        self.ui.label_2.setFont(
            QtGui.QFont('SansSerif', 10)
        )
        self.ui.label_2.setText("Введенные данные")
        self.ui.pushButton_2.setText("Нарисовать")
        self.ui.pushButton_3.setText("Очистить")
        #self.ui.pushButton_6.setText("Изменить\n объект №") # <======
        self.ui.label_9.setText("Имя файла")
        self.ui.pushButton_4.setText("Загрузить сцену")
        self.ui.pushButton_5.setText("Сохранить сцену")

        self.ui.pushButton.clicked.connect(self.btnClicked)
        self.ui.pushButton_2.clicked.connect(self.draw)
        self.ui.pushButton_3.clicked.connect(self.hide)
        self.ui.pushButton_4.clicked.connect(self.OpenFile)
        self.ui.pushButton_5.clicked.connect(self.SaveFile)
        #self.ui.pushButton_6.clicked.connect(self.Change) # <======

        self.figures = []
        self.to_put = ""
        self.counter = 0
        self.ui.pushButton.setText("Ввести объект")

        self.name_figures = {
            "Сфера": "BeautifulSphere",
            "Плоскость": "BeautifulPlane",
            "Освещение": "Ambient_light",
            "Точечное освещение": "Point_light"
        }

        self.ui.comboBox.addItem("Сфера")
        self.ui.comboBox.addItem("Плоскость")
        self.ui.comboBox.addItem("Освещение")
        self.ui.comboBox.addItem("Точечное освещение")

    def SaveFile(self):
        filename = self.ui.lineEdit_7.text()
        f = open(os.path.join("scene_collection", filename), "w")
        f.write(self.to_put)
        f.close()

    def OpenFile(self):
        filename = self.ui.lineEdit_7.text()
        if not os.path.exists("scene_collection/" + filename) or filename == "":
            return
        f = open(os.path.join("scene_collection", filename), "r")
        self.to_put = f.read()
        self.ui.label.setText(self.to_put)   
        f.close()

    def draw(self):
        f = open("info.ass", "w")
        if self.to_put.find("Camera") == -1:
            f.write("Camera 0 0 4 0 0 0 1.2 1000 1000 1\n")
        f.write(self.to_put)
        f.close()
        draw_men_with_dumplings_lol()
        os.remove("info.ass")

    def Change(self):
        return

    def hide(self):
        self.hide_all()
        self.to_put = ""
        self.figures = ""
        self.ui.label.setText("")

    def hide_all(self):
        self.ui.lineEdit.hide()
        self.ui.lineEdit_2.hide()
        self.ui.lineEdit_3.hide()
        self.ui.lineEdit_4.hide()
        self.ui.lineEdit_5.hide()
        self.ui.lineEdit_6.hide()

        self.ui.lineEdit.clear()
        self.ui.lineEdit_2.clear()
        self.ui.lineEdit_3.clear()
        self.ui.lineEdit_4.clear()
        self.ui.lineEdit_5.clear()
        self.ui.lineEdit_6.clear()

        self.ui.label_3.setText("")
        self.ui.label_4.setText("")
        self.ui.label_5.setText("")
        self.ui.label_6.setText("")
        self.ui.label_7.setText("")
        self.ui.label_8.setText("")

    def btnClicked(self):
        self.counter += 1

        if self.counter == 1:
            self.ui.pushButton.setText("Ввести данные")
            typ = self.name_figures[self.ui.comboBox.currentText()]
            if typ == "BeautifulSphere":
                self.ui.label_3.setText("x")
                self.ui.label_4.setText("y")
                self.ui.label_5.setText("z")
                self.ui.label_6.setText("радиус")
                self.ui.label_7.setText("цвет")
                self.ui.label_8.setText("материал")
                self.ui.lineEdit.show()
                self.ui.lineEdit_2.show()
                self.ui.lineEdit_3.show()
                self.ui.lineEdit_4.show()
                self.ui.lineEdit_5.show()
                self.ui.lineEdit_6.show()
            
            if typ == "BeautifulPlane":
                self.ui.label_3.setText("x")
                self.ui.label_4.setText("y")
                self.ui.label_5.setText("z")
                self.ui.label_6.setText("d")
                self.ui.label_7.setText("цвет")
                self.ui.label_8.setText("материал")
                self.ui.lineEdit.show()
                self.ui.lineEdit_2.show()
                self.ui.lineEdit_3.show()
                self.ui.lineEdit_4.show()
                self.ui.lineEdit_5.show()
                self.ui.lineEdit_6.show()

            if typ == "Ambient_light":
                self.ui.label_3.setText("i")
                self.ui.lineEdit.show()

            if typ == "Point_light":
                self.ui.label_3.setText("x")
                self.ui.label_4.setText("y")
                self.ui.label_5.setText("z")
                self.ui.label_6.setText("i")
                self.ui.lineEdit.show()
                self.ui.lineEdit_2.show()
                self.ui.lineEdit_3.show()
                self.ui.lineEdit_4.show()


        if self.counter == 2:
            self.ui.pushButton.setText("Добавить данные")
            typ = self.name_figures[self.ui.comboBox.currentText()]
            if typ == "BeautifulSphere":
                self.figures.append({
                    "figure": typ,
                    "x": self.ui.lineEdit.text(),
                    "y": self.ui.lineEdit_2.text(),
                    "z": self.ui.lineEdit_3.text(),
                    "r": self.ui.lineEdit_4.text(),
                    "color": self.ui.lineEdit_5.text(),
                    "mat": self.ui.lineEdit_6.text()
                })
                for s in self.figures[-1].values():
                    self.to_put += s + " "
                self.to_put += "\n"

            if typ == "BeautifulPlane":
                self.figures.append({
                    "figure": typ,
                    "x": self.ui.lineEdit.text(),
                    "y": self.ui.lineEdit_2.text(),
                    "z": self.ui.lineEdit_3.text(),
                    "d": self.ui.lineEdit_4.text(),
                    "color": self.ui.lineEdit_5.text(),
                    "mat": self.ui.lineEdit_6.text()
                })
                for s in self.figures[-1].values():
                    self.to_put += s + " "
                self.to_put += "\n"
            
            if typ == "Ambient_light":
                self.figures.append({
                    "figure": typ,
                    "i": self.ui.lineEdit.text()
                })
                for s in self.figures[-1].values():
                    self.to_put += s + " "
                self.to_put += "\n"

            if typ == "Point_light":
                self.ui.label_3.setText("x")
                self.ui.label_4.setText("y")
                self.ui.label_5.setText("z")
                self.ui.label_6.setText("i")
                self.ui.lineEdit.show()
                self.ui.lineEdit_2.show()
                self.ui.lineEdit_3.show()
                self.ui.lineEdit_4.show()
                self.figures.append({
                    "figure": typ,
                    "x": self.ui.lineEdit.text(),
                    "y": self.ui.lineEdit_2.text(),
                    "z": self.ui.lineEdit_3.text(),
                    "i": self.ui.lineEdit_4.text()
                })
                for s in self.figures[-1].values():
                    self.to_put += s + " "
                self.to_put += "\n"

            self.ui.pushButton.setText("Добавить объект")
            self.hide_all()
            self.ui.label.setText(self.to_put)   
            self.counter = 0       

if __name__ == "__main__":
    app = QtWidgets.QApplication([])
    application = mywindow()
    application.show()

    sys.exit(app.exec())

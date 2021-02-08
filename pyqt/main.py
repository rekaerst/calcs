#!/usr/bin/python

import sys
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QPushButton, QVBoxLayout, QLineEdit


class Calc(QWidget):

    def __init__(self):
        super().__init__()
        self.display = QLineEdit()
        self.initUI()

    def initUI(self):
        self.setWindowTitle("Calculator")
        self.display.setText("I fart in your general direction")

        gridLayout = QGridLayout()

        mainLayout = QVBoxLayout() 
        mainLayout.addWidget(self.display)
        mainLayout.addLayout(gridLayout)
        self.setLayout(mainLayout)
        

        buttonNames = ["C", "<-", "%", "/",
                       "7", "8", "9", "x",
                       "4", "5", "6", "-",
                       "1", "2", "3", "+",
                       ".", "0", "="]

        buttonPositions = [(i, j) for i in range(5) for j in range(4)]

        for position, name in zip(buttonPositions, buttonNames):
            button = QPushButton(name)

            if name == "=":
                gridLayout.addWidget(button, 4, 2, 1, 2)
                continue

            if name.isdigit():
                button.clicked.connect(
                    lambda checked, arg=int(name): self.on_number_button_clicked(arg))
            else:
                button.clicked.connect(
                    lambda checked, arg=name: self.on_button_clicked(arg)
                )

            gridLayout.addWidget(button, *position)

        self.setWindowTitle("Calculator")
        self.show()

    def on_number_button_clicked(self, digit):
        print(str(digit) + " clicked")

    def on_button_clicked(self, name):
        print(name + " clicked")


def main():
    app = QApplication(sys.argv)

    calc = Calc()

    sys.exit(app.exec())


if __name__ == '__main__':
    main()

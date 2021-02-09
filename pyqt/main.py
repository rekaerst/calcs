#!/usr/bin/python

import sys
import math
from enum import Enum, auto
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QPushButton, QVBoxLayout, QLineEdit, QShortcut
from PyQt5.QtGui import QKeySequence


class Calc(QWidget):

    class Display(QLineEdit):
        def __init__(self):
            super().__init__()
            self.setReadOnly(True)

        def backspace(self):
            if self.text() == "0":
                pass
            else:
                super().backspace()

        def insert(self, a0: str):
            if a0 != ".":
                if self.text() == "0":
                    self.setText(a0)
                else:
                    super().insert(a0)
            elif self.text().find(".") == -1:
                super().insert(".")

        def clear(self):
            self.setText("0")

    class Operation(Enum):
        NUL = auto()
        ADD = auto()
        SUB = auto()
        MUL = auto()
        DIV = auto()
        MOD = auto()

    def __init__(self):
        super().__init__()
        self.display = Calc.Display()
        self.accumulator = 0
        self.operation = Calc.Operation.NUL
        self.is_operation_button_clicked = False

        self.setStyleSheet(
            """QWidget {
	background-color: #181818;
}
QLineEdit {
	background-color: #9CAA9B;
	color: #222E31;
	font: 24px "Monospace";
}
"""
        )
        self.initUI()

    def initUI(self):
        self.setWindowTitle("Calculator")
        self.display.clear()

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
            button = QPushButton("&" + name)
            if name.isdigit():
                button.clicked.connect(
                    lambda checked, arg=int(name): self.on_number_button_clicked(arg))
            else:
                button.clicked.connect(
                    lambda checked, arg=name: self.on_button_clicked(arg)
                )

            if name == "=":
                gridLayout.addWidget(button, 4, 2, 1, 2)
                continue

            gridLayout.addWidget(button, *position)

        for i in range(0, 10):
            shortcut = QShortcut(QKeySequence(str(i)), self)
            shortcut.activated.connect(
                lambda arg=i: self.on_number_button_clicked(arg))

        shortcut = QShortcut(QKeySequence("c"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("C"))
        shortcut = QShortcut(QKeySequence("Backspace"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("<-"))
        shortcut = QShortcut(QKeySequence("+"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("+"))
        shortcut = QShortcut(QKeySequence("-"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("-"))
        shortcut = QShortcut(QKeySequence("*"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("*"))
        shortcut = QShortcut(QKeySequence("x"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("*"))
        shortcut = QShortcut(QKeySequence("/"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("/"))
        shortcut = QShortcut(QKeySequence("%"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("%"))
        shortcut = QShortcut(QKeySequence("."), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("."))
        shortcut = QShortcut(QKeySequence("="), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("="))
        shortcut = QShortcut(QKeySequence("Enter"), self)
        shortcut.activated.connect(lambda arg=i: self.on_button_clicked("="))

        self.show()

    def calculate(self, operation):
        if not self.is_operation_button_clicked:
            if self.operation == Calc.Operation.ADD:
                self.accumulator += float(self.display.text())
            elif self.operation == Calc.Operation.SUB:
                self.accumulator -= float(self.display.text())
            elif self.operation == Calc.Operation.MUL:
                self.accumulator *= float(self.display.text())
            elif self.operation == Calc.Operation.DIV:
                self.accumulator /= float(self.display.text())
            elif self.operation == Calc.Operation.MOD:
                self.accumulator %= float(self.display.text())
            elif self.operation == Calc.Operation.NUL:
                self.accumulator = float(self.display.text())

        if math.ceil(self.accumulator) == self.accumulator:
            self.display.setText(str(int(self.accumulator)))
        else:
            self.display.setText(str(self.accumulator))
        self.is_operation_button_clicked = True
        self.operation = operation

    def on_number_button_clicked(self, digit: int):
        if self.is_operation_button_clicked:
            self.display.clear()
            self.is_operation_button_clicked = False
        self.display.insert(str(digit))

    def on_button_clicked(self, name: str):
        if name == "C":
            self.accumulator = 0
            self.is_operation_button_clicked = False
            self.operation = Calc.Operation.NUL
            self.display.clear()
        elif name == "<-":
            self.display.backspace()
        elif name == "+":
            self.calculate(Calc.Operation.ADD)
        elif name == "-":
            self.calculate(Calc.Operation.SUB)
        elif name == "x":
            self.calculate(Calc.Operation.MUL)
        elif name == "/":
            self.calculate(Calc.Operation.DIV)
        elif name == "%":
            self.calculate(Calc.Operation.MOD)
        elif name == "=":
            self.calculate(Calc.Operation.NUL)
        elif name == ".":
            self.display.insert(".")


def main():
    app = QApplication(sys.argv)

    calc = Calc()

    sys.exit(app.exec())


if __name__ == '__main__':
    main()

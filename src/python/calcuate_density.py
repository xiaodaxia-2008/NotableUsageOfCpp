from PySide6.QtWidgets import QApplication, QWidget, QLabel, QLineEdit, QPushButton, QVBoxLayout, QHBoxLayout
import sys
from iapws import IAPWS97

class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle("水比容计算器")
        
        # 创建用户输入框以输入水的温度和压力
        self.temperature_label = QLabel("水的温度（℃）：", self)
        self.temperature_input = QLineEdit(self)

        self.pressure_label = QLabel("水的压力（MPa）：", self)
        self.pressure_input = QLineEdit(self)

        # 创建计算按钮
        self.calculate_button = QPushButton("计算", self)
        self.calculate_button.clicked.connect(self.calculate_specific_volume)

        # 创建用于显示计算结果的标签
        self.result_label = QLabel(self)

        # 将所有小部件放在水平和垂直框中
        input_hbox = QHBoxLayout()
        input_hbox.addWidget(self.temperature_label)
        input_hbox.addWidget(self.temperature_input)

        input_hbox2 = QHBoxLayout()
        input_hbox2.addWidget(self.pressure_label)
        input_hbox2.addWidget(self.pressure_input)

        button_hbox = QHBoxLayout()
        button_hbox.addWidget(self.calculate_button)

        result_hbox = QHBoxLayout()
        result_hbox.addWidget(self.result_label)

        vbox = QVBoxLayout()
        vbox.addLayout(input_hbox)
        vbox.addLayout(input_hbox2)
        vbox.addLayout(button_hbox)
        vbox.addLayout(result_hbox)

        self.setLayout(vbox)

    def calculate_specific_volume(self):
        try:
            # 从用户输入中提取温度和压力
            temperature = float(self.temperature_input.text())
            pressure = float(self.pressure_input.text())

            # 将温度从 ℃ 转换为 K
            temperature = temperature + 273.15

            # 计算水的比容
            water = IAPWS97(T=temperature, P=pressure)
            specific_volume = water.v
            
            self.result_label.setText(f"水的比容为：{specific_volume:.6f} m³/kg")

        except ValueError:
            # 处理用户输入错误的情况
            self.result_label.setText("输入无效！请确保输入值是实数。")


if __name__ == '__main__':
    app = QApplication(sys.argv)
    main = MainWindow()
    main.show()
    sys.exit(app.exec())

import math

# 用户可输入的参数
Cv_rating = 2.5  # 阀门的额定Cv值，单位 -
delta_p = 2.0  # 调节阀两端压差，单位 kgf/cm^2
P1 = 10.0  # 调节阀进口压力，单位 kgf/cm^2
opening = 0.5  # 调节阀的开度，单位 %

# 常量
rho = 0.997  # 流体密度，单位 kg/m^3
m2 = 1.0  # 调节阀两端的质量流率，单位 kg/h
d = 15.0  # 调节阀口径，单位 mm
K = 0.95  # 理论流量系数

# 计算调节阀的流量系数Kv
Kv = Cv_rating / 1.17  # 根据额定Cv值计算Kv值（假设流体是水）

# 根据调节阀的开度计算实际Cv值
x = opening / 100
Cv = Kv / (math.sqrt(delta_p) * x)

# 计算Reynolds数Ren
G = m2 / (rho * 3600)
u = 0.55 / 1000
D = d / 1000
Ren = 30000 * G / (D * u)

# 计算噪声特性系数Kw
Kw = 4.4 / ((Ren * x) ** 0.5)  # 这里我们简化一下计算Kw的方法

# 计算噪音级别Lw和声压级Lp
Q2 = G / rho
V2 = 4 * Q2 / (math.pi * (d ** 2) * 3600)
Lw = Kw + 10 * math.log10(Q2 / m2) + 16.6 * math.log10(V2 / (u * 100))
Lp = Lw - 20 * math.log10(1.0 / 1.0)  # 假设计算点距离参考点是1米

print("调节阀的流量系数Kv为：{:.3f}".format(Kv))
print("调节阀的实际Cv值为：{:.3f}".format(Cv))
print("Reynolds数Ren为：{:.3f}".format(Ren))
print("噪声特性系数Kw为：{:.3f}".format(Kw))
print("噪声级别Lw为：{:.3f} dB(A)".format(Lw))
print("声压级Lp为：{:.3f} dB(A)".format(Lp))

from sympy import symbols, Matrix, pprint

P00, P01, P02, P10, P11, P12, P20, P21, P22 = symbols(
    'P_00 P_01 P_02 P_10 P_11 P_12 P_20 P_21 P_22')
p, v, a = symbols('p v a')
q, dt = symbols('q dt')

F = Matrix([[1, dt, dt**2 / 2], [0, 1, dt], [0, 0, 1]])
P = Matrix([[P00, P01, P02], [P10, P11, P12], [P20, P21, P22]])
G = Matrix([[dt**2 / 2], [dt], [1]])
Q = G * G.T
x = Matrix([[p], [v], [a]])

sa2 = q * dt

Fx = F * x
FPF = F * P * F.T

print("\nx=")
pprint(x)

print("\nP=")
pprint(P)

print("\nF=")
pprint(F)

print("\nFx=")
pprint(Fx)

print("\nFPF=")
pprint(FPF)

print("\nQ=")
pprint(Q)

print("\nQ (dt q)=")
pprint(Q * sa2)

##Calcolo tensione di breakdown e capacità del diodo a partire dai parametri ottenuti dai best fit dei dati.

from scipy.constants import e #carica elettrica
import math

#Prima esperienza

#guadagno 40dB
p0=-37.51
p1=0.7074
err_p0 = 0.2973
err_p1 = 0.005167

C_D= p1 * e
C_D_err = err_p1* e
V_br= -p0/p1
V_br_err = err_p0 /p1 + p0*err_p1/(p1*p1)
V_br_err = V_br * math.sqrt((err_p0 / p0) ** 2 + (err_p1 / p1) ** 2)

print(" \n Guadagno 40dB")
print("V_breakdown =", V_br, "\n err =", V_br_err)
print("C_D =", C_D, "\n err =", C_D_err,"\n")

#guadagno 32dB
p0=-38.17
p1=0.7194
err_p0 = 0.8228
err_p1 = 0.01484

C_D= p1 *e
C_D_err = err_p1* e
V_br= -p0/p1
V_br_err = err_p0 /p1 + p0*err_p1/(p1*p1)
V_br_err = V_br * math.sqrt((err_p0 / p0) ** 2 + (err_p1 / p1) ** 2)

print(" \n Guadagno 32dB")
print("V_breakdown =", V_br, "\n err =", V_br_err)
print("C_D =", C_D, "\n err =", C_D_err, "\n")

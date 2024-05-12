import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Exponente que indica la cantidad de puntos del set P
# Es para el label más que nada
# Y para el nombre del archivo ahora
n = 21

# Para el método CP
fileNameCP = 'experimento_CP_' + str(n) + '.tsv'
dfCP = pd.read_csv(fileNameCP, sep = '\t')
# display(dfCP)

# Para el método SS
# fileNameSS = 'experimento_SS_' + str(n) + '.tsv'
# dfSS = pd.read_csv(fileNameSS, sep = '\t')

# El eje X son los accesos
# El eje Y el conteo de esos accesos

iosDataCP = dfCP['ios'].values
# iosDataSS = dfSS['ios'].values

# Experimentación en una experimentación supongo
# Para hacer una distribución normal basada en estos valores x
# mediaX = np.mean(dataCP)
# desviacionEstandarX = np.std(dataCP)
# datosNormal = np.random.normal(mediaX, desviacionEstandarX, 1000)

# Se arman los histogramas a partir de los datos que supuestamente ya tenemos
# bins: cantidad de "contenedores" para dividir los datos
# density: normalizar la altura de cada contenedor (influirá si ambos trabajan con 100 datos?)
# alpha: transparencia/opacidad de las barras
plt.hist(iosDataCP, bins=25, density=False, alpha=0.5, color='r', label='Método CP')
# plt.hist(dataSS, bins=25, density=False, alpha=0.5, color='b', label='Método SS')
# plt.hist(datosNormal, bins=25, density=True, alpha=0.5, color='g', label='Distribución normal')

# Labels
plt.xlabel('Cantidad de accesos')
plt.ylabel('Frecuencia')
plt.title('Distribución de accesos para 2^' + str(n) + ' puntos')
plt.legend()

# Para dibujar la curva
# mu, sigma = np.mean(datosNormal), np.std(datosNormal)
# x_dn = np.linspace(mu - 3*sigma, mu + 3*sigma, 100)
# plt.plot(x_dn, 1/(sigma * np.sqrt(2 * np.pi)) * np.exp(-0.5 * ((x_dn - mu) / sigma) ** 2), color='r')

# Mostrar el histograma
plt.show()

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Exponente que indica la cantidad de puntos del set P
# Es para el label más que nada
# Y para el nombre del archivo ahora
n = 13
totalPuntos = 2**n

# Para el método CP
fileNameCP = 'experimento_CP_' + str(n) + '.tsv'
dfCP = pd.read_csv(fileNameCP, sep = '\t')
# display(dfCP)

# Para el método SS
fileNameSS = 'wsl_experimento_SS_' + str(n) + '.tsv'
dfSS = pd.read_csv(fileNameSS, sep = '\t')

# El eje X son los accesos
# El eje Y el conteo de esos accesos
iosDataCP = dfCP['ios'].values
iosDataSS = dfSS['ios'].values

# Datos para CP
promedioCP = iosDataCP.mean()
stdCP = iosDataCP.std()
intervaloConfianzaCP = 1.96 * (promedioCP / np.sqrt(len(iosDataCP)))
dfCP['% del conjunto'] = (dfCP['size'] / totalPuntos)*100

# Datos para SS
promedioSS = iosDataSS.mean()
stdSS = iosDataSS.std()
intervaloConfianzaSS = 1.96 * (promedioSS / np.sqrt(len(iosDataSS)))
dfSS['% del conjunto'] = (dfSS['size'] / totalPuntos)*100

# Printeo de datos
print(" ")
print("Datos del método CP -------------------------------------------------")
print("Promedio CP: " + str(promedioCP))
print("Desviación estándar CP: " + str(stdCP))
print("Intervalo de Confianza al 95%: " + str(intervaloConfianzaCP))
print("Promedio del % del conjunto de puntos como respuesta a la búsqueda: " + str(dfCP['% del conjunto'].mean()))
print(" ")
print("Datos del método SS -------------------------------------------------")
print("Promedio SS: " + str(promedioSS))
print("Desviación estándar SS: " + str(stdSS))
print("Intervalo de Confianza al 95%: " + str(intervaloConfianzaSS))
print("Promedio del % del conjunto de puntos como respuesta a la búsqueda: " + str(dfSS['% del conjunto'].mean()))
print(" ")

# Se arman los histogramas a partir de los datos que supuestamente ya tenemos
# bins: cantidad de "contenedores" para dividir los datos
# density: normalizar la altura de cada contenedor (influirá si ambos trabajan con 100 datos?)
# alpha: transparencia/opacidad de las barras
plt.hist(iosDataCP, bins=10, density=False, alpha=0.5, color='r', label='Método CP')
plt.hist(iosDataSS, bins=10, density=False, alpha=0.5, color='b', label='Método SS')

# Labels
plt.xlabel('Cantidad de accesos')
plt.ylabel('Frecuencia')
plt.title('Distribución de accesos para 2^' + str(n) + ' puntos')
plt.legend()

# Mostrar el histograma
plt.show()

import matplotlib.pyplot as plt
import numpy as np
import math

# El eje X son los accesos
# El eje Y el conteo de esos accesos

x = np.array([1, 2, 3, 3, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 8, 8, 8, 9, 10])
y = np.array([0, 1, 4, 7, 10, 17, 13, 4, 0])

# Experimentación en una experimentación supongo
# Para hacer una distribución normal basada en estos valores x
mediaX = np.mean(x)
desviacionEstandarX = np.std(x)
datosNormal = np.random.normal(mediaX, desviacionEstandarX, 1000)

# Normal con media 5, desviación estándar y 10000 números generados para esta muestra
datos_normal = np.random.normal(5, 1, 10000)

# Exponente que indica la cantidad de puntos del set P
# Es para el label más que nada
n = 10

# Se arman los histogramas a partir de los datos que supuestamente ya tenemos
# bins: cantidad de "contenedores" para dividir los datos
# density: normalizar la altura de cada contenedor (influirá si ambos trabajan con 100 datos?)
# alpha: transparencia/opacidad de las barras
plt.hist(x, bins=25, density=True, alpha=0.5, color='r', label='Método CP')
plt.hist(datosNormal, bins=25, density=True, alpha=0.5, color='b', label='Método SS')

# Labels
plt.xlabel('Cantidad de accesos')
plt.ylabel('Frecuencia')
plt.title('Distribución de accesos para 2^' + str(n) + ' puntos')
plt.legend()

# Para dibujar la curva
mu, sigma = np.mean(datosNormal), np.std(datosNormal)
x = np.linspace(mu - 3*sigma, mu + 3*sigma, 100)
plt.plot(x, 1/(sigma * np.sqrt(2 * np.pi)) * np.exp(-0.5 * ((x - mu) / sigma) ** 2), color='r')

# Mostrar el histograma
plt.show()

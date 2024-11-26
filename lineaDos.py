import pandas as pd
import matplotlib.pyplot as plt


    # Leer ambos archivos CSV
archivo_csv1 = 'bs_tam_ns'
archivo_csv2 = 'gs_tam_ns'
data1 = pd.read_csv(archivo_csv1)
data2 = pd.read_csv(archivo_csv2)

# Crear el gráfico
plt.figure(figsize=(10, 6))

# Graficar los datos del primer archivo CSV
plt.plot(data1['n'], data1['t_mean'], marker='o', linestyle='-', color='b', label='Búsqueda binaria')

# Graficar los datos del segundo archivo CSV
plt.plot(data2['n'], data2['t_mean'], marker='s', linestyle='--', color='r', label='Búsqueda exponencial')

# Añadir títulos y etiquetas
plt.title('Comparación de n vs. t_mean entre dos CSV')
plt.xlabel('n')
plt.ylabel('t_mean (nano segundos)')

# Añadir leyenda
plt.legend()

# Mostrar el gráfico con cuadrícula
plt.grid(True)
plt.show()

comparar_dos_csv(archivo_csv1, archivo_csv2)

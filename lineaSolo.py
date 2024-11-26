import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Leer el archivo CSV
csv_file1 = 'graph_data1'
csv_file2 = 'graph_data2'
csv_file3 = 'graph_data3'
data1 = pd.read_csv(csv_file1)
data2 = pd.read_csv(csv_file2)
data3 = pd.read_csv(csv_file3)
# Crear el gráfico

plt.figure(figsize=(10, 6))
plt.plot(data1['n'], data1['total']/1024, marker='o', linestyle='-', color='b') #PMinHash
plt.plot(data2['n'], data2['total']/1024, marker='o', linestyle='-', color='g') #ProbMinHash3a
plt.plot(data3['n'], data3['total']/1024, marker='o', linestyle='-', color='r') #ProbMinHash4

# Añadir títulos y etiquetas
plt.title('Gráfico de muestreo vs. tamaño en bytes')
plt.xlabel('número de muestreo')
plt.ylabel('Kilobytes')

# Llamar a la función con el nombre del archivo CSV
# Cambia esto al nombre de tu archivo CSV
plt.savefig("plotxd.png")  # Save the plot as a file instead of showing it


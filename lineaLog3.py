import pandas as pd
import matplotlib.pyplot as plt


# Leer ambos archivos CSV
archivo_csv1 = 'graph_data1'
data1 = pd.read_csv(archivo_csv1)

# Crear el gráfico
plt.figure(figsize=(10, 6))

# Graficar los datos del primer archivo CSV
plt.plot(data1['n'], data1['total'], marker='o', linestyle='-', color='b', label='Búsqueda binaria')

# Añadir títulos y etiquetas
plt.title('Comparación 3 algoritmos de búsqueda escalados a log')
plt.xlabel('n')
plt.ylabel('tiempo (ns)')

# Añadir leyenda
plt.legend()

# Mostrar el gráfico con cuadrícula
#plt.grid(True)
#plt.show()
plt.savefig("plot.png")  # Save the plot as a file instead of showing it

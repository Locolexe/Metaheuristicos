import pandas as pd

#data = pd.read_csv("results_pso.csv")
#data = pd.read_csv("results_ga.csv")
data = pd.read_csv("results_de.csv")

mean = data["best"].mean()

std = data["best"].std()

print("Promedio:", mean)
print("Desviacion:", std)
import pandas as pd
import matplotlib.pyplot as plt

pso = pd.read_csv("pso.csv")
ga = pd.read_csv("ga.csv")
de = pd.read_csv("de.csv")

plt.plot(pso["iter"], pso["fitness"], label="PSO")
plt.plot(ga["iter"], ga["fitness"], label="GA")
plt.plot(de["iter"], de["fitness"], label="DE")

plt.xlabel("Iteraciones")
plt.ylabel("Fitness")

plt.title("Convergencia de algoritmos")

plt.legend()

plt.grid()

plt.show()
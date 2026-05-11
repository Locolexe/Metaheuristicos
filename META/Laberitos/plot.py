import pandas as pd
import matplotlib.pyplot as plt

ga = pd.read_csv("ga_maze.csv")
pso = pd.read_csv("pso_maze.csv")
de = pd.read_csv("de_maze.csv")

plt.plot(ga["iter"], ga["fitness"], label="GA")
plt.plot(pso["iter"], pso["fitness"], label="PSO")
plt.plot(de["iter"], de["fitness"], label="DE")

plt.xlabel("Iteraciones")
plt.ylabel("Fitness")

plt.title("Convergencia en Laberinto")

plt.legend()

plt.grid()

plt.show()
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

from palettable.tableau import Tableau_20
colors = Tableau_20.mpl_colors

params = {
   'axes.labelsize': 8,
   'font.size': 8,
   'legend.fontsize': 10,
   'xtick.labelsize': 10,
   'ytick.labelsize': 10,
   'text.usetex': False,
   'figure.figsize': [4.5, 4.5]
   }
plt.rcParams.update(params)

BASENAME = "hull_sizes_5000_30"

angle_div = [10, 15, 20, 25, 30]
steps = [i for i in range(1, 31)]

for idx, div in enumerate(angle_div):

    filepath = f"build/{BASENAME}_{div}.csv"

    df = pd.read_csv(filepath, skiprows=3)
    df = df["hull_sizes"].str.split(expand=True).astype(float)

    naive = [div**i for i in range(1, 31)]

    plt.plot(steps, df.mean(), color=colors[idx], marker="", linestyle='-', label=f"$\\gamma = {angle_div[idx]}$ (C)")
    plt.plot(steps, naive, color=colors[idx], marker="", linestyle='--', label=f"$\\gamma = {angle_div[idx]}$ (N)")

x_min = min(steps) - 1
x_max = max(steps) + 1

plt.yscale("log")
plt.xlim(x_min, x_max)
plt.ylim(1,)
plt.xlabel("Prediction Step")
plt.ylabel(r'$\operatorname{Mean}\left(\mathrm{Number\ of\ Computed\ States}\right)$')
plt.legend(bbox_to_anchor=(1.40, 1), loc="upper right", borderaxespad=0)
plt.savefig("hull_sizes.pdf", bbox_inches="tight")
plt.close()

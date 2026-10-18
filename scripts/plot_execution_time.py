import matplotlib.pyplot as plt
import pandas as pd

from palettable.colorbrewer.qualitative import Dark2_8
colors = Dark2_8.mpl_colors

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

BASENAME = "exec_time_5000"

pred_window = [10, 20, 30, 40, 50]
angle_div = [10, 15, 20, 25, 30]

execution_times = dict.fromkeys(pred_window, 0)

for idx, div in enumerate(angle_div):
    means = []
    stds = []
    for window in pred_window:
        filepath = f"build/{BASENAME}_{window}_{div}.csv"

        df = pd.read_csv(filepath, skiprows=3)
        df["t_ms"] = df["t_us"] / 1e3

        means.append(df["t_ms"].mean())
        stds.append(df["t_ms"].std())

    formatted_means = [f"{i:.1f}" for i in means]
    formatted_stds = [f"{i:.2g}" for i in stds]
    print(f"Gamma = {div}, Means = {formatted_means}, Stds = {formatted_stds}")
    plt.errorbar(pred_window, means, yerr=stds, color=colors[idx], marker='o', linestyle='-', capsize=8, label=f"$\\gamma = {angle_div[idx]}$")

x_min = min(pred_window) - 10
x_max = max(pred_window) + 10

plt.yscale("log")
plt.xlim(x_min, x_max)
plt.ylim(1e0, 1e3)
plt.xlabel("Prediction window")
plt.ylabel("Execution time (ms)")
plt.legend(loc="lower right")
plt.savefig("execution_time.pdf", bbox_inches="tight")
plt.close()

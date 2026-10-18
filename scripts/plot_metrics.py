import matplotlib.pyplot as plt
import numpy as np
import pickle

from palettable.colorbrewer.qualitative import Dark2_8
colors = Dark2_8.mpl_colors

from statsmodels.stats.proportion import proportion_confint

PREDICTION_WINDOW = [5, 10, 15]
COLOR_OFFSET = 2

params = {
   'axes.labelsize': 9,
   'font.size': 9,
   'legend.fontsize': 11,
   'xtick.labelsize': 11,
   'ytick.labelsize': 11,
   'text.usetex': False,
   'figure.figsize': [12, 12]
   }
plt.rcParams.update(params)

TOTAL_POINTS=int(1e5)

def format2print(array):
    """Formats the input to print with a maximum of 2 digits after dot.

    @param array: Array containing the metrics to be formatted.
    """
    return [f"{x*100:.2f}" for x in array]

with open("metrics.pkl", "rb") as f:
    metrics = pickle.load(f)

players = np.arange(1, 12)

c_mean_acc = np.array([])
c_std_acc = np.array([])
c_mean_ratio = np.array([])
c_std_ratio = np.array([])
for i in PREDICTION_WINDOW:
    for p in players:
        acc = metrics[i]["convexhull"][p]["accuracy"]
        ratio = metrics[i]["convexhull"][p]["ratio"]
        assert len(acc) >= TOTAL_POINTS
        assert len(ratio) >= TOTAL_POINTS
        acc = acc[:TOTAL_POINTS]
        ratio = ratio[:TOTAL_POINTS]
        c_mean_acc = np.append(c_mean_acc, np.mean(acc))
        c_std_acc = np.append(c_std_acc, np.std(acc))
        c_mean_ratio = np.append(c_mean_ratio, np.mean(ratio))
        c_std_ratio = np.append(c_std_ratio, np.std(ratio))

ca_mean_acc = np.array([])
ca_std_acc = np.array([])
ca_mean_ratio = np.array([])
ca_std_ratio = np.array([])
for i in PREDICTION_WINDOW:
    for p in players:
        acc = metrics[i]["convexhulladaptive"][p]["accuracy"]
        ratio = metrics[i]["convexhulladaptive"][p]["ratio"]
        assert len(acc) >= TOTAL_POINTS
        assert len(ratio) >= TOTAL_POINTS
        acc = acc[:TOTAL_POINTS]
        ratio = ratio[:TOTAL_POINTS]
        ca_mean_acc = np.append(ca_mean_acc, np.mean(acc))
        ca_std_acc = np.append(ca_std_acc, np.std(acc))
        ca_mean_ratio = np.append(ca_mean_ratio, np.mean(ratio))
        ca_std_ratio = np.append(ca_std_ratio, np.std(ratio))

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 12))
fig.subplots_adjust(wspace=0.25)

plot_spaces = ["   ", " ", " "]
for i in range(0, len(PREDICTION_WINDOW)):
    mean_acc = c_mean_acc[11*i:11*(i+1)]
    std_acc = c_std_acc[11*i:11*(i+1)]
    ax1.plot(players, mean_acc, marker='o', markersize=5, color=colors[COLOR_OFFSET + i], label=f"n = {PREDICTION_WINDOW[i]}{plot_spaces[i]}(C)")
    ax1.fill_between(players, np.clip(mean_acc - std_acc, 0, 1), np.clip(mean_acc + std_acc, 0, 1), color=colors[COLOR_OFFSET + i], alpha=0.12)
    print(f"c_mean_acc[n = {PREDICTION_WINDOW[i]}] = ", format2print(mean_acc))
    print(f"c_std_acc[n = {PREDICTION_WINDOW[i]}] = ", format2print(std_acc))
for i in range(0, len(PREDICTION_WINDOW)):
    mean_acc = ca_mean_acc[11*i:11*(i+1)]
    std_acc = ca_std_acc[11*i:11*(i+1)]
    print(f"ca_mean_acc[n = {PREDICTION_WINDOW[i]}] = ", format2print(mean_acc))
    print(f"ca_std_acc[n = {PREDICTION_WINDOW[i]}] = ", format2print(std_acc))
    ax1.plot(players, mean_acc, color=colors[COLOR_OFFSET + i], marker='D', markersize=5, linestyle='--', label=f"n = {PREDICTION_WINDOW[i]}{plot_spaces[i]}(CA)")
    ax1.fill_between(players, np.clip(mean_acc - std_acc, 0, 1), np.clip(mean_acc + std_acc, 0, 1), color=colors[COLOR_OFFSET + i], alpha=0.12)
ax1.text(0.02, 0.99, "(a)", transform=ax1.transAxes, fontsize=10, fontweight='bold', va='top', ha='left')
ax1.set_xticks(players)
ax1.set_xlabel("Uniform Number")
ax1.set_ylabel("Accuracy")

for i in range(0, len(PREDICTION_WINDOW)):
    mean_ratio = c_mean_ratio[11*i:11*(i+1)]
    std_ratio = c_std_ratio[11*i:11*(i+1)]
    ax2.plot(players, mean_ratio, color=colors[COLOR_OFFSET + i], marker='o', markersize=5)
    ax2.fill_between(players, mean_ratio - std_ratio, mean_ratio + std_ratio, color=colors[COLOR_OFFSET + i], alpha=0.12)
    print(f"c_mean_ratio[n = {PREDICTION_WINDOW[i]} = ", format2print(mean_ratio))
    print(f"c_std_ratio[n = {PREDICTION_WINDOW[i]} = ", format2print(std_ratio))
for i in range(0, len(PREDICTION_WINDOW)):
    mean_ratio = ca_mean_ratio[11*i:11*(i+1)]
    std_ratio = ca_std_ratio[11*i:11*(i+1)]
    ax2.plot(players, mean_ratio, color=colors[COLOR_OFFSET + i], marker='D', linestyle='--', markersize=5)
    ax2.fill_between(players, mean_ratio - std_ratio, mean_ratio + std_ratio, color=colors[COLOR_OFFSET + i], alpha=0.12)
    print(f"ca_mean_ratio[n = {PREDICTION_WINDOW[i]} = ", format2print(mean_ratio))
    print(f"ca_std_ratio[n = {PREDICTION_WINDOW[i]} = ", format2print(std_ratio))
ax2.text(0.02, 0.99, "(b)", transform=ax2.transAxes, fontsize=10, fontweight='bold', va='top', ha='left')
ax2.set_xticks(players)
ax2.set_xlabel("Uniform Number")
ax2.set_ylabel("Area Ratio (Predicted Region / Field Area)")

fig.legend(loc="upper center", bbox_to_anchor=(0.5, 0.95), ncol=2, frameon=False)
plt.savefig("metrics.pdf", bbox_inches="tight")
plt.close()

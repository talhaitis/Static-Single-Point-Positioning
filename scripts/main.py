import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import os
import numpy as np

# === Load solution file ===
file_path = os.path.join(os.path.dirname(__file__), "../result/solution.txt")
df = pd.read_csv(file_path)

# === True Position (ECEF) ===
X_true = -1641890.118
Y_true = -3664879.354
Z_true =  4939969.421

# === Plot Estimated vs True ECEF Position ===
fig, axs = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

# X
axs[0].plot(df["EpochTime"], df["X"], label="Estimated X", color="blue")
axs[0].axhline(y=X_true, color="blue", linestyle="--", label="True X")
axs[0].set_ylabel("X (m)")
axs[0].legend()
axs[0].grid(True)

# Y
axs[1].plot(df["EpochTime"], df["Y"], label="Estimated Y", color="green")
axs[1].axhline(y=Y_true, color="green", linestyle="--", label="True Y")
axs[1].set_ylabel("Y (m)")
axs[1].legend()
axs[1].grid(True)

# Z
axs[2].plot(df["EpochTime"], df["Z"], label="Estimated Z", color="red")
axs[2].axhline(y=Z_true, color="red", linestyle="--", label="True Z")
axs[2].set_xlabel("GPS Time (s of week)")
axs[2].set_ylabel("Z (m)")
axs[2].legend()
axs[2].grid(True)

fig.suptitle("Estimated vs True Receiver Position (ECEF)", fontsize=14)
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()

# === Plot Receiver Clock Bias ===
plt.figure(figsize=(8, 4))
plt.plot(df["EpochTime"], df["ClockBias"], marker='o', linestyle='-', color='purple')
plt.xlabel("GPS Time (s of week)")
plt.ylabel("Clock Bias (m)")
plt.title("Receiver Clock Bias Over Time")
plt.grid(True)
plt.tight_layout()
plt.show()

# === Plot DOPs ===
plt.figure(figsize=(10, 6))
plt.plot(df["EpochTime"], df["HDOP"], label="HDOP", color="orange")
plt.plot(df["EpochTime"], df["VDOP"], label="VDOP", color="red")
plt.plot(df["EpochTime"], df["PDOP"], label="PDOP", color="blue")
plt.plot(df["EpochTime"], df["GDOP"], label="GDOP", color="green")
plt.xlabel("GPS Time (s of week)")
plt.ylabel("DOP Value")
plt.title("DOP Values Over Time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# === Compute Uncertainty Bands (based on DOPs and assumed sigma) ===
sigma_E = 1.5   # East
sigma_N = 1.5   # North
sigma_U = 3.5   # Up  ← you can test and fine-tune this

df["EastUnc"]  = df["HDOP"] * sigma_E
df["NorthUnc"] = df["HDOP"] * sigma_N
df["UpUnc"]    = df["VDOP"] * sigma_U


# === Plot ENU Errors Separately with Uncertainty Bands ===
fig, axs = plt.subplots(3, 1, figsize=(10, 9), sharex=True)

# East
axs[0].plot(df["EpochTime"], df["EastError"], label="East Error", color="blue")
axs[0].fill_between(df["EpochTime"], -df["EastUnc"], df["EastUnc"], color="blue", alpha=0.1, label=f"±HDOP·σ={sigma_E}")
axs[0].set_ylabel("East Error (m)")
axs[0].legend()
axs[0].grid(True)

# North
axs[1].plot(df["EpochTime"], df["NorthError"], label="North Error", color="green")
axs[1].fill_between(df["EpochTime"], -df["NorthUnc"], df["NorthUnc"], color="green", alpha=0.1, label=f"±HDOP·σ={sigma_N}")
axs[1].set_ylabel("North Error (m)")
axs[1].legend()
axs[1].grid(True)

# Up
axs[2].plot(df["EpochTime"], df["UpError"], label="Up Error", color="red")
axs[2].fill_between(df["EpochTime"], -df["UpUnc"], df["UpUnc"], color="red", alpha=0.1, label=f"±VDOP·σ={sigma_U}")
axs[2].set_ylabel("Up Error (m)")
axs[2].set_xlabel("GPS Time (s of week)")
axs[2].legend()
axs[2].grid(True)

fig.suptitle("ENU Position Errors with DOP-Based Uncertainty", fontsize=14)
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()


# === Plot DOP Values and Number of Satellites Over Time ===
fig, ax1 = plt.subplots(figsize=(10, 6))

# --- Plot DOPs (left y-axis) ---
ax1.plot(df["EpochTime"], df["HDOP"], label="HDOP", color="orange")
ax1.plot(df["EpochTime"], df["VDOP"], label="VDOP", color="red")
ax1.plot(df["EpochTime"], df["PDOP"], label="PDOP", color="blue")
ax1.plot(df["EpochTime"], df["GDOP"], label="GDOP", color="green")

ax1.set_xlabel("GPS Time (s of week)")
ax1.set_ylabel("DOP Values")
ax1.set_ylim(0, max(df[["GDOP", "PDOP", "VDOP", "HDOP"]].max()) + 0.5)
ax1.grid(True)
ax1.legend(loc="upper left")

# --- Plot number of satellites (right y-axis) ---
ax2 = ax1.twinx()
ax2.plot(df["EpochTime"], df["NumSats"], label="# Satellites",
         color="black", linestyle='-', marker='o', markersize=3)

ax2.set_ylabel("Number of Satellites Used")
ax2.set_ylim(df["NumSats"].min() - 1, df["NumSats"].max() + 1)
ax2.yaxis.set_major_locator(mtick.MaxNLocator(integer=True))
ax2.legend(loc="upper right")

plt.title("DOP Values and Number of Satellites Over Time")
plt.tight_layout()
plt.show()

# === Task 3.1: Plot Histograms of Position Errors ===
fig, axs = plt.subplots(3, 1, figsize=(10, 9))
bins = 50

axs[0].hist(df["EastError"], bins=bins, color='blue', alpha=0.7)
axs[0].set_title("East Error Histogram")
axs[0].set_xlabel("Error (m)")
axs[0].set_ylabel("Frequency")
axs[0].grid(True)

axs[1].hist(df["NorthError"], bins=bins, color='green', alpha=0.7)
axs[1].set_title("North Error Histogram")
axs[1].set_xlabel("Error (m)")
axs[1].set_ylabel("Frequency")
axs[1].grid(True)

axs[2].hist(df["UpError"], bins=bins, color='red', alpha=0.7)
axs[2].set_title("Up Error Histogram")
axs[2].set_xlabel("Error (m)")
axs[2].set_ylabel("Frequency")
axs[2].grid(True)

plt.tight_layout()
plt.show()


# === Task 3.2: Compute Accuracy Statistics ===
def compute_stats(error_series):
    abs_max = np.max(np.abs(error_series))
    mean = np.mean(error_series)
    std = np.std(error_series)
    rms = np.sqrt(np.mean(np.square(error_series)))
    return abs_max, mean, std, rms

east_stats = compute_stats(df["EastError"])
north_stats = compute_stats(df["NorthError"])
up_stats = compute_stats(df["UpError"])

print("\n--- Position Error Statistics ---")
print(f"{'Axis':<6} | {'Max Abs Error':>14} | {'Mean Error':>10} | {'STD':>6} | {'RMS':>6}")
print("-" * 56)
for label, stats in zip(["East", "North", "Up"], [east_stats, north_stats, up_stats]):
    print(f"{label:<6} | {stats[0]:>14.3f} | {stats[1]:>10.3f} | {stats[2]:>6.3f} | {stats[3]:>6.3f}")
import pandas as pd
import matplotlib.pyplot as plt
import os

# === Load solution.txt ===
file_path = os.path.join(os.path.dirname(__file__), "../result/solution_with_dops.txt")
df = pd.read_csv(file_path)

# === True Position ===
X_true = -1641890.118
Y_true = -3664879.354
Z_true =  4939969.421

# === Create subplots for X, Y, Z ===
fig, axs = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

# X subplot
axs[0].plot(df["EpochTime"], df["X"], label="Estimated X", color="blue")
axs[0].axhline(y=X_true, color="blue", linestyle="--", label="True X")
axs[0].set_ylabel("X (m)")
axs[0].legend()
axs[0].grid(True)

# Y subplot
axs[1].plot(df["EpochTime"], df["Y"], label="Estimated Y", color="green")
axs[1].axhline(y=Y_true, color="green", linestyle="--", label="True Y")
axs[1].set_ylabel("Y (m)")
axs[1].legend()
axs[1].grid(True)

# Z subplot
axs[2].plot(df["EpochTime"], df["Z"], label="Estimated Z", color="red")
axs[2].axhline(y=Z_true, color="red", linestyle="--", label="True Z")
axs[2].set_xlabel("GPS Time (s of week)")
axs[2].set_ylabel("Z (m)")
axs[2].legend()
axs[2].grid(True)

# Title and layout
fig.suptitle("Estimated vs True Receiver Position (ECEF)", fontsize=14)
plt.tight_layout(rect=[0, 0, 1, 0.96])
plt.show()

# === Plot Clock Bias ===
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

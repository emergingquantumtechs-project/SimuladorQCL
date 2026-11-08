import glob
import pandas as pd
import matplotlib.pyplot as plt
import re

files = glob.glob("EnergyScan_E_*.csv")

if not files:
    print("No energy scan files found.")
    exit()

files.sort(key=lambda filename: float(re.search(r"EnergyScan_E_([-+]?\d*\.?\d+)", filename).group(1)))

plt.figure(figsize=(12, 7))

for filename in files:

    data = pd.read_csv(filename)

    x = data["x"]
    psi = data["psi"]

    energy = re.search(r"EnergyScan_E_([-+]?\d*\.?\d+)", filename).group(1)

    plt.plot(x, psi, label=f"E = {energy}")

plt.axhline(0, linestyle=":", linewidth=1)

plt.xlabel("Position x")
plt.ylabel("Wavefunction ψ(x)")
plt.title("Wavefunctions for Different Trial Energies")

plt.legend()
plt.grid(True)
plt.tight_layout()

plt.savefig("EnergyScan_Comparison.png", dpi=150)

plt.show()

import glob
import pandas as pd
import matplotlib.pyplot as plt
import os

files = sorted(glob.glob("EnergyScan_E_*.csv"))

if not files:
    print("No energy scan files found.")
    exit()

for filename in files:

    data = pd.read_csv(filename)

    energy = os.path.basename(filename)

    plt.figure(figsize=(10, 5))

    plt.plot(data["x"], data["psi"], label=energy)

    plt.axhline(0, linestyle=":", linewidth=1)

    plt.xlabel("Position x")
    plt.ylabel("Wavefunction ψ(x)")
    plt.title(f"Wavefunction - {energy}")

    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    output_name = filename.replace(".csv", ".png")
    plt.savefig(output_name, dpi=150)

    plt.close()

print("Individual graphs generated.")

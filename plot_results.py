
import csv
import matplotlib.pyplot as plt


def read_csv(filename):
    with open(filename, "r", newline="") as file:
        return list(csv.DictReader(file))


# Load the numerical wavefunction.
wave_data = read_csv("EulerBisectionWavefunction.csv")

x = [float(row["x"]) for row in wave_data]
potential = [float(row["V_x"]) for row in wave_data]
psi = [float(row["psi"]) for row in wave_data]

# Load the bisection iteration history.
history = read_csv("BisectionConvergence.csv")

iterations = [int(row["iteration"]) for row in history]
energies = [float(row["energy"]) for row in history]
residuals = [float(row["residual"]) for row in history]

# Plot the spatial solution.
fig, axes = plt.subplots(2, 1, figsize=(9, 8))

axes[0].plot(x, potential, label="Potential V(x)")
axes[0].set_title("Harmonic Oscillator Potential")
axes[0].set_xlabel("Position x")
axes[0].set_ylabel("V(x)")
axes[0].grid(True)
axes[0].legend()

axes[1].plot(x, psi, label="Euler wavefunction")
axes[1].axhline(0, linewidth=0.8)
axes[1].set_title("Wavefunction at the Computed Energy")
axes[1].set_xlabel("Position x")
axes[1].set_ylabel("psi(x)")
axes[1].grid(True)
axes[1].legend()

plt.tight_layout()
plt.savefig("EulerBisectionWavefunction.png", dpi=300)
plt.show()

# Plot the energy search.
fig, axes = plt.subplots(2, 1, figsize=(9, 8))

axes[0].plot(iterations, energies, marker="o")
axes[0].set_title("Bisection Energy Search")
axes[0].set_xlabel("Iteration")
axes[0].set_ylabel("Trial energy")
axes[0].grid(True)

axes[1].semilogy(
    iterations,
    [max(abs(value), 1e-16) for value in residuals],
    marker="o",
)
axes[1].set_title("Residual Convergence")
axes[1].set_xlabel("Iteration")
axes[1].set_ylabel("|psi(3)|")
axes[1].grid(True)

plt.tight_layout()
plt.savefig("BisectionConvergence.png", dpi=300)
plt.show()

print("Generated EulerBisectionWavefunction.png")
print("Generated BisectionConvergence.png")

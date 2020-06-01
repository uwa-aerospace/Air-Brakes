import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from pandas import DataFrame
import sys

df = pd.read_csv(sys.argv[1])

fig, ax = plt.subplots()

ax.plot(df["height"], "b", label="Height")
ax.set_ylabel("Height (m)")

ax2 = ax.twinx()
ax2.plot(df["brake-percentage"], "r", label="Brake percentage")
ax2.set_ylabel("Brake percentage")

lines = ax.get_lines() + ax2.get_lines()
ax.legend(lines, [line.get_label() for line in lines], loc='lower right')

ax.set_title("Simulated air-brake percentage vs. height")
fig.canvas.set_window_title('Air brakes simulations')
plt.show()

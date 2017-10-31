import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 1, 10)

plt.plot(x, np.sin(2 * np.pi *x), label='old')
plt.plot(x, np.sin(2 * np.pi * (x + 0.1)), label='current')
plt.xlabel('x label')
plt.ylabel('y label')

plt.legend()
plt.show()
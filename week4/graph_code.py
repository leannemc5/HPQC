#needed libraries
import matplotlib.pyplot as plt
import numpy as np

#import data
# x= size in bytes, y= one way time in seconds

x = np.array([8, 1048576, 2097152]) 
y = np.array([0.000001, 0.000127, 0.000245])

#linear fit
m, c = np.polyfit(x, y, 1)

#plot
plt.figure(figsize=(10, 6))
plt.scatter(x, y, color='red', label='Measured Data')
plt.plot(x, m*x + c, color='blue', label=f'Linear Fit: y = {m:.2e}x + {c:.2e}')

#add labels and title
plt.xlabel('Message Size (Bytes)')
plt.ylabel('One-Way Time (Seconds)')
plt.title('MPI Ping-Pong: Latency and Bandwidth Analysis')
plt.legend()
plt.grid(True)

#save image
plt.savefig('bandwidth_plot.png')
print("Graph saved as 'bandwidth_plot.png'.")
print(f"Calculated Latency (c): {c*1e6:.2f} microseconds")
print(f"Calculated Bandwidth (1/m): {(1/m)/(1024*1024):.2f} MiB/s")

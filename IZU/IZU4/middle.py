import numpy as np

points = np.array([[ 0, -1, -2],
 [-3, -1, -3],
 [ 1,  0, -3],
 [-3,  0,  0],
 [ 2, -5, -4]])

print(np.mean(points, axis=0))



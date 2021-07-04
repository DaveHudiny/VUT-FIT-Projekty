import numpy as np

points = np.array([[0,-1,-2],[-3,-1,-3],[ 1,-3,2],[-2,-2,2],[1,2,-4],
                     [0,-4,3],[1,0,-3],[-3,0,0],[-2,2,-4],[-2,4,3],
                     [3,-2,4],[2,-5,-4]])

centres = np.array([[ 0.5,  -2.75,  2.75],[ -1,   2.66667,  -1.666667],[ -0.6, -1.4 ,  -2.4]])


for i in range(3):
    for j in range(12):
        #print(points[j], centres[i])
        print('%.2f'%(np.linalg.norm(-points[j]+centres[i])), end="  ")
        #print(np.linalg.norm(-points[j]+centres[i])**2)
    print()
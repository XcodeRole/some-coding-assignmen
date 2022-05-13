import numpy as np
import matplotlib.pyplot as plt
 
n_data = 400
n_cluster = 4
 
# generate centers of clusters
centers = np.random.rand(4, 2)*5
 
EPOCH = 10
tol = 1e-5
for epoch in range(EPOCH):
    labels = np.zeros(n_data, dtype=np.int)
 
    # 计算每个点到簇中心的距离并分配label
    for i in range(n_data):
        distance = np.sum(np.square(X[i]-centers), axis=1)
        label = np.argmin(distance)
        labels[i] = label
 
    # 重新计算簇中心
    for i in range(n_cluster):
        indices = np.where(labels == i)[0]       # 找出第i簇的样本点的下标
        points = X[indices]
        centers[i, :] = np.mean(points, axis=0)  # 更新第i簇的簇中心
 
plt.scatter(X[:, 0], X[:, 1], c=labels, s=40, cmap='viridis')
plt.show()

# Optical Flow LK

# Lucas-Kanade (LK) 光流演算法推導

Lucas-Kanade 演算法是計算影像運動向量（光流）的經典方法，其核心建立在 **「亮度恆定假設」** 與 **「局部運動一致性」** 之上。

## 1. 亮度恆定假設 (Brightness Constancy)
假設影像中某個點在時間 $t$ 的位置為 $(x, y)$，其亮度值為 $I(x, y, t)$。經過微小的時間 $\Delta t$ 後，該點移動到了 $(x+\Delta x, y+\Delta y)$，但其**亮度保持不變**：

$$I(x, y, t) = I(x + \Delta x, y + \Delta y, t + \Delta t)$$

## 2. 泰勒展開與光流限制方程式
將上式右側進行一階泰勒展開（Taylor Series Expansion）：

$$I(x + \Delta x, y + \Delta y, t + \Delta t) \approx I(x, y, t) + \frac{\partial I}{\partial x}\Delta x + \frac{\partial I}{\partial y}\Delta y + \frac{\partial I}{\partial t}\Delta t$$

根據亮度恆定假設，兩側 $I(x, y, t)$ 抵消，得到：

$$\frac{\partial I}{\partial x}\Delta x + \frac{\partial I}{\partial y}\Delta y + \frac{\partial I}{\partial t}\Delta t = 0$$

將全式除以 $\Delta t$，並定義：
* $u = \frac{dx}{dt}$ ($x$ 方向的速度)
* $v = \frac{dy}{dt}$ ($y$ 方向的速度)
* $I_x, I_y$ 為空間梯度，$I_t$ 為時間梯度

得到**光流基本方程式**：
$$I_x u + I_y v + I_t = 0 \quad \Rightarrow \quad I_x u + I_y v = -I_t$$



## 3. 局部一致性假設 (Spatial Coherence)
Lucas 與 Kanade 提出：假設在一個微小的窗口 $W$（例如 $5 \times 5$ 像素）內，**所有像素具有相同的運動向量 $(u, v)$**。

若窗口內有 $n$ 個像素，則可列出 $n$ 個方程式組成的系統：
$$\begin{cases} 
I_{x1}u + I_{y1}v = -I_{t1} \\ 
I_{x2}u + I_{y2}v = -I_{t2} \\ 
\vdots \\ 
I_{xn}u + I_{yn}v = -I_{tn} 
\end{cases}$$

轉換為矩陣形式 $A \mathbf{d} = \mathbf{b}$：
$$A = \begin{bmatrix} I_{x1} & I_{y1} \\ I_{x2} & I_{y2} \\ \vdots & \vdots \\ I_{xn} & I_{yn} \end{bmatrix}, \quad \mathbf{d} = \begin{bmatrix} u \\ v \end{bmatrix}, \quad \mathbf{b} = \begin{bmatrix} -I_{t1} \\ -I_{t2} \\ \vdots \\ -I_{tn} \end{bmatrix}$$

## 4. 最小平方法求解 (Least Squares Solution)
這是一個超定系統（Overdetermined System），我們透過**最小平方法**求出使誤差平方和最小的最優解：

$$A^T A \mathbf{d} = A^T \mathbf{b}$$
$$\mathbf{d} = (A^T A)^{-1} A^T \mathbf{b}$$

其中 $A^T A$ 是一個 $2 \times 2$ 矩陣（即結構張量 Structure Tensor）：
$$A^T A = \begin{bmatrix} \sum I_{xi}^2 & \sum I_{xi}I_{yi} \\ \sum I_{xi}I_{yi} & \sum I_{yi}^2 \end{bmatrix}$$

如何呈現移動量?

這裡我使用HSV色彩空間的HUE代表方向,Saturation代表強度,Value則固定在1  

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/HSV.bmp)


# Result  

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/testopticalflowLK.bmp)

# Implement note  


# Reference  


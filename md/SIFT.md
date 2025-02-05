
# SIFT(scale-invariant feature transform)   
The SIFT algorithm is considered to be one of the algorithms in the field of feature matching.
SIFT is divided into four major parts  
1. Find extremum  from Scale-space  
2. Keypoint localization
   The SIFT algorithm aim to locate precise extrema with sub-pixel accuracy, which can be achieved through quadratic surface fitting.  
SIFT uses a second-order approximation of the Taylor expansion to represent the variation of the DoG image near the extrema:  
$$D(\mathbf{ X })=D+\frac{ \partial D^T }{\partial X}X+ \frac{1}{2}X^T\frac{\partial^2 D}{\partial X^2}X$$
 


where:  
$$X = (x,y,\sigma) $$
D is stand for DOG's extrema position in image    
$$
\frac{ \partial D }{\partial X} =\left[\begin{matrix}  \frac{       \partial D }{\partial x} \\ \frac{ \partial D }{\partial y} \\ \frac{    \partial D }{\partial \sigma} \\\end{matrix}\right]  
$$

$$ \frac{\partial^2 D}{\partial X^2}$$ is DOG's Hessian matrix  
$$
        \frac{\partial^2 D}{\partial X^2} = \left[\begin{matrix}        D_{xx}&D_{xy}&D_{x\sigma} \\ D_{yx}&D_{yy}&D_{y\sigma} \\ D_{\sigma      x}&D_{\sigma y}&D_{\sigma \sigma} \\\end{matrix}\right]
$$
extema point assume:
$$ \frac{ \partial D }{\partial X}+ \frac{\partial^2 D}{\partial X^2}\hat{X} = 0
$$
$$\hat{X} = -H^{-1}\frac{ \partial D }{\partial X}$$
$$X = X_0 + \hat{X}$$

1. Keypoint description  
2. Keypoint match


# Result  
![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/sifttest3.bmp)
  
	
# 	Implement note  
TBD



# How to improve speed  
TBD






# Reference  
[1] https://github.com/robertwgh/ezSIFT

[2] https://www.sciencedirect.com/topics/computer-science/scale-invariant-feature-transform
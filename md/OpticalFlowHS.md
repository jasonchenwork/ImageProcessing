
# Optical Flow HS  

The Horn–Schunck (HS) optical flow algorithm is a classical method used to estimate motion between consecutive image frames.  
It formulates optical flow as a global energy minimization problem, combining two main assumptions: the brightness constancy constraint (pixel intensity remains constant over time) and the spatial smoothness constraint (neighboring pixels have similar motion).  
By minimizing the total energy function that balances these two terms, the HS method computes a dense optical flow field across the entire image.  

However, while it provides smooth and globally consistent motion estimates, it can be sensitive to noise and fails in areas with large motion or brightness changes. Despite these limitations, the Horn–Schunck algorithm remains a foundational approach in computer vision and serves as the basis for many modern optical flow techniques.  
Test Image  
First Image  
![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/frame10.bmp)  
Second Image  
![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/frame11.bmp)

# Result  

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/testopticalflowHS.bmp)
  
# Implement note  


# Reference  


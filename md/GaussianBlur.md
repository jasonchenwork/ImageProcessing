


# Improve Gaussian Blur  Speed  

To improve Gaussina Blur methods:
1. Separable Convolution
2. StackBlur



# Separable Convolution  
In Gaussian Filtering, Separable Convolution improves computational efficiency by decomposing a 2D Gaussian kernel into two 1D convolutions.  

$$ image \circledast G= image \circledast G_x  \circledast  G_y$$  
where:  
G is 2D's convolution filter.
Gx and Gy is 1D convolution of G.
normal 2D convolution is $$ O(MNK^2) $$
separable convolution is $$ O(MNK) + O(MNK) $$  
my result:  
input image size: 900×600  

| windows size  | 3         | 5         | 7         | 11        | 13        | 15        | 17        | 19        | 21        |
|------------------|----------|----------|----------|----------|----------|----------|----------|----------|----------|
| **2D conv**      | 48.1300  | 107.6222 | 205.9178 | 467.5654 | 692.5340 | 908.1879 | 1209.8604 | 1512.9523 | 1916.9018 |
| **Separable conv** | 35.4557  | 49.3173  | 62.4495  | 92.7108  | 141.8746 | 130.4745 | 145.1840  | 152.9011  | 172.8537  |  
# StackBlur  
StackBlur is a technique for approximating Gaussian blur. It uses two major methods:  
1. Sliding windows  
2. Separable convolution
The pseudo code is as follows:  

    tempbuf[w*h]
    // horizontal pass  
    loop y
	    loop x
	    tempbuf[y,x] = sliding windows(x-radius~s+radius) 
	   
	 //  vertical pass
	 loop x
	   loop y  
	     tempbuf[y,x] = sliding windows(y-radius~y+radius) 
	     dst[y,x] = tempbuf[y,x] 



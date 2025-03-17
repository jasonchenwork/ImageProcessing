



# Discrete Cosine Transform  

I use DCT to denoise in image.
Pseudo code:

    loop XY direction
     copy 8x8 patch 
     do DCT
     loop 8x8 
	     if( DCT data < threshold)
	         DCT = 0
     do IDCT
     assgine new value to image data
    end loop

My result is as shown blow:  
noise image:  
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/Alley_30_noisy.bmp)  
after denoise with DCT:  
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/DCT.bmp)  
To improve DCT denoising function, I used OpenMP for parallelization and SIMD.  
|  | RAW |OpenMP For |OpenMP SIMD |OpenMP For+SIMD |
|--|--|--|--|--|
|Speed(Second)  | 5.846394346 | 2.006626839 | 5.168365941|0.367201274 |  



Simple OpenMP pseudo code:
Parallelization loop:  

    #pragma omp parallel  
    {
    #pragma omp for  
    for(...)  
	    for(...)  
	}
  
  SIMD:  
  

    #pragma omp parallel  
    {
	    #pragma omp simd  
	    c[.] = a[.] * b[.]  
    }


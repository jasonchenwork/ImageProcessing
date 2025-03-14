



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




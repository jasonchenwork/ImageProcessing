
# Histogram Equalization
What is Histogram Equalization?
Through this adjustment, the intensities can be better distributed on the histogram utilizing the full range of intensities evenly. This allows for areas of lower local contrast to gain a higher contrast. Histogram equalization accomplishes this by effectively spreading out the highly populated intensity values which are used to degrade image contrast. 

Pseudo code

    //first step (hist)
    Hist[]
    loop image
	    calculate Hist[]
	 // second step (cdf)
	loop Hist[] range
		  Hist[i] += Hist[i-1]
	// final step assign new value
	loop image
	    image[X] = (Hist[image[X]]-min(Hist)) / (max(Hist)-min(Hist))*255.0

	    

Original Image
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/InputImage01.bmp)
After Histogram Equalization
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/InputImage01Afteristogramequalization.bmp)


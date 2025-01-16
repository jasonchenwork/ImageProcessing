
# Harris Corner Detector  
Its reference can be found at this link:[WIKI](https://en.wikipedia.org/wiki/Harris_corner_detector)  

The following are the results presented by my program.  

![Origin image](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/chess.bmp)  
![Harris Corner ](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/chessAfterCornerPoint.bmp)  

Pseudo code

    //calculate differial X,Y with Sobel
    Dx = GrayImage conv mask of Sobel's X direction 
    Dy = GrayImage conv mask of Sobel's Y direction
    //calculate Dxx Dyy Dxy
    Dxx = Dx * Dx with Gassian Filter
    Dyy = Dy * Dy with Gassian Filter
    Dxy = Dx * Dy with Gassian Filter
    // calculate corner strength
    Q= Dxx*Dyy-Dxy*Dxy-alpha*(Dxx+Dyy)^2
    // find coner
    if(Q[*] > threshhold && Q[*] is local max point)
	    Q[*] is a corner point




# The non-locate mean algorithm using integral image skill improves speed.


Using the integral image can achieve more than 3x the speed of the original method.

|  |origin  |using integral image  |
|--|--|--|
|  time| 23.044s |7.454s |


 

1.origin non-locate mean algorithm 
pseudo code

    loop (image pixels)
    {
        loop (search windows)
        {
                loop (search kernel mask)
                {
                    do Mean-Squared Error
                }
            colloect weight
        }
       image[x,y] =weight * image[x,y] 
    }

2.use integral image method:
pseudo code

    declare array w
    loop (search windows)
    {
        Compute differ array between origin image and shifted image
        Compute integral image from differ array

        loop (image pixels)
        {
               Compute the weight of each pixel
        }
    }
    loop (image pixels)
    {
         image[x,y] =w[x,y] * image[x,y] 
    }


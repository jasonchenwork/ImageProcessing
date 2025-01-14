
# The non-locate mean algorithm using integral image skill improves speed

To improve non-locate mean algorithm, I use integral image to solve speed issue.
I discovered that the non-local means algorithm involves many repeated calculations of the mean squared error (MSE). Therefore, I think I can use the integral image to optimize and improve its efficiency.
![repeated calculating](https://github.com/jasonchenwork/ImageProcessing/blob/main/04denoisewithIntegralImage/repeatedcalculating.PNG)

**What is integral image?**
An integral image is also known as a summed-area table.
You can find more details in this reference:
<https://en.wikipedia.org/wiki/Summed-area_table>

**How does an integral image work?**
First, I compute integral image by calculating difference between the origin image and a shifted version of the image.
Next, I create a weight table for entire image.
Finally, I use the weight table to calculate new values in each pixel.

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

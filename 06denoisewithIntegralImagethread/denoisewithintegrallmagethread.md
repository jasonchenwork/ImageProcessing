
# The non-locate mean algorithm using integral image skill and thread improves speed

To improve non-locate mean algorithm, I combined integral image and threading to address speed issue.


|  |origin  |integral image  | threading |mix|
|--|--|--|--|--|
|  time| 23.044s |7.454s |6.950s|2.280s|


pseudo code

    declare array w
    loop (search windows) Divide into 8 parts and assign them to threads for execution.
    {
        Compute differ array between origin image and shifted image
        Compute integral image from differ array

        loop (image pixels) each thread has its own start position to avoid race conditions
        {
               Compute the weight of each pixel
        }
    }
    loop (image pixels)
    {
         image[x,y] =w[x,y] * image[x,y] 
    }

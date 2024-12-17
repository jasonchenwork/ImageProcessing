# The non-locate mean algorithm using thread improves speed

Using the thread can achieve more than 3x the speed of the original method.
![enter image description here](https://github.com/jasonchenwork/ImageProcessing/blob/main/05denoisewiththread/thread.PNG?raw=true)
|  |origin   |thread  |
|--|--|--|
| time |23.044s   |6.950s|

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

2.use thread method:

pseudo code

     loop (image pixels) Divide into 8 parts and assign them to threads for execution.
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



# The non-locate mean algorithm using integral image skill and OpenMP improves speed

To improve non-locate mean algorithm, I combined integral image and OpenMP to address speed issue.
What's OpenMP?
OpenMP is an API for Open Multi-Processing. You can use OpenMP as a replacement for threading.
OpenMP provides us with a convenient method to work with threads.

|  |origin  |integral image  |origin threading |mix (integral&thread)|origin OpenMP|integral image OpenMP|
|--|--|--|--|--|--|--|
|  time| 23.044s |7.454s |6.950s|2.280s|6.498s|2.790s|

pseudo code

    declare array w
    #pragma  omp  parallel  num_threads(8)
    {
      declare local w,integral images
      #pragma  omp  for
      loop (search windows) 
      {
          Compute differ array between origin image and shifted image
          Compute integral image from differ array
          loop (image pixels)
          {
                 Compute the weight of each pixel
          }
      }
      #pragma  omp  critical
      loop(w)
      {
       w += (local w)s
      } 
    }
    #pragma  omp  parallel  for  num_threads(8)
    loop (image pixels)
    {
         image[x,y] =w[x,y] * image[x,y] 
    }

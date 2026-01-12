


# Fast Median Filter

To improve Median Filter methods:
1. sliding window


my result:  
input image size: 900×600  

| windows size  | 7         | 9         | 11         |     
|------------------|----------|----------|----------|
| **Orgin**(ms) | 17118  | 24624 | 35109 |  
| **Fast**(ms) | 854  |  822  | 859  |   
 
 
The pseudo code is as follows:  

    hisgram[256]
    
    loop image
      // add new col hisgram
      loop new col
        hisgram[]++
      // remove old col histgram
      loop old col
        hisgram[]--
      cnt=0
      median=0
      loop i in range hisgram
        cnt+= hisgram[i]
        if cnt >= target
          median = i
          break;
      //get new median from hisgram
      dst[] = median
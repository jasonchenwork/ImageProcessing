
# Connected Component Labeling



  
There have many methods can implement  

1. One-Pass / Search-based Algorithms(Done)  
use DFS  
2. Two Pass with Union Find(Done)  
  
3. Run-Length Encoding (Done)  

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/CCL.bmp)
# Result  

![](https://github.com/jasonchenwork/ImageProcessing/blob/main/10reorganisation/img/testCCL.bmp)
  
# Implement note  
Run-Length Encoding  
Step 1: 先將每一列的二值化後的影像資料轉成 線段 資料結構
Step 2: 找出當前列與先前列 重疊的線段 如果有重疊則合併ID，無則賦予新ID  
  
在合併線段時可以使用雙指針優化  
Brute Force  
```C++
      for (int i = 0; i < prelen; i++) {
        for (int j = 0; j < curlen; j++) {
          if (isOverlap(pre[i], cur[j])) {
            uf->_union(pre[i].id, cur[j].id);
          }
        }
      }
```  

Two Pointers Method:  
```C++
      int preptr = 0;
      int curptr = 0;
      while (preptr < prelen && curptr < curlen) {
        if (isOverlap(pre[preptr], cur[curptr])) {
          uf->_union(pre[preptr].id, cur[curptr].id);
        }
        if (pre[preptr].end < cur[curptr].end) {
          preptr++;
        } else {
          curptr++;
        }
      }
```


# Reference  



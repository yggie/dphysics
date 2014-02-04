#include "react/Utilities/util_funcs.h"

#include "react/math.h"

void re::generateSortedUInts(reUInt* array, reUInt size, reUInt maxValue) {
  if (size == 1) {
    array[0] = re::randi() % maxValue;
    return;
  }
  
  reUInt i1 = 0;
  reUInt i2 = size - 1;
  reUInt v1 = re::randi() % maxValue;
  reUInt v2 = re::randi() % maxValue;
  while (v2 == v1) {
    v2 = re::randi() % maxValue;
  }
  
  if (v1 < v2) {
    array[i1] = v1;
    array[i2] = v2;
  } else {
    array[i1] = v2;
    array[i2] = v1;
  }
  
  if (size == 2) {
    return;
  }
  
  // loop for each remaining numbers
  for (reUInt i = 2; i < size; i++) {
    reUInt val = re::randi() % maxValue;
    
    // check if it is repeated at the heads
    if (val == array[i1] || val == array[i2]) {
      i--;
      continue;
    }
    
    // if possible candidate for first head
    if (val < array[i1]) {
    
      if (i1 == 0) {
        array[1] = array[0];
        array[0] = val;
        i1 = 1;
        continue;
      }
      
      bool success = false;
      
      for (reUInt j = i1; j > 0; j--) {
        if (val < array[j] && val > array[j - 1]) {
          for (reUInt k = ++i1; k > j; k--) {
            array[k] = array[k - 1];
          }
          array[j] = val;
          success = true;
          break;
        } else if (val == array[j - 1]) {
          i--;
          success = true;
          break;
        }
      }
      
      if (!success) {
        for (reUInt k = ++i1; k > 0; k--) {
          array[k] = array[k - 1];
        }
        array[0] = val;
      }
      
    } else if (val > array[i2] ) {
    
      if (i2 == 0) {
        array[size - 2] = array[size - 1];
        array[size - 1] = val;
        i2 = size - 2;
        continue;
      }
      
      bool success = false;
      
      for (reUInt j = i2; j < size - 1; j++) {
        if (val > array[j] && val < array[j + 1]) {
          for (reUInt k = --i2; k < j; k++) {
            array[k] = array[k + 1];
          }
          array[j] = val;
          success = true;
          break;
        } else if (val == array[j + 1]) {
          i--;
          success = true;
          break;
        }
      }
      
      if (!success) {
        for (reUInt k = --i2; k < size - 1; k++) {
          array[k] = array[k + 1];
        }
        array[size - 1] = val;
      }
      
    } else {
      array[++i1] = val;
    }
  }
}

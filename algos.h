/*
 * algos.h: sorting algorithms
 */

#ifndef __ALGOS_H
#define __ALGOS_H

void swap(elem *a, int n1, int n2){
  int tmp = a[n1].val;
  a[n1].val = a[n2].val;
  a[n2].val = tmp;

  a[n1].state = state_swapped;
  a[n2].state = state_swapped;
}

/***/

void bubblesort(elem *a, int n, func disp){
  int s = 1,
      j = 0,
      i;
  while(s){
    s = 0;
    j++;
    for(i=0;i<n-j;i++){
      if(a[i].val > a[i+1].val){
        swap(a, i, i+1);
        s = 1;
        disp(a, n, i, -1, 0);
        usleep(RUN_SPEED);
      }
    }
  }
}

/***/

void quicksort(elem *a, int left, int right, int len, func disp){
  int i = left,
      j = right,
      tmp;

  elem pivot = a[(left + right) / 2];

  while(i <= j){
    while(a[i].val < pivot.val){
      i++;
    }
    while(a[j].val > pivot.val){
      j--;
    }
    if(i <= j){
      swap(a, i, j);
      i++;
      j--;
      disp(a, len, pivot.val, j, 0);
      usleep(RUN_SPEED);
    }
  }

  if(left < j){
    quicksort(a, left, j, len, disp);
  }
  if(i < right){
    quicksort(a, i, right, len, disp);
  }
}

void quicksort_wrapper(elem *a, int n, func disp){
  quicksort(a, 0, n - 1, n, disp);
}

/***/

void selectionsort(elem *a, int n, func disp){
  int i, j, min, tmp;
  for(i=0;i<n-1;i++){
    min = i;
    for(j=i+1;j<n;j++){
      if(a[j].val < a[min].val){
        min = j;
      }
      a[min].state = state_pivot;
      disp(a, n, i, j, 0);
      usleep(RUN_SPEED / 2);
    }
    if(min != i){
      swap(a, min, i);
      disp(a, n, i, j, 0);
      usleep(RUN_SPEED);
    }
  }
}

/***/

void shellsort(elem *a, int n, func disp){
  int h, i, j, t;
  for(h=n;h/=2;){
    for(i=h;i<n;i++){
      t = a[i].val;
      for(j=i;j>=h&&t<a[j-h].val;j-=h){
        a[j].val = a[j-h].val;
        a[j].state = state_swapped;
        a[j-h].state = state_swapped;
        disp(a, n, i, j, 0);
        usleep(RUN_SPEED);
      }
      a[j].val = t;
    }
  }
}

/***/

int bogo_is_sorted(elem *a, int n){
  while(--n > 1){
    if(a[n].val < a[n-1].val){
      return 0;
    }
  }
  return 1;
}

void bogo_shuf(elem *a, int n){
  int i, tmp, rnd;
  for(i=0;i<n;i++){
    tmp = a[i].val;
    rnd = (int)rand()%n;
    a[i].val = a[rnd].val;
    a[rnd].val = tmp;
  }
}

void bogosort(elem *a, int n, func disp){
  int max_iter = 20;
  int iter = 0;
  while(!bogo_is_sorted(a, n) && iter < max_iter){
    iter++;
    bogo_shuf(a, n);
    disp(a, n, -1, -1, 0);
    usleep(RUN_SPEED);
  }
}

/***/

void cocktailshakersort(elem *a, int n, func disp){
  int s = 0;
  int i;
  do {
    for(i=0;i<n-1;i++){
      if(a[i].val > a[i+1].val){
        s = 1;
        swap(a, i, i+1);
        disp(a, n, i, -1, 0);
        usleep(RUN_SPEED);
      }
    }
    if(!s){
      return;
    }
    s = 0;
    for(i=n-1;i>=0;i--){
      if(a[i].val > a[i+1].val){
        s = 1;
        swap(a, i, i+1);
        disp(a, n, i, -1, 0);
        usleep(RUN_SPEED);
      }
    }
  } while(s);
}

/***/

void gnomesort(elem *a, int n, func disp){
  int i = 1,
      j = 2,
      t;

  while(i < n){
    if(a[i-1].val > a[i].val){
      swap(a, i-1, i);
      disp(a, n, i, j, 0);
      usleep(RUN_SPEED);
      if(--i){
        continue;
      }
    }
    i = j++;
  }
}

/***/

void combsort(elem *a, int n, func disp){
  int gap = n,
      s = 1,
      i, j;

  while(gap > 1 || s == 1){
    gap = gap * 10 / 13;
    if(gap == 9 || gap == 10){
      gap = 11;
    }
    if(gap < 1){
      gap = 1;
    }
    s = 0;
    for(i=0,j=gap;j<n;i++,j++){
      if(a[i].val > a[j].val){
        swap(a, i, j);
        s = 1;
      }
      disp(a, n, i, j, 0);
      usleep(RUN_SPEED);
    }
  }
}

/***/

void countingsort_zero(elem **arr, int n){
  int i;
  *arr = malloc(n * sizeof(elem));

  for(i=0;i<n;i++){
    (*arr)[i].val = 0;
  }
}

void countingsort(elem *a, int n, func disp){
  int i, j,
      c = 0,
      max = 0;
  elem *count,
       *out;

  countingsort_zero(&count, n);
  copy_arr(&a, &out, n);

  for(i=0;i<n;i++){
    count[a[i].val].val = count[a[i].val].val+1;
    if(a[i].val > max){
      max = a[i].val;
    }
    disp(a, n, i, -1, 0);
    usleep(RUN_SPEED);
  }

  for(i=0;i<=max;i++){
    for(j=1;j<=count[i].val;j++){
      out[c].val = i;
      c++;
      disp(out, n, c, -1, 0);
      usleep(RUN_SPEED);
    }
  }

  disp(out, n, -1, -1, 0);
  usleep(RUN_SPEED);

  free(count);
  free(out);
}

/***/

void gravitysort(elem *a, int n, func disp){
  int i, j, max, sum;
  elem *beads;
  
  for(i=1,max=a[0].val;i<n;i++){
    if(a[i].val > max){
      max = a[i].val;
    }
  }

  countingsort_zero(&beads, max*n);

  for(i=0;i<n;i++){
    for(j=0;j<a[i].val;j++){
      beads[i*max+j].val = 1;
    }
    disp(a, n, i, -1, 0);
    usleep(RUN_SPEED);
  }

  for(j=0;j<max;j++){
    for(sum=i=0;i<n;i++){
      sum += beads[i*max+j].val;
      beads[i*max+j].val = 0;
    }
    for(i=n-sum;i<n;i++){
      beads[i*max+j].val = 1;
    }
    disp(a, n, i, -1, 0);
    usleep(RUN_SPEED);
  }

  for(i=0;i<n;i++){
    for(j=0;j<max&&beads[i*max+j].val>0;j++){
      a[i].val = j;
      disp(a, n, i, -1, 0);
      usleep(RUN_SPEED / 2);
    }
    a[i].val = j;
    disp(a, n, i, -1, 0);
    usleep(RUN_SPEED);
  }

  disp(a, n, -1, -1, 0);
  usleep(RUN_SPEED);

  free(beads);
}

#endif

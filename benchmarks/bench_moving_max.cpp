// Benchmark on the moving max algorithm from hackerrank deque STL C++ problem between a solution with an int and a deque.
// Ran on https://quick-bench.com/ on Clang 17.0
// Results:
// 
// increasing array 5K with k=2   1.3x slower
// decreasing array 5K with k=2   1.2x faster
// random array 5K with k=2       1.3x faster
// zeros with k-1 spaced ones     1.3x faster
//
// increasing array 5K with k=20  1.8x faster
// decreasing array 5K with k=20  4.1x slower
// random array 5K with k=20      2.6x faster
// zeros with k-1 spaced ones     1.7x faster
//
// increasing array 5K with k=200  1.8x faster
// decreasing array 5K with k=200  36x slower
// random array 5K with k=200      3.9x faster
// zeros with k-1 spaced ones      1.3x faster
//
// increasing array 5K with k=2000  1.8x faster
// decreasing array 5K with k=2000  270x slower
// random array 5K with k=2000      6.7x faster
// zeros with k-1 spaced ones       1.4x faster

#include <iostream>
#include <deque>


int n = 5000;
int k = 20;
int arr[5000];



static void PrintKMaxInt(benchmark::State& state){
  // increasing array
  //for (int i = 0; i < n; i++){
  //  arr[i] = i;
  //}

  //decreasing array
  //for (int i = 0; i < n ; i++){
  //  arr[n-1-i] = i;
  //}

  //random array
  for (int i = 0; i < n ; i++){
   arr[i] = std::rand();
  }

  // zeroes with ones every k-1
  //for (int i = 0; i < n ; i++){
  //  if (i % (k-1) == 0)
  //    arr[i] = 1;
  //  else
  //    arr[i] = 0;
  //}

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {

    int max = 0;
    for (int i = 0; i < n; i++){
        // std::cout << i << "\n";
        if (i < k){
            // std::cout << "current window first " << 0 << " " << i << "\n";
            if (arr[max] < arr[i]){ // max smaller than new value
                max = i;
            }
        } else {
            // std::cout << arr[max] << " ";
            // std::cout << "current window second " << i-k+1 << " " << i << "\n";
            
            if (max <= i+1-k){ // max is out of the window compute new max in window.
                max = i+1-k;
                for (int j = i+1-k; j < i; j++){
                    // std::cout << "i: " << i << " ";
                    // std::cout << "max: " << max << ":" << arr[max] << " ";
                    // std::cout << "j: " << j << ":" << arr[j] << "\n";
                    if (arr[max] <= arr[j]){
                        max = j;
                    }
                }       
            }
            if (arr[max] < arr[i]){
                max = i;
            }
        }
    }
    //std::cout << arr[max] << "\n";

    
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(arr);
    benchmark::DoNotOptimize(max);
  }
}
BENCHMARK(PrintKMaxInt);


static void PrintKMaxDeque(benchmark::State& state){
  // increasing array
  //for (int i = 0; i < n; i++){
  //  arr[i] = i;
  //}

  //decreasing array
  //for (int i = 0; i < n ; i++){
  //  arr[n-1-i] = i;
  //}

  //random array
  for (int i = 0; i < n ; i++){
   arr[i] = std::rand();
  }

  // zeroes with ones every k-1
  //for (int i = 0; i < n ; i++){
  //  if (i % (k-1) == 0)
  //    arr[i] = 1;
  //  else
  //    arr[i] = 0;
  //}

  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
  // https://stackoverflow.com/a/12239580
    std::deque<int> Q;
    for (int i = 0; i < k; i++) {
    while (!Q.empty() && arr[i] >= arr[Q.back()])
        Q.pop_back();
      Q.push_back(i);
    }
    for (int i = k; i < n; i++) {
      //std::cout << arr[Q.front()] << " ";
      while (!Q.empty() && arr[i] >= arr[Q.back()])
        Q.pop_back();
      while (!Q.empty() && Q.front() <= i-k)
        Q.pop_front();
      Q.push_back(i);
    }
    //std::cout << arr[Q.front()] << "\n";

    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(arr);
    benchmark::DoNotOptimize(Q);
  }
}
BENCHMARK(PrintKMaxDeque);




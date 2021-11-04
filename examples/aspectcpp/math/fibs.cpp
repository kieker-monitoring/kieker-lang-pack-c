#include "../../aspectcpp/math/fibs.hh"

int fib_rec(int n) {
  switch(n) {
    case 0: 
      return 0;
      break;
    case 1:
      return 1;
      break;
    default:
      return fib_rec(n - 2) + fib_rec(n - 1);
      break;
  }
}

int fib_iter(int n) {
  int a = 1, b = 1;
  for (int i = 3; i <= n; i++) {
    int c = a + b;
    a = b;
    b = c;
  }           
  return b;
}


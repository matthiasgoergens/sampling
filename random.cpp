#include <random>
#include "stdio.h"

int main()
{
  double p = 0.5;
  // std::uniform_int<int> unif(1, 52);

  // std::tr1::geometric_distribution<int, double> geom(p);

  std::uniform_real_distribution<double> unif(0,1);
  std::geometric_distribution<int, double> g(0.5);

  std::default_random_engine re;
  re.seed(2);
  double a = unif(re);
   
  printf ("%f\n", a);
  return 0;
}

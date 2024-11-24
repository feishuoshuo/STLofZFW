#include "smallVector.h"

int main()
{
  smallVector<int> vec(10);
  for (int i = 0; i < vec.size(); i++)
    vec.push_back(std::move(i));
  cout << vec;
}
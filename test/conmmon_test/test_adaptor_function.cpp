#include "../src/iterator.h" //for ostream_iterator, istream_iterator
#include "../STL/vector.h"
#include "../src/algorithms/algorithm.h"
#include "../src/functional.h" //for multiplies, plus, less
#include <iostream>

void print(int i)
{
  cout << i << " ";
}

class Int
{
public:
  // explicit 只能修改只有一个参数的类构造函数，
  // 或是除了第一个参数外其他参数都有默认值的情况
  // 表明该构造函数是显示的而非隐式的
  // 作用是禁止类构造函数的隐式自动转换
  // implicit 表示隐式，类构造函数默认声明为隐式
  // google c++规范与effective c++都推荐使用explicit声明
  explicit Int(int i) : m_i(i) {}

  void print1() const
  {
    cout << "[" << m_i << "]";
  }

private:
  int m_i;
};

// int main()
// {
//   /**
//    * function-adapter
//    */
//   // 将outite绑定到cout，每次对outite指派一个元素，就后接一个“ ”
//   zfwstl::ostream_iterator<int> outite(std::cout, " ");

//   int ia[6] = {2, 21, 12, 7, 19, 23};
//   zfwstl::vector<int> iv(ia, ia + 6);

//   // 找出不小于12的元素个数
//   std::cout << zfwstl::count_if(iv.begin(), iv.end(),
//                                 zfwstl::not1(zfwstl::bind2nd(zfwstl::less<int>(), 12)));
//   std::cout << std::endl;

//   // 令每个元素v执行(v+2)*3然后输往outite
//   transform(iv.begin(), iv.end(), outite,
//             zfwstl::compose1(zfwstl::bind2nd(zfwstl::multiplies<int>(), 3),
//                              zfwstl::bind2nd(zfwstl::plus<int>(), 2)));
//   std::cout << std::endl;

//   // 将所有元素拷贝到outite
//   zfwstl::copy(iv.begin(), iv.end(), outite);
//   std::cout << std::endl;
//   // 1. 使用函数指针搭配stl算法
//   zfwstl::for_each(iv.begin(), iv.end(), print);
//   std::cout << std::endl;

//   // 2. 以修饰过的一般函数搭配stl算法
//   zfwstl::for_each(iv.begin(), iv.end(), zfwstl::ptr_fun(print));
//   std::cout << std::endl;

//   Int t1(3), t2(7), t3(20), t4(14), t5(68);
//   zfwstl::vector<Int> Iv;
//   Iv.push_back(t1);
//   Iv.push_back(t2);
//   Iv.push_back(t3);
//   Iv.push_back(t4);
//   Iv.push_back(t5);
//   // 3. 以修饰过的成员函数搭配stl算法
//   zfwstl::for_each(Iv.begin(), Iv.end(), zfwstl::mem_fun_ref(&Int::print1));
//   return 0;
// }
//-------------------------------------------------------
// class Shape
// {
// public:
//   virtual void display() = 0;
// };

// class Rect : public Shape
// {
// public:
//   virtual void display()
//   {
//     cout << "Rect: ";
//   }
// };

// class Circle : public Shape
// {
// public:
//   virtual void display()
//   {
//     cout << "Circle: ";
//   }
// };

// class Square : public Shape
// {
// public:
//   virtual void display()
//   {
//     cout << "Square: ";
//   }
// };

// int main()
// {
//   // STL容器只支持value semantic，不支持reference semantics
//   // vector<Shape&> V; 无法通过编译
//   zfwstl::vector<Shape *> V;
//   V.push_back(new Rect);
//   V.push_back(new Circle);
//   V.push_back(new Square);
//   V.push_back(new Circle);
//   V.push_back(new Rect);

//   // polymorphically
//   for (int i = 0; i < V.size(); ++i)
//     V[i]->display();
//   cout << endl;

//   // polymorphically
//   // TAG:std::mem_fun是一个函数适配器，它用于将类的成员函数转换为可以作为算法参数的函数对象
//   zfwstl::for_each(V.begin(), V.end(), zfwstl::mem_fun(&Shape::display));
//   cout << endl;
// }

//-------------------------------------------------------
int main()
{
  // 将outite绑定到cout，每次对outite指派一个元素，就后接一个“ ”
  zfwstl::ostream_iterator<int> outite(cout, " ");

  int ia[6] = {2, 21, 12, 7, 19, 23};
  zfwstl::vector<int> iv(ia, ia + 6);

  // 想执行v = (v+2)*3
  // for_each是non-mutating algorithm，元素内容不能更改
  // 所以执行后iv内容不变
  zfwstl::for_each(iv.begin(), iv.end(),
                   zfwstl::compose1(zfwstl::bind2nd(zfwstl::multiplies<int>(), 3),
                                    zfwstl::bind2nd(zfwstl::plus<int>(), 2)));
  zfwstl::copy(iv.begin(), iv.end(), outite);
  cout << endl;

  zfwstl::transform(iv.begin(), iv.end(), outite,
                    zfwstl::compose1(zfwstl::bind2nd(zfwstl::multiplies<int>(), 3),
                                     zfwstl::bind2nd(plus<int>(), 2)));
  cout << endl;
}
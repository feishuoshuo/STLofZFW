/**
 * 简易vector，无法实现动态扩容
 */
#include <iostream>
using namespace std;

template <class T>
class smallVector
{
public:
  typedef T value;
  typedef T *iterator;
  typedef T &reference;

protected:
  iterator m_data;
  int m_len; // 元素个数
  int pos;   // 需要修改的索引位置
  iterator start;

public:
  smallVector(int len = 0) : m_len(len), m_data(nullptr), start(nullptr), pos(0)
  {
    if (len > 0)
    {
      // 创建数组
      m_data = new value[len];
      start = m_data;
    }
  }

  void push_back(const value &x)
  {
    if (m_len != pos)
    {
      *(start + pos) = x;
      ++pos;
    }
    else
      std::cout << "数组越界！" << std::endl;
  }

  inline value pop_back()
  {
    --pos; // 查找不到就当它去除了[逻辑上消失]
    --m_len;
    return *(start + pos);
  }

  int size()
  {
    return this->m_len;
  }

  iterator begin()
  {
    return this->start;
  }

  iterator end()
  {
    return start + pos;
  }

  value &operator[](int n)
  {
    if (n < pos)
    {
      return *(start + n);
    }
    else
    {
      throw std::out_of_range("数组越界！");
    }
  }
};

template <typename T>
ostream &operator<<(ostream &cout, smallVector<T> vec)
{
  for (int i = 0; i < vec.size(); ++i)
  {
    cout << vec[i] << " ";
  }
  return cout;
}
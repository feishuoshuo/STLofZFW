#include "../STL_2/rb_tree.h"
#include "../src/functional.h"
// KeyOfValue 函数对象
template <class Key>
struct KeyOfValue
{
  const Key &operator()(const Key &val) const
  {
    return val;
  }
};
int main()
{
  /**
   * 红黑树
   */
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>> itree;
  std::cout << itree.size() << std::endl;
  // 插入唯一元素
  auto result = itree.insert_unique(17);

  // 检查插入是否成功
  if (result.second)
  {
    std::cout << "Insertion successful." << std::endl;
  }
  else
  {
    std::cout << "Insertion failed. Element already exists." << std::endl;
  }
  result = itree.insert_unique(18);
  result = itree.insert_unique(17);
  if (result.second)
  {
    std::cout << "Insertion successful." << std::endl;
  }
  else
  {
    std::cout << "Insertion failed. Element already exists." << std::endl;
  }
  result = itree.insert_unique(23);
  result = itree.insert_unique(34);

  result = itree.insert_unique(27);
  result = itree.insert_unique(15);
  result = itree.insert_unique(9);
  result = itree.insert_unique(6);
  result = itree.insert_unique(8);
  result = itree.insert_unique(5);
  result = itree.insert_unique(25);

  std::cout << itree.size() << std::endl;
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>>::iterator ite1 = itree.begin();
  zfwstl::rb_tree<int, int, KeyOfValue<int>, zfwstl::less<int>>::iterator ite2 = itree.end();
  // for (; ite1 != ite2; ++ite1)
  //   std::cout << *ite1 << ' ';
  // std::cout << std::endl;

  zfwstl::__rb_tree_base_iterator rbtite;
  for (; ite1 != ite2; ++ite1)
  {
    rbtite = zfwstl::__rb_tree_base_iterator(ite1);
    std::cout << *ite1 << "(" << rbtite.node->color << ")";
  }
  std::cout << std::endl;
  return 0;
}
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
  cout << itree.size() << endl;

  itree.insert_equal(17);
  itree.insert_equal(18);
  itree.insert_equal(23);
  itree.insert_equal(34);

  itree.insert_equal(27);
  itree.insert_equal(15);
  itree.insert_equal(9);
  itree.insert_equal(6);
  itree.insert_equal(8);
  itree.insert_equal(5);
  itree.insert_equal(25);

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
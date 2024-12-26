#include "../src/functional.h"
#include <iostream>
int main()
{
  /**
   * functor-arithmetic
   */
  // // 产生仿函数实体
  // zfwstl::plus<int> plusobj;
  // zfwstl::minus<int> minusobj;
  // zfwstl::multiplies<int> multipliesobj;
  // zfwstl::divides<int> dividesobj;
  // zfwstl::modulus<int> modulusobj;
  // zfwstl::negate<int> negateobj;

  // // 使用对象完成函数功能
  // std::cout << plusobj(3, 5) << std::endl;
  // std::cout << minusobj(3, 5) << std::endl;
  // std::cout << multipliesobj(3, 5) << std::endl;
  // std::cout << dividesobj(3, 5) << std::endl;
  // std::cout << modulusobj(3, 5) << std::endl;
  // std::cout << negateobj(3) << std::endl;

  // // 直接使用仿函数的临时对象完成函数功能
  // std::cout << zfwstl::plus<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::minus<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::multiplies<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::divides<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::modulus<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::negate<int>()(3) << std::endl;
  /**
   * functor-rational
   */
  // // 产生仿函数实体
  // zfwstl::equal_to<int> equal_to_obj;
  // zfwstl::not_equal_to<int> not_equal_to_obj;
  // zfwstl::greater<int> greater_obj;
  // zfwstl::greater_equal<int> greater_equal_obj;
  // zfwstl::less<int> less_obj;
  // zfwstl::less_equal<int> less_equal_obj;

  // // 使用对象完成函数功能
  // std::cout << equal_to_obj(3, 5) << std::endl;
  // std::cout << not_equal_to_obj(3, 5) << std::endl;
  // std::cout << greater_obj(3, 5) << std::endl;
  // std::cout << greater_equal_obj(3, 5) << std::endl;
  // std::cout << less_obj(3, 5) << std::endl;
  // std::cout << less_equal_obj(3, 5) << std::endl;

  // // 直接使用仿函数的临时对象完成函数功能
  // std::cout << zfwstl::equal_to<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::not_equal_to<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::greater<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::greater_equal<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::less<int>()(3, 5) << std::endl;
  // std::cout << zfwstl::less_equal<int>()(3, 5) << std::endl;
  /**
   * functor-logical
   */
  // 产生仿函数实体
  zfwstl::logical_and<int> and_obj;
  zfwstl::logical_or<int> or_obj;
  zfwstl::logical_not<int> not_obj;

  // 使用对象完成函数功能
  std::cout << and_obj(true, true) << std::endl;
  std::cout << or_obj(true, false) << std::endl;
  std::cout << not_obj(true) << std::endl;

  // 直接使用仿函数的临时对象完成函数功能
  std::cout << zfwstl::logical_and<int>()(true, true) << std::endl;
  std::cout << zfwstl::logical_or<int>()(true, false) << std::endl;
  std::cout << zfwstl::logical_not<int>()(true) << std::endl;
  return 0;
}
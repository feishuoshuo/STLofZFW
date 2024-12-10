#ifndef ZFWSTL_UTIL_H_
#define ZFWSTL_UTIL_H_
/**
 * 包含一些通用工具，包括 move, forward, swap 等函数，以及 pair 等
 */
#include <cstddef>       //for size_t, ptrdiff_t
#include "type_traits.h" //for remove_reference
namespace zfwstl
{
  // ====================================move====================================

  template <class T>
  typename std::remove_reference<T>::type &&move(T &&arg) noexcept
  {
    return static_cast<typename std::remove_reference<T>::type &&>(arg);
  }

  // ====================================forward====================================

  template <class T>
  T &&forward(typename std::remove_reference<T>::type &arg) noexcept
  {
    return static_cast<T &&>(arg);
  }

  template <class T>
  T &&forward(typename std::remove_reference<T>::type &&arg) noexcept
  {
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T &&>(arg);
  }
  // ====================================pair=======================================
  // TODO:pair
  /**
   * 结构体模板 : pair
   * 两个模板参数分别表示两个数据的类型
   * 用 first 和 second 来分别取出第一个数据和第二个数据
   */
  template <class Ty1, class Ty2>
  struct pair
  {
    typedef Ty1 first_type;
    typedef Ty2 second_type;

    first_type first;   // 保存第一个数据
    second_type second; // 保存第二个数据

    // default constructiable
    template <class Other1 = Ty1, class Other2 = Ty2,
              typename = typename std::enable_if<
                  std::is_default_constructible<Other1>::value &&
                      std::is_default_constructible<Other2>::value,
                  void>::type>
    constexpr pair() : first(), second() {}

    // explicit constructible for this type
    template <class U1 = Ty1, class U2 = Ty2,
              typename std::enable_if<
                  std::is_copy_constructible<U1>::value &&
                      std::is_copy_constructible<U2>::value &&
                      (!std::is_convertible<const U1 &, Ty1>::value ||
                       !std::is_convertible<const U2 &, Ty2>::value),
                  int>::type = 0>
    explicit constexpr pair(const Ty1 &a, const Ty2 &b) : first(a), second(b) {}

    pair(const pair &rhs) = default;
    pair(pair &&rhs) = default;

    // implicit constructiable for other type
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<Ty1, Other1>::value &&
                      std::is_constructible<Ty2, Other2>::value &&
                      std::is_convertible<Other1 &&, Ty1>::value &&
                      std::is_convertible<Other2 &&, Ty2>::value,
                  int>::type = 0>
    constexpr pair(Other1 &&a, Other2 &&b) : first(zfwstl::forward<Other1>(a)), second(zfwstl::forward<Other2>(b)) {}

    // explicit constructiable for other type
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<Ty1, Other1>::value &&
                      std::is_constructible<Ty2, Other2>::value &&
                      (!std::is_convertible<Other1, Ty1>::value ||
                       !std::is_convertible<Other2, Ty2>::value),
                  int>::type = 0>
    explicit constexpr pair(Other1 &&a, Other2 &&b) : first(zfwstl::forward<Other1>(a)), second(zfwstl::forward<Other2>(b)) {}

    // implicit constructiable for other pair
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<Ty1, const Other1 &>::value &&
                      std::is_constructible<Ty2, const Other2 &>::value &&
                      std::is_convertible<const Other1 &, Ty1>::value &&
                      std::is_convertible<const Other2 &, Ty2>::value,
                  int>::type = 0>
    constexpr pair(const pair<Other1, Other2> &other) : first(other.first), second(other.second) {}

    // explicit constructiable for other pair
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<Ty1, const Other1 &>::value &&
                      std::is_constructible<Ty2, const Other2 &>::value &&
                      (!std::is_convertible<const Other1 &, Ty1>::value ||
                       !std::is_convertible<const Other2 &, Ty2>::value),
                  int>::type = 0>
    explicit constexpr pair(const pair<Other1, Other2> &other) : first(other.first), second(other.second) {}

    // implicit constructiable for other pair
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<Ty1, Other1>::value &&
                      std::is_constructible<Ty2, Other2>::value &&
                      std::is_convertible<Other1, Ty1>::value &&
                      std::is_convertible<Other2, Ty2>::value,
                  int>::type = 0>
    constexpr pair(pair<Other1, Other2> &&other) : first(zfwstl::forward<Other1>(other.first)), second(zfwstl::forward<Other2>(other.second)) {}

    // explicit constructiable for other pair
    template <class Other1, class Other2,
              typename std::enable_if<
                  std::is_constructible<Ty1, Other1>::value &&
                      std::is_constructible<Ty2, Other2>::value &&
                      (!std::is_convertible<Other1, Ty1>::value ||
                       !std::is_convertible<Other2, Ty2>::value),
                  int>::type = 0>
    explicit constexpr pair(pair<Other1, Other2> &&other) : first(zfwstl::forward<Other1>(other.first)), second(zfwstl::forward<Other2>(other.second)) {}

    // copy assign for this pair
    pair &operator=(const pair &rhs)
    {
      if (this != &rhs)
      {
        first = rhs.first;
        second = rhs.second;
      }
      return *this;
    }

    // move assign for this pair
    pair &operator=(pair &&rhs)
    {
      if (this != &rhs)
      {
        first = zfwstl::move(rhs.first);
        second = zfwstl::move(rhs.second);
      }
      return *this;
    }

    // copy assign for other pair
    template <class Other1, class Other2>
    pair &operator=(const pair<Other1, Other2> &other)
    {
      first = other.first;
      second = other.second;
      return *this;
    }

    // move assign for other pair
    template <class Other1, class Other2>
    pair &operator=(pair<Other1, Other2> &&other)
    {
      first = zfwstl::forward<Other1>(other.first);
      second = zfwstl::forward<Other2>(other.second);
      return *this;
    }

    ~pair() = default;

    void swap(pair &other)
    {
      if (this != &other)
      {
        zfwstl::swap(first, other.first);
        zfwstl::swap(second, other.second);
      }
    }
  };
}
#endif // !ZFWSTL_UTIL_H_
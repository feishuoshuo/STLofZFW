#ifndef ZFWINYSTL_FUNCTIONAL_H_
#define ZFWINYSTL_FUNCTIONAL_H_
/**
 * 包含了 mystl 的函数对象与哈希函数
 */
#include <cstddef> //for size_t
namespace zfwstl
{
  // 定义一元函数的参数型别和返回值型别
  template <class Arg, class Result>
  struct unary_function
  {
    typedef Arg argument_type;  // 函数参数型别
    typedef Result result_type; // 传回值型别
  };

  // 定义二元函数的参数型别的返回值型别
  template <class Arg1, class Arg2, class Result>
  struct binary_function
  {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
  };

  //==========================函数对象===========================
  /**
   * functor-arithmetic 算术类仿函数
   */
  // 加法
  template <class T>
  struct plus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x + y; }
  };

  // 减法
  template <class T>
  struct minus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x - y; }
  };

  // 乘法
  template <class T>
  struct multiplies : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x * y; }
  };

  // 除法
  template <class T>
  struct divides : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x / y; }
  };

  // 模取
  template <class T>
  struct modulus : public binary_function<T, T, T>
  {
    T operator()(const T &x, const T &y) const { return x % y; }
  };

  // 否定
  template <class T>
  struct negate : public unary_function<T, T>
  {
    T operator()(const T &x) const { return -x; }
  };

  // ======证同元素================
  // 典型案例：RB-tree所需的KeyOfValue op, set进行实现
  template <class T>
  struct identity : public unary_function<T, T>
  {
    const T &operator()(const T &x) const { return x; }
  };

  // 选择函数(selection function):接受一个pair，传回其第一个元素
  // 典型案例：RB-tree所需的KeyOfValue op, map进行实现
  template <typename Pair>
  struct select1st : public unary_function<Pair, typename Pair::first_type>
  {
    typename Pair::first_type &operator()(const Pair &pair) const
    {
      return pair.first;
    }
  };

  // 选择函数(selection function):接受一个pair，传回其第二个元素
  template <typename Pair>
  struct select2nd : public unary_function<Pair, typename Pair::second_type>
  {
    typename Pair::second_type &operator()(const Pair &pair) const
    {
      return pair.second;
    }
  };

  // 加法的证同元素
  template <class T>
  T identity_element(plus<T>) { return T(0); }

  // 乘法的证同元素
  template <class T>
  T identity_element(multiplies<T>) { return T(1); }

  //======投射函数================
  // 投射函数:传回第一参数，忽略第二参数
  template <typename Arg1, typename Arg2>
  struct project1st : public binary_function<Arg1, Arg2, Arg1>
  {
    Arg1 operator()(const Arg1 &x, const Arg2 &) const { return x; }
  };

  // 投射函数:传回第二参数，忽略第一参数
  template <typename Arg1, typename Arg2>
  struct project2nd : public binary_function<Arg1, Arg2, Arg2>
  {
    Arg2 operator()(const Arg1 &, const Arg2 &y) const { return y; }
  };

  /**
   * functor-rational 关系运算类仿函数
   */
  // 等于
  template <class T>
  struct equal_to : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x == y; }
  };

  // 不等于
  template <class T>
  struct not_equal_to : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x != y; }
  };

  // 大于
  template <class T>
  struct greater : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x > y; }
  };

  // 小于
  template <class T>
  struct less : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x < y; }
  };

  // 大于等于
  template <class T>
  struct greater_equal : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x >= y; }
  };

  // 小于等于
  template <class T>
  struct less_equal : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x <= y; }
  };

  /**
   * functor-logical 逻辑运算类仿函数
   */
  // 逻辑与
  template <class T>
  struct logical_and : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x && y; }
  };

  // 逻辑或
  template <class T>
  struct logical_or : public binary_function<T, T, bool>
  {
    bool operator()(const T &x, const T &y) const { return x || y; }
  };

  // 逻辑非
  template <class T>
  struct logical_not : public unary_function<T, bool>
  {
    bool operator()(const T &x) const { return !x; }
  };

  /**
   * function adapter
   */
  // ==============对返回值进行逻辑否定not1, not2====================
  // 表示逻辑负值
  template <class Predicate>
  class unary_negate : public unary_function<typename Predicate::argument_type, bool>
  {
  protected:
    Predicate pred; // 内部成员
  public:
    explicit unary_negate(const Predicate &x) : pred(x) {}
    bool operator()(const typename Predicate::argument_type &x) const
    {
      return !pred(x); // 将谓词结果加上否定运算
    }
  };

  // 辅助函数 (function adapter)
  template <class Predicate>
  inline unary_negate<Predicate>
  not1(const Predicate &pred)
  {
    return unary_negate<Predicate>(pred);
  }

  // 表示逻辑负值
  template <class Predicate>
  class binary_negate : public binary_function<typename Predicate::first_argument_type,
                                               typename Predicate::second_argument_type,
                                               bool>
  {
  protected:
    Predicate pred; // 内部成员
  public:
    explicit binary_negate(const Predicate &x) : pred(x) {}
    bool operator()(const typename Predicate::first_argument_type &x,
                    const typename Predicate::second_argument_type &y) const
    {
      return !pred(x, y); // 对谓词结果加上否定运算
    }
  };

  // 辅助函数 (function adapter)
  template <class Predicate>
  inline binary_negate<Predicate>
  not2(const Predicate &pred)
  {
    return binary_negate<Predicate>(pred);
  }

  // ==============对参数进行绑定bind1st, bind2nd====================
  //  以下配接器将某个Adaptable Binary function 转换为 Unary Function
  template <class Operation>
  class binder1st : public unary_function<typename Operation::second_argument_type,
                                          typename Operation::result_type>
  {
  protected:
    Operation op;                                  // 内部成员
    typename Operation::first_argument_type value; // 内部成员
  public:
    binder1st(const Operation &x, const typename Operation::first_argument_type &y)
        : op(x), value(y) {} // 记录表达式和第一参数
    typename Operation::result_type
    operator()(const typename Operation::second_argument_type &x) const
    {
      return op(value, x); // 实际调用表达式，并将value绑定为第一参数
    }
  };

  // 辅助函数 (function adapter)
  template <class Operation, class Tp>
  inline binder1st<Operation>
  bind1st(const Operation &fn, const Tp &x)
  {
    typedef typename Operation::first_argument_type _Arg1_type;
    return binder1st<Operation>(fn, _Arg1_type(x));
  }

  // 以下配接器将某个Adaptable Binary function 转换为 Unary Function
  template <class Operation>
  class binder2nd
      : public unary_function<typename Operation::first_argument_type,
                              typename Operation::result_type>
  {
  protected:
    Operation op;
    typename Operation::second_argument_type value;

  public:
    binder2nd(const Operation &x,
              const typename Operation::second_argument_type &y)
        : op(x), value(y) {} // 记录表达式和第二参数
    typename Operation::result_type
    operator()(const typename Operation::first_argument_type &x) const
    {
      return op(x, value); // 实际调用表达式，并将value绑定为第二参数
    }
  };

  // 辅助函数 (function adapter)
  template <class Operation, class Tp>
  inline binder2nd<Operation>
  bind2nd(const Operation &fn, const Tp &x)
  {
    typedef typename Operation::second_argument_type _Arg2_type;
    return binder2nd<Operation>(fn, _Arg2_type(x));
  }

  // ==============用于函数合成compose1, compose2====================
  // unary_compose and binary_compose (extensions, not part of the standard).

  // 已知两个Adaptable Unary Functions f(), g()
  // 以下配接器用来产生一个h(), h(x) = f(g(x))
  template <class Operation1, class Operation2>
  class unary_compose
      : public unary_function<typename Operation2::argument_type,
                              typename Operation1::result_type>
  {
  protected:
    Operation1 fn1; // 内部成员
    Operation2 fn2; // 内部成员
  public:
    unary_compose(const Operation1 &x, const Operation2 &y) : fn1(x), fn2(y) {} // 记录两个表达式
    typename Operation1::result_type
    operator()(const typename Operation2::argument_type &x) const
    {
      return fn1(fn2(x)); // 函数合成
    }
  };

  // 辅助函数
  template <class Operation1, class Operation2>
  inline unary_compose<Operation1, Operation2>
  compose1(const Operation1 &fn1, const Operation2 &fn2)
  {
    return unary_compose<Operation1, Operation2>(fn1, fn2);
  }

  // 已知一个Adaptable Binary Functions f 和
  // 两个Adaptable Unary Functions g1(), g2()
  // 以下配接器用来产生一个h(), h(x) = f(g1(x), g2(x))
  template <class Operation1, class Operation2, class Operation3>
  class binary_compose : public unary_function<typename Operation2::argument_type,
                                               typename Operation1::result_type>
  {
  protected:
    // 内部成员
    Operation1 fn1;
    Operation2 fn2;
    Operation3 fn3;

  public:
    binary_compose(const Operation1 &x, const Operation2 &y, const Operation3 &z)
        : fn1(x), fn2(y), fn3(z) {} // 记录表达式
    typename Operation1::result_type
    operator()(const typename Operation2::argument_type &x) const
    {
      return fn1(fn2(x), fn3(x)); // 函数合成
    }
  };

  // 辅助函数
  template <class Operation1, class Operation2, class Operation3>
  inline binary_compose<Operation1, Operation2, Operation3>
  compose2(const Operation1 &fn1, const Operation2 &fn2, const Operation3 &fn3)
  {
    return binary_compose<Operation1, Operation2, Operation3>(fn1, fn2, fn3);
  }
  // ==============用于函数指针ptr_fun====================
  // 这种配接器adapter使我们能将一般函数当作仿函数使用
  // 以下配接器将一个一元函数指针包装起来
  // 使用仿函数时就调用函数指针
  template <class Arg, class Result>
  class pointer_to_unary_function : public unary_function<Arg, Result>
  {
  protected:
    Result (*ptr)(Arg); // 内部成员，函数指针
  public:
    pointer_to_unary_function() {}
    // 记录函数指针
    explicit pointer_to_unary_function(Result (*x)(Arg)) : ptr(x) {}

    // 通过函数指针执行函数
    Result operator()(Arg x) const { return ptr(x); }
  };

  // 辅助函数
  template <class Arg, class Result>
  inline pointer_to_unary_function<Arg, Result> ptr_fun(Result (*x)(Arg))
  {
    return pointer_to_unary_function<Arg, Result>(x);
  }

  // 以下配接器将一个二元函数指针包装起来
  // 使用仿函数时就调用函数指针
  template <class Arg1, class Arg2, class Result>
  class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result>
  {
  protected:
    Result (*ptr)(Arg1, Arg2); // 内部成员，函数指针
  public:
    pointer_to_binary_function() {}
    // 记录函数指针
    explicit pointer_to_binary_function(Result (*x)(Arg1, Arg2))
        : ptr(x) {}

    // 通过函数指针执行函数
    Result operator()(Arg1 x, Arg2 y) const
    {
      return ptr(x, y);
    }
  };

  // 辅助函数
  template <class Arg1, class Arg2, class Result>
  inline pointer_to_binary_function<Arg1, Arg2, Result>
  ptr_fun(Result (*x)(Arg1, Arg2))
  {
    return pointer_to_binary_function<Arg1, Arg2, Result>(x);
  }
  // ==============用于成员函数指针mem_fun, mem_fun_ref====================
  // NOTE: 一定要以配接器mem_fun修饰成员函数，才能被算法for_each接受
  // NOTE： STL容器只支持“实值语意”(value semantic)，不支持“引用语意”(rederence semantics)
  // 8 = 2^3个函数对象
  // 1. 无参 vs 有一个参数
  // 2. 指针调用 vs 引用调用
  // 3. const 成员函数 vs non-const 成员函数

  // 无参，指针调用，non-const
  template <class Ret, class Tp>
  class mem_fun_t : public unary_function<Tp *, Ret>
  {
  public:
    // 记录
    explicit mem_fun_t(Ret (Tp::*pf)()) : f(pf) {}
    // 调用
    Ret operator()(Tp *p) const { return (p->*f)(); }

  private:
    Ret (Tp::*f)(); // 内部成员，指向成员函数指针
  };

  // 无参，指针调用，const
  template <class Ret, class Tp>
  class const_mem_fun_t : public unary_function<const Tp *, Ret>
  {
  public:
    explicit const_mem_fun_t(Ret (Tp::*pf)() const) : f(pf) {}
    Ret operator()(const Tp *p) const { return (p->*f)(); }

  private:
    Ret (Tp::*f)() const;
  };

  // 无参，引用调用，non-const
  template <class Ret, class Tp>
  class mem_fun_ref_t : public unary_function<Tp, Ret>
  {
  public:
    explicit mem_fun_ref_t(Ret (Tp::*pf)()) : f(pf) {}
    Ret operator()(Tp &r) const { return (r.*f)(); }

  private:
    Ret (Tp::*f)();
  };

  // 无参，引用调用，const
  template <class Ret, class Tp>
  class const_mem_fun_ref_t : public unary_function<Tp, Ret>
  {
  public:
    explicit const_mem_fun_ref_t(Ret (Tp::*pf)() const) : f(pf) {}
    Ret operator()(const Tp &r) const { return (r.*f)(); }

  private:
    Ret (Tp::*f)() const;
  };

  // 一个参数，指针调用，non-const
  template <class Ret, class Tp, class Arg>
  class mem_fun1_t : public binary_function<Tp *, Arg, Ret>
  {
  public:
    explicit mem_fun1_t(Ret (Tp::*pf)(Arg)) : f(pf) {}
    Ret operator()(Tp *p, Arg x) const { return (p->*f)(x); }

  private:
    Ret (Tp::*f)(Arg);
  };

  // 一个参数，指针调用，const
  template <class Ret, class Tp, class Arg>
  class const_mem_fun1_t : public binary_function<const Tp *, Arg, Ret>
  {
  public:
    explicit const_mem_fun1_t(Ret (Tp::*pf)(Arg) const) : f(pf) {}
    Ret operator()(const Tp *p, Arg x) const
    {
      return (p->*f)(x);
    }

  private:
    Ret (Tp::*f)(Arg) const;
  };

  // 一个参数，引用调用，non-const
  template <class Ret, class Tp, class Arg>
  class mem_fun1_ref_t : public binary_function<Tp, Arg, Ret>
  {
  public:
    explicit mem_fun1_ref_t(Ret (Tp::*pf)(Arg)) : f(pf) {}
    Ret operator()(Tp &r, Arg x) const { return (r.*f)(x); }

  private:
    Ret (Tp::*f)(Arg); // 内部成员，pointer to member function
  };

  // 一个参数，引用调用，const
  template <class Ret, class Tp, class Arg>
  class const_mem_fun1_ref_t : public binary_function<Tp, Arg, Ret>
  {
  public:
    explicit const_mem_fun1_ref_t(Ret (Tp::*pf)(Arg) const) : f(pf) {}
    Ret operator()(const Tp &r, Arg x) const { return (r.*f)(x); }

  private:
    Ret (Tp::*f)(Arg) const;
  };

  // 辅助函数们
  template <class Ret, class Tp>
  inline mem_fun_t<Ret, Tp> mem_fun(Ret (Tp::*f)())
  {
    return mem_fun_t<Ret, Tp>(f);
  }

  template <class Ret, class Tp>
  inline const_mem_fun_t<Ret, Tp> mem_fun(Ret (Tp::*f)() const)
  {
    return const_mem_fun_t<Ret, Tp>(f);
  }

  template <class Ret, class Tp>
  inline mem_fun_ref_t<Ret, Tp> mem_fun_ref(Ret (Tp::*f)())
  {
    return mem_fun_ref_t<Ret, Tp>(f);
  }

  template <class Ret, class Tp>
  inline const_mem_fun_ref_t<Ret, Tp> mem_fun_ref(Ret (Tp::*f)() const)
  {
    return const_mem_fun_ref_t<Ret, Tp>(f);
  }

  // 以下可以用函数重载
  template <class Ret, class Tp, class Arg>
  inline mem_fun1_t<Ret, Tp, Arg> mem_fun(Ret (Tp::*f)(Arg))
  {
    return mem_fun1_t<Ret, Tp, Arg>(f);
  }

  template <class Ret, class Tp, class Arg>
  inline const_mem_fun1_t<Ret, Tp, Arg> mem_fun(Ret (Tp::*f)(Arg) const)
  {
    return const_mem_fun1_t<Ret, Tp, Arg>(f);
  }

  template <class Ret, class Tp, class Arg>
  inline mem_fun1_ref_t<Ret, Tp, Arg> mem_fun_ref(Ret (Tp::*f)(Arg))
  {
    return mem_fun1_ref_t<Ret, Tp, Arg>(f);
  }

  template <class Ret, class Tp, class Arg>
  inline const_mem_fun1_ref_t<Ret, Tp, Arg>
  mem_fun_ref(Ret (Tp::*f)(Arg) const)
  {
    return const_mem_fun1_ref_t<Ret, Tp, Arg>(f);
  }

  template <class Ret, class Tp, class Arg>
  inline mem_fun1_t<Ret, Tp, Arg> mem_fun1(Ret (Tp::*f)(Arg))
  {
    return mem_fun1_t<Ret, Tp, Arg>(f);
  }

  template <class Ret, class Tp, class Arg>
  inline const_mem_fun1_t<Ret, Tp, Arg> mem_fun1(Ret (Tp::*f)(Arg) const)
  {
    return const_mem_fun1_t<Ret, Tp, Arg>(f);
  }

  template <class Ret, class Tp, class Arg>
  inline mem_fun1_ref_t<Ret, Tp, Arg> mem_fun1_ref(Ret (Tp::*f)(Arg))
  {
    return mem_fun1_ref_t<Ret, Tp, Arg>(f);
  }

  template <class Ret, class Tp, class Arg>
  inline const_mem_fun1_ref_t<Ret, Tp, Arg>
  mem_fun1_ref(Ret (Tp::*f)(Arg) const)
  {
    return const_mem_fun1_ref_t<Ret, Tp, Arg>(f);
  }

  /**
   * 哈希函数对象
   */
  // 对于大部分类型，hash function 什么都不做
  template <class Key>
  struct hash
  {
  };

  // 针对指针的偏特化版本
  template <class T>
  struct hash<T *>
  {
    size_t operator()(T *p) const noexcept
    {
      return reinterpret_cast<size_t>(p);
    }
  };

  // 对于整型类型，只是返回原值
// 通过宏MYSTL_TRIVIAL_HASH_FCN简化对整型类型的哈希函数模板的特化
// noexcept 提高代码安全性。如果函数保证不抛出异常，它返回 true；如果函数可能抛出异常，它返回 false
#define MYSTL_TRIVIAL_HASH_FCN(Type)           \
  template <>                                  \
  struct hash<Type>                            \
  {                                            \
    size_t operator()(Type val) const noexcept \
    {                                          \
      return static_cast<size_t>(val);         \
    }                                          \
  };

  MYSTL_TRIVIAL_HASH_FCN(bool)

  MYSTL_TRIVIAL_HASH_FCN(char)

  MYSTL_TRIVIAL_HASH_FCN(signed char)

  MYSTL_TRIVIAL_HASH_FCN(unsigned char)

  MYSTL_TRIVIAL_HASH_FCN(wchar_t)

  MYSTL_TRIVIAL_HASH_FCN(char16_t)

  MYSTL_TRIVIAL_HASH_FCN(char32_t)

  MYSTL_TRIVIAL_HASH_FCN(short)

  MYSTL_TRIVIAL_HASH_FCN(unsigned short)

  MYSTL_TRIVIAL_HASH_FCN(int)

  MYSTL_TRIVIAL_HASH_FCN(unsigned int)

  MYSTL_TRIVIAL_HASH_FCN(long)

  MYSTL_TRIVIAL_HASH_FCN(unsigned long)

  MYSTL_TRIVIAL_HASH_FCN(long long)

  MYSTL_TRIVIAL_HASH_FCN(unsigned long long)

#undef MYSTL_TRIVIAL_HASH_FCN

  // 对于浮点数，逐位哈希
  inline size_t bitwise_hash(const unsigned char *first, size_t count)
  {
#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) && __SIZEOF_POINTER__ == 8)
    const size_t fnv_offset = 14695981039346656037ull;
    const size_t fnv_prime = 1099511628211ull;
#else
    const size_t fnv_offset = 2166136261u;
    const size_t fnv_prime = 16777619u;
#endif
    size_t result = fnv_offset;
    for (size_t i = 0; i < count; ++i)
    {
      result ^= (size_t)first[i];
      result *= fnv_prime;
    }
    return result;
  }

  template <>
  struct hash<float>
  {
    size_t operator()(const float &val)
    {
      return val == 0.0f ? 0 : bitwise_hash((const unsigned char *)&val, sizeof(float));
    }
  };

  template <>
  struct hash<double>
  {
    size_t operator()(const double &val)
    {
      return val == 0.0f ? 0 : bitwise_hash((const unsigned char *)&val, sizeof(double));
    }
  };

  template <>
  struct hash<long double>
  {
    size_t operator()(const long double &val)
    {
      return val == 0.0f ? 0 : bitwise_hash((const unsigned char *)&val, sizeof(long double));
    }
  };

}
#endif // !ZFWINYSTL_FUNCTIONAL_H_
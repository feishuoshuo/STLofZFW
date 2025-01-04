#include "../STL_2/unordered_map.h"
#include "../STL_2/unordered_set.h"
#include "../src/functional.h" //for hash
#include <iostream>
#include <string>

// 仿函数
struct eqstr
{
  bool operator()(const char *s1, const char *s2) const
  {
    return strcmp(s1, s2) == 0;
  }
};

void lookup(const zfwstl::unordered_set<const char *, zfwstl::hash<const char *>, eqstr> &Set,
            const char *word)
{
  zfwstl::unordered_set<const char *, zfwstl::hash<const char *>, eqstr>::const_iterator it = Set.find(word);
  cout << " " << word << ": " << (it != Set.end() ? "present" : "not present") << endl;
}

// int main()
// {
//   /**
//    * unordered set/map
//    */
//   zfwstl::unordered_set<const char *, zfwstl::hash<const char *>, eqstr> Set;
//   Set.insert("kiwi");
//   Set.insert("plum");
//   Set.insert("apple");
//   Set.insert("mango");
//   Set.insert("apricot");
//   Set.insert("banana");

//   auto ite1 = Set.begin();
//   auto ite2 = Set.end();

//   for (; ite1 != ite2; ++ite1)
//     cout << *ite1 << ' ';
//   cout << endl;

//   lookup(Set, "mango");
//   lookup(Set, "apple");
//   lookup(Set, "durian");

//   zfwstl::unordered_set<int> iSet;
//   iSet.insert(59);
//   iSet.insert(63);
//   iSet.insert(108);
//   iSet.insert(2);
//   iSet.insert(53);
//   iSet.insert(55);

//   auto ite11 = iSet.begin();
//   auto ite22 = iSet.end();
//   // 为何已排序：
//   // 因为bucket为193（内置28个质数中大于100的最小质数）
//   // bucket足够多，造成排序假象
//   for (; ite11 != ite22; ++ite11)
//     cout << *ite11 << ' ';
//   cout << endl;

//   iSet.clear();
//   iSet.insert(3);
//   iSet.insert(196);
//   iSet.insert(1);
//   iSet.insert(389);
//   iSet.insert(194);
//   iSet.insert(387);
//   ite11 = iSet.begin();
//   for (; ite11 != ite22; ++ite11)
//     cout << *ite11 << ' ';
//   cout << endl;
//   return 0;
// }

int main()
{
  zfwstl::unordered_map<const char *, int, hash<const char *>, eqstr> days;
  // const char *key = "january";
  days["january"] = 31;
  days["february"] = 28;
  days["march"] = 31;
  days["april"] = 30;
  days["may"] = 31;
  days["june"] = 30;
  days["july"] = 31;
  days["august"] = 31;
  days["september"] = 30;
  days["october"] = 31;
  days["november"] = 30;
  days["december"] = 31;

  cout << "september -> " << days["september"] << endl;
  cout << "june -> " << days["june"] << endl;
  cout << "february -> " << days["february"] << endl;
  cout << "december -> " << days["december"] << endl;

  auto ite1 = days.begin();
  auto ite2 = days.end();
  for (; ite1 != ite2; ++ite1)
    cout << ite1->first << " ";
}
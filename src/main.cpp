#include <cstdlib>
#include "SharedPtr.h"
#include <iostream>
using test_type = std::pair<int,int>;

int main() {
   SharedPtr<int> t = makeSharedBasic<int>(99);
   std::cout << *t << " refCount: " << t.useCount() << "\n";
   // 99 refCount: 1

   

   SharedPtr<int> a;
   a = t;
   *a = 32;
   std::cout << *a << " refCount: " << a.useCount() << "\n";
   // 32 refCount: 2

   t = makeShared<int>(42);
   std::cout << *t << " refCount: " << t.useCount() << "\n"; 

   // 42 refCount: 0
   // 32 refCount: 1

   SharedPtr<test_type> myPair = makeSharedBasic<test_type>(1,2);  
   printf("%d %d\n", myPair->first, myPair->second);

   // 1 2








   
   return EXIT_SUCCESS;
}

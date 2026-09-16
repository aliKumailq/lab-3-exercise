#include <cstdlib>
#include "SharedPtr.h"
#include <iostream>
using poop = std::pair<int,int>;

int main() {
   SharedPtr<int> t = makeSharedBasic<int>(99);
   std::cout << *t << " refCount: " << t.useCount() << "\n";
   // 99 1

   

   SharedPtr<int> a;
   a = t;
   *a = 32;
   std::cout << *a << " refCount: " << a.useCount() << "\n";
   // 32 2

   t = makeShared<int>(42);
   std::cout << *t << " refCount: " << t.useCount() << "\n"; // 42 1
   std::cout << *a << " refCount: " << a.useCount() << "\n"; // 32 1

   SharedPtr<poop> myPair = makeSharedBasic<poop>(1,2);  
   printf("%d %d\n", myPair->first, myPair->second);








   
   return EXIT_SUCCESS;
}

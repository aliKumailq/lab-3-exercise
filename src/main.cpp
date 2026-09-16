#include <cstdlib>
#include "SharedPtr.h"
#include <iostream>

int main() {
   SharedPtr<int> t = makeSharedBasic<int>(99);
   std::cout << *t << " refCount: " << t.useCount() << "\n";
   // 99 1

   

   SharedPtr<int> a;
   a = t;
   *a = 32;
   std::cout << *a << " refCount: " << a.useCount() << "\n";
   // 32 2

   t = makeSharedBasic<int>(42);
   std::cout << *t << " refCount: " << t.useCount() << "\n"; // 42 1
   std::cout << *a << " refCount: " << a.useCount() << "\n"; // 32 1






   
   return EXIT_SUCCESS;
}

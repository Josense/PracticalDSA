#include <dsa/vector.h>
#include <iostream>
#include <algorithm>

#include "include/PlayerStorageUse.h"

int main() {
    dsa::MyVector vec;
    vec.push_back(42);

    std::cout << "Application Running. Vector size: "
              << vec.data.size() << "\n";


    /** lesson 6-1 SoA 容器的定义与使用 */
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Lesson 6-1 SoA Domain-Specific Containers" << std::endl;
    usePlayerStorage();
    std::cout << "-----------------------------------------" << std::endl;
    return 0;
}

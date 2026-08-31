#include <dsa/vector.h>
#include <iostream>

int main() {
    dsa::MyVector vec;
    vec.push_back(42);

    std::cout << "Application Running. Vector size: "
              << vec.data.size() << "\n";
    return 0;
}

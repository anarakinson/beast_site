#include <iostream>
#include <bitset>
#include <cstdint>


struct A  {
    void f() { std::cout << "A::f()\n"; }
};
struct B final : public A  {
    using A::f;
    void f() {std::cout << "B::f()\n"; }
};


int main() {
    
    B b;
    A aaa = B();
    aaa.f();
    b.f();
    A* b_ptr = &b;
    b_ptr->f(); // A::f()

    // uint16_t x = 12345;
    // uint16_t x2 = 12332;
    // uint16_t y = x ^ x2;
    // uint16_t y2 = ~((x & x2) | (~x & ~x2));

    // // x          = 0101010111
    // // x + 1      = 0101011000
    // // x - 1      = 0101010110
    // // (x + 1) | x  0101011111
    // // (x - 1) & x  0101010110
    
    // bool even = x % 2;
    // bool even2 = x & 1;
    
    // std::cout << even << " " << even2 << std::endl;

    // uint16_t z = ~x;

    // std::cout << y << " " << z << " " << x+2 << " " << (y == z) << std::endl;
    // std::cout << std::bitset<16>(x) << " " << std::endl;
    // std::cout << std::bitset<16>(x2) << " " << std::endl;
    // std::cout << std::bitset<16>(y) << " " << std::endl;
    // std::cout << std::bitset<16>(y2) << " " << std::endl;
    // std::cout << std::bitset<16>(z) << " " << std::endl;
    // std::cout << (0xa == 10) << " " << (010 == 8) << " " << std::endl;

    // long long x = -1234567;
    // long long y = 4321;

    // std::cout << "x: " << x << " y: " << y << "\n";

    // x ^= y;
    // y ^= x;
    // x ^= y;

    // std::cout << "x: " << x << " y: " << y << "\n";


    const int x = 10;

    auto *x_ptr = const_cast<int*>(&x);

    *x_ptr = 20;

    std::cout << &x << " " << x_ptr << std::endl;
    std::cout << x << " " << *x_ptr;

}



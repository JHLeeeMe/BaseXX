#ifndef BASEXX_H
#define BASEXX_H


#include <iostream>

namespace BaseXX
{
namespace _64_
{
    void hello()
    {
        printf("hello, Base64!\n");
    }
}  // namespace BaseXX::_64_

namespace _32_
{
    void hello()
    {
        printf("hello, Base32!\n");
    }
}  // namespace BaseXX::_32_

namespace _16_
{
    void hello()
    {
        printf("hello, Base16!\n");
    }
}  // namespace BaseXX::_16_
}  // namespace BaseXX


#endif  // BASEXX_H

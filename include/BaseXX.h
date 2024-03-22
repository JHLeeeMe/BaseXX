/// rfc4648
/// https://datatracker.ietf.org/doc/html/rfc4648
///

#ifndef BASEXX_H
#define BASEXX_H


#include <iostream>

/// The Base 64 Alphabet Table
/// https://datatracker.ietf.org/doc/html/rfc4648#section-4
///
static const unsigned char base64_table[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',  // 24 ~ 31
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  // 32 ~ 39
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',  // 40 ~ 47
    'w', 'x', 'y', 'z', '0', '1', '2', '3',  // 48 ~ 55
    '4', '5', '6', '7', '8', '9', '+', '/',  // 56 ~ 63
};

/// The Base 64 Alphabet Table (URL and Filename safe)
/// https://datatracker.ietf.org/doc/html/rfc4648#section-5
///
static const unsigned char base64_urlsafe_table[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',  // 24 ~ 31
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  // 32 ~ 39
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',  // 40 ~ 47
    'w', 'x', 'y', 'z', '0', '1', '2', '3',  // 48 ~ 55
    '4', '5', '6', '7', '8', '9', '-', '_',  // 56 ~ 63
};

/// The Base 32 Alphabet Table
/// https://datatracker.ietf.org/doc/html/rfc4648#section-6
///
static const unsigned char base32_table[32] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
    'Y', 'Z', '2', '3', '4', '5', '6', '7',  // 24 ~ 31
};

/// The Base 32 Alphabet Table (Extended Hex)
/// https://datatracker.ietf.org/doc/html/rfc4648#section-7
///
static const unsigned char base32_hex_table[32] = {
    '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',  // 16 ~ 23
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',  // 24 ~ 31
};

/// The Base 16 Alphabet Table
/// https://datatracker.ietf.org/doc/html/rfc4648#section-8
///
static const unsigned char base16_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
};

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


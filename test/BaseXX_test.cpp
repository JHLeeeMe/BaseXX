#include <array>
#include "gtest/gtest.h"

#include "BaseXX.h"


TEST(BaseXX, _64_)
{
    ASSERT_EQ("", base64::encode(""));
    ASSERT_EQ("", base64::encode(std::string()));
    ASSERT_EQ("", base64::encode({}));

    ASSERT_EQ("XA==", base64::encode("\\"));
    ASSERT_EQ("XG4=", base64::encode("\\n"));
    ASSERT_EQ("XG5cMA==", base64::encode("\\n\\0"));
    ASSERT_EQ("IA==", base64::encode(" "));
    ASSERT_EQ("YA==", base64::encode("`"));
    ASSERT_EQ("7ZWc6riA", base64::encode("한글"));

    {  // initializer_list<unsigned char>
        ASSERT_EQ("7ZWc", base64::encode({ 0xed, 0x95, 0x9c }));  // '한'
        ASSERT_EQ("IA==", base64::encode({ ' ', }));
    }

    {  // std::array<unsigned char, N>
        std::array<unsigned char, 0> arr_empty{};
        ASSERT_EQ("", base64::encode(arr_empty));

        std::array<unsigned char, 3> arr_3{ 0xed, 0x95, 0x9c };  // '한'
        ASSERT_EQ("7ZWc", base64::encode(arr_3));

        std::array<unsigned char, 2> arr_2{ 'a', 'A' };  // "aA"
        ASSERT_EQ("YUE=", base64::encode(arr_2));
    }
    ASSERT_EQ("////", base64::encode({ 0xff, 0xff, 0xff }));
    ASSERT_EQ("____", base64::encode_urlsafe({ 0xff, 0xff, 0xff }));
}

TEST(BaseXX, _32_)
{
    ::BaseXX::_32_::hello();
    base32::hello();
}

TEST(BaseXX, _16_)
{
    ::BaseXX::_16_::hello();
    base16::hello();
}

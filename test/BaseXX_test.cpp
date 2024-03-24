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

    {  // initializer_list<uint8_t>
        ASSERT_EQ("7ZWc", base64::encode({ 0xed, 0x95, 0x9c }));  // '한'
        ASSERT_EQ("IA==", base64::encode({ ' ', }));
    }

    {  // std::array<uint8_t, N>
        std::array<uint8_t, 0> arr_empty{};
        ASSERT_EQ("", base64::encode(arr_empty));

        std::array<uint8_t, 3> arr_3{ 0xed, 0x95, 0x9c };  // '한'
        ASSERT_EQ("7ZWc", base64::encode(arr_3));

        std::array<uint8_t, 2> arr_2{ 'a', 'A' };  // "aA"
        ASSERT_EQ("YUE=", base64::encode(arr_2));
    }
    ASSERT_EQ("////", base64::encode({ 0xff, 0xff, 0xff }));
    ASSERT_EQ("____", base64::encode_urlsafe({ 0xff, 0xff, 0xff }));
}

TEST(BaseXX, _32_)
{
    { // base32
        ASSERT_EQ("", base32::encode(""));
        ASSERT_EQ("", base32::encode(std::string()));
        ASSERT_EQ("", base32::encode({}));

        ASSERT_EQ("LQ======", base32::encode("\\"));
        ASSERT_EQ("LRXA====", base32::encode("\\n"));
        ASSERT_EQ("LRXFYMA=", base32::encode("\\n\\0"));
        ASSERT_EQ("EA======", base32::encode(" "));
        ASSERT_EQ("MA======", base32::encode("`"));
        ASSERT_EQ("5WKZZ2VYQA======", base32::encode("한글"));

        {  // initializer_list<uint8_t>
            ASSERT_EQ("5WKZY===", base32::encode({ 0xed, 0x95, 0x9c }));  // '한'
            ASSERT_EQ("EA======", base32::encode({ ' ', }));
        }

        {  // std::array<uint8_t, N>
            std::array<uint8_t, 0> arr_empty{};
            ASSERT_EQ("", base32::encode(arr_empty));

            std::array<uint8_t, 3> arr_3{ 0xed, 0x95, 0x9c };  // '한'
            ASSERT_EQ("5WKZY===", base32::encode(arr_3));

            std::array<uint8_t, 2> arr_2{ 'a', 'A' };  // "aA"
            ASSERT_EQ("MFAQ====", base32::encode(arr_2));
        }
    }  // base32

    {  // base32_hex
        ASSERT_EQ("", base32::encode_hex(""));
        ASSERT_EQ("", base32::encode_hex(std::string()));
        ASSERT_EQ("", base32::encode_hex({}));

        ASSERT_EQ("BG======", base32::encode_hex("\\"));
        ASSERT_EQ("BHN0====", base32::encode_hex("\\n"));
        ASSERT_EQ("BHN5OC0=", base32::encode_hex("\\n\\0"));
        ASSERT_EQ("40======", base32::encode_hex(" "));
        ASSERT_EQ("C0======", base32::encode_hex("`"));
        ASSERT_EQ("TMAPPQLOG0======", base32::encode_hex("한글"));

        {  // initializer_list<uint8_t>
            ASSERT_EQ("TMAPO===", base32::encode_hex({ 0xed, 0x95, 0x9c }));  // '한'
            ASSERT_EQ("40======", base32::encode_hex({ ' ', }));
        }

        {  // std::array<uint8_t, N>
            std::array<uint8_t, 0> arr_empty{};
            ASSERT_EQ("", base32::encode_hex(arr_empty));

            std::array<uint8_t, 3> arr_3{ 0xed, 0x95, 0x9c };  // '한'
            ASSERT_EQ("TMAPO===", base32::encode_hex(arr_3));

            std::array<uint8_t, 2> arr_2{ 'a', 'A' };  // "aA"
            ASSERT_EQ("C50G====", base32::encode_hex(arr_2));
        }
    }  // base32_hex
}

TEST(BaseXX, _16_)
{
    ::BaseXX::_16_::hello();
    base16::hello();
}

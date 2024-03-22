#include "gtest/gtest.h"

#include "BaseXX.h"


TEST(BaseXX, _64_)
{
    ASSERT_EQ("", BaseXX::_64_::encode());
    ASSERT_EQ("", base64::encode());
    ASSERT_EQ("XA==", base64::encode("\\"));
    ASSERT_EQ("XG4=", base64::encode("\\n"));
    ASSERT_EQ("XG5cMA==", base64::encode("\\n\\0"));
    ASSERT_EQ("IA==", base64::encode(" "));
    ASSERT_EQ("YA==", base64::encode("`"));
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

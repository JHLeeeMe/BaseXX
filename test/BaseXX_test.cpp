#include "gtest/gtest.h"

#include "BaseXX.h"

TEST(Base64, encode)
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
    ASSERT_EQ("5ryi5a2X", base64::encode("漢字"));
    ASSERT_EQ("5rGJ5a2X", base64::encode("汉字"));
    ASSERT_EQ("44Gy44KJ44GM44Gq", base64::encode("ひらがな"));
    ASSERT_EQ("44Kr44K/44Kr44OK", base64::encode("カタカナ"));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("7ZWc", base64::encode({ 0xed, 0x95, 0x9c }));  // '한'
        ASSERT_EQ("IA==", base64::encode({ ' ', }));
        ASSERT_EQ("////", base64::encode({ 0xff, 0xff, 0xff }));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base64::encode(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(128);
        ASSERT_EQ("", base64::encode(vec_1));

        std::vector<uint8_t> vec_2{ 0xed, 0x95, 0x9c };  // '한'
        ASSERT_EQ("7ZWc", base64::encode(vec_2));

        std::vector<uint8_t> vec_3{ 'a', 'A' };  // "aA"
        ASSERT_EQ("YUE=", base64::encode(vec_3));
    }
}  // TEST(Base64, encode)

TEST(Base64, encode_urlsafe)
{
    ASSERT_EQ("", base64::encode_urlsafe(""));
    ASSERT_EQ("", base64::encode_urlsafe(std::string()));
    ASSERT_EQ("", base64::encode_urlsafe({}));

    ASSERT_EQ("XA==", base64::encode_urlsafe("\\"));
    ASSERT_EQ("XG4=", base64::encode_urlsafe("\\n"));
    ASSERT_EQ("XG5cMA==", base64::encode_urlsafe("\\n\\0"));
    ASSERT_EQ("IA==", base64::encode_urlsafe(" "));
    ASSERT_EQ("YA==", base64::encode_urlsafe("`"));

    ASSERT_EQ("7ZWc6riA", base64::encode_urlsafe("한글"));
    ASSERT_EQ("5ryi5a2X", base64::encode_urlsafe("漢字"));
    ASSERT_EQ("5rGJ5a2X", base64::encode_urlsafe("汉字"));
    ASSERT_EQ("44Gy44KJ44GM44Gq", base64::encode_urlsafe("ひらがな"));
    ASSERT_EQ("44Kr44K_44Kr44OK", base64::encode_urlsafe("カタカナ"));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("7ZWc", base64::encode_urlsafe({ 0xed, 0x95, 0x9c }));  // '한'
        ASSERT_EQ("IA==", base64::encode_urlsafe({ ' ', }));
        ASSERT_EQ("____", base64::encode_urlsafe({ 0xff, 0xff, 0xff }));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base64::encode_urlsafe(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(128);
        ASSERT_EQ("", base64::encode_urlsafe(vec_1));

        std::vector<uint8_t> vec_2{ 0xed, 0x95, 0x9c };  // '한'
        ASSERT_EQ("7ZWc", base64::encode_urlsafe(vec_2));

        std::vector<uint8_t> vec_3{ 'a', 'A' };  // "aA"
        ASSERT_EQ("YUE=", base64::encode_urlsafe(vec_3));
    }
}  // TEST(Base64, encode_urlsafe)

TEST(Base64, decode)
{
    ASSERT_EQ("", base64::decode(""));
    ASSERT_EQ("", base64::decode(std::string()));
    ASSERT_EQ("", base64::decode({}));

    ASSERT_EQ("\\", base64::decode("XA=="));
    ASSERT_EQ("\\n", base64::decode("XG4="));
    ASSERT_EQ("\\n\\0", base64::decode("XG5cMA=="));
    ASSERT_EQ(" ", base64::decode("IA=="));
    ASSERT_EQ("`", base64::decode("YA=="));

    ASSERT_EQ("한글", base64::decode("7ZWc6riA"));
    ASSERT_EQ("漢字", base64::decode("5ryi5a2X"));
    ASSERT_EQ("汉字", base64::decode("5rGJ5a2X"));
    ASSERT_EQ("ひらがな", base64::decode("44Gy44KJ44GM44Gq"));
    ASSERT_EQ("カタカナ", base64::decode("44Kr44K/44Kr44OK"));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("한", base64::decode({ '7', 'Z', 'W', 'c' }));  // '한'
        ASSERT_EQ("한", base64::decode({ "7ZWc" }));  // '한'
        ASSERT_EQ("\xED\x95\x9C", base64::decode({ "7ZWc" }));
        ASSERT_EQ(" ", base64::decode({ 'I', 'A', '=', '=' }));
        ASSERT_EQ("\xff\xff\xff", base64::decode({"////"}));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base64::decode(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(128);
        ASSERT_EQ("", base64::decode(vec_1));

        std::vector<uint8_t> vec_2{ '7', 'Z', 'W', 'c' };  // '한'
        ASSERT_EQ("한", base64::decode(vec_2));

        std::vector<uint8_t> vec_3{ 'Y', 'U', 'E', '=' };  // "aA"
        ASSERT_EQ("aA", base64::decode(vec_3));
    }

    {  // multiple encode & decode
        using namespace ::BaseXX::_64_;
        ASSERT_EQ("https://www.base64decode.org",
            decode(decode(
            encode(encode(
            decode(
            encode(
            decode("aHR0cHM6Ly93d3cuYmFzZTY0ZGVjb2RlLm9yZw==")))))))
        );
    }
    {  // exception
        ASSERT_THROW(base64::decode("====="), std::runtime_error);
        ASSERT_THROW(base64::decode_urlsafe({"_/_/"}), std::runtime_error);
    }
}  // TEST(Base64, decode)

TEST(Base32, encode)
{
    ASSERT_EQ("", base32::encode(""));
    ASSERT_EQ("", base32::encode(std::string()));
    ASSERT_EQ("", base32::encode({}));

    ASSERT_EQ("LQ======", base32::encode("\\"));
    ASSERT_EQ("LRXA====", base32::encode("\\n"));
    ASSERT_EQ("LRXFYMA=", base32::encode("\\n\\0"));
    ASSERT_EQ("EA======", base32::encode(" "));
    ASSERT_EQ("MA======", base32::encode("`"));

    ASSERT_EQ("5WKZZ2VYQA======", base32::encode("한글"));
    ASSERT_EQ("426KFZNNS4======", base32::encode("漢字"));
    ASSERT_EQ("42YYTZNNS4======", base32::encode("汉字"));
    ASSERT_EQ("4OA3FY4CRHRYDDHDQGVA====", base32::encode("ひらがな"));
    ASSERT_EQ("4OBKXY4CX7RYFK7DQOFA====", base32::encode("カタカナ"));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("5WKZY===", base32::encode({ 0xed, 0x95, 0x9c }));  // '한'
        ASSERT_EQ("EA======", base32::encode({ ' ', }));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base32::encode(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(128);
        ASSERT_EQ("", base32::encode(vec_1));

        std::vector<uint8_t> vec_2{ 'a', 'A' };  // "aA"
        ASSERT_EQ("MFAQ====", base32::encode(vec_2));

        std::vector<uint8_t> vec_3{ 0xed, 0x95, 0x9c };  // '한'
        ASSERT_EQ("5WKZY===", base32::encode(vec_3));
    }
}  // TEST(Base32, encode)

TEST(Base32, encode_hex)
{
    ASSERT_EQ("", base32::encode_hex(""));
    ASSERT_EQ("", base32::encode_hex(std::string()));
    ASSERT_EQ("", base32::encode_hex({}));

    ASSERT_EQ("BG======", base32::encode_hex("\\"));
    ASSERT_EQ("BHN0====", base32::encode_hex("\\n"));
    ASSERT_EQ("BHN5OC0=", base32::encode_hex("\\n\\0"));
    ASSERT_EQ("40======", base32::encode_hex(" "));
    ASSERT_EQ("C0======", base32::encode_hex("`"));

    ASSERT_EQ("TMAPPQLOG0======", base32::encode_hex("한글"));
    ASSERT_EQ("SQUA5PDDIS======", base32::encode_hex("漢字"));
    ASSERT_EQ("SQOOJPDDIS======", base32::encode_hex("汉字"));
    ASSERT_EQ("SE0R5OS2H7HO3373G6L0====", base32::encode_hex("ひらがな"));
    ASSERT_EQ("SE1ANOS2NVHO5AV3GE50====", base32::encode_hex("カタカナ"));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("TMAPO===", base32::encode_hex({ 0xed, 0x95, 0x9c }));  // '한'
        ASSERT_EQ("40======", base32::encode_hex({ ' ', }));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base32::encode_hex(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(128);
        ASSERT_EQ("", base32::encode_hex(vec_1));

        std::vector<uint8_t> vec_2{ 'a', 'A' };  // "aA"
        ASSERT_EQ("C50G====", base32::encode_hex(vec_2));

        std::vector<uint8_t> vec_3{ 0xed, 0x95, 0x9c };  // '한'
        ASSERT_EQ("TMAPO===", base32::encode_hex(vec_3));
    }
}  // TEST(Base32, encode_hex)

TEST(Base32, decode)
{
    ASSERT_EQ("", base32::decode(""));
    ASSERT_EQ("", base32::decode(std::string()));
    ASSERT_EQ("", base32::decode({}));

    ASSERT_EQ("\\", base32::decode("LQ======"));
    ASSERT_EQ("\\n", base32::decode("LRXA===="));
    ASSERT_EQ("\\n\\0", base32::decode("LRXFYMA="));
    ASSERT_EQ(" ", base32::decode("EA======"));
    ASSERT_EQ("`", base32::decode("MA======"));

    ASSERT_EQ("한글", base32::decode("5WKZZ2VYQA======"));
    ASSERT_EQ("漢字", base32::decode("426KFZNNS4======"));
    ASSERT_EQ("汉字", base32::decode("42YYTZNNS4======"));
    ASSERT_EQ("ひらがな", base32::decode("4OA3FY4CRHRYDDHDQGVA===="));
    ASSERT_EQ("カタカナ", base32::decode("4OBKXY4CX7RYFK7DQOFA===="));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("한", base32::decode({ '5', 'W', 'K', 'Z', 'Y', '=', '=', '=' }));  // '한'
        ASSERT_EQ(" ", base32::decode({ 'E', 'A', '=', '=', '=', '=', '=', '=' }));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base32::decode(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(128);
        ASSERT_EQ("", base32::decode(vec_1));

        std::vector<uint8_t> vec_2{ 'M', 'F', 'A', 'Q', '=', '=', '=', '=' };  // "aA"
        ASSERT_EQ("aA", base32::decode(vec_2));

        std::vector<uint8_t> vec_3{ '5', 'W', 'K', 'Z', 'Y', '=', '=', '=' };  // '한'
        ASSERT_EQ("한", base32::decode(vec_3));
    }

    {  // exception
        ASSERT_THROW(base32::decode("========"), std::runtime_error);
        ASSERT_THROW(base32::decode("        "), std::runtime_error);
    }
}  // TEST(Base32, decode)

TEST(Base16, encode)
{
    ASSERT_EQ("", base16::encode(""));
    ASSERT_EQ("", base16::encode(std::string()));
    ASSERT_EQ("", base16::encode({}));

    ASSERT_EQ("5C", base16::encode("\\"));
    ASSERT_EQ("5C6E", base16::encode("\\n"));
    ASSERT_EQ("5C6E5C30", base16::encode("\\n\\0"));
    ASSERT_EQ("20", base16::encode(" "));
    ASSERT_EQ("60", base16::encode("`"));

    ASSERT_EQ("ED959CEAB880", base16::encode("한글"));
    ASSERT_EQ("E6BCA2E5AD97", base16::encode("漢字"));
    ASSERT_EQ("E6B189E5AD97", base16::encode("汉字"));
    ASSERT_EQ("E381B2E38289E3818CE381AA", base16::encode("ひらがな"));
    ASSERT_EQ("E382ABE382BFE382ABE3838A", base16::encode("カタカナ"));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("ED959C", base16::encode({ 0xed, 0x95, 0x9c }));  // '한'
        ASSERT_EQ("20", base16::encode({ ' ', }));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base16::encode(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(100);
        ASSERT_EQ("", base16::encode(vec_1));

        std::vector<uint8_t> vec_2{ 'a', 'A' };  // "aA"
        ASSERT_EQ("6141", base16::encode(vec_2));

        std::vector<uint8_t> vec_3{ 0xed, 0x95, 0x9c };  // '한'
        ASSERT_EQ("ED959C", base16::encode(vec_3));
    }
}  // TEST(Base16, encode)

TEST(Base16, decode)
{
    ASSERT_EQ("", base16::decode(""));
    ASSERT_EQ("", base16::decode(std::string()));
    ASSERT_EQ("", base16::decode({}));

    ASSERT_EQ("\\", base16::decode("5C"));
    ASSERT_EQ("\\n", base16::decode("5C6E"));
    ASSERT_EQ("\\n\\0", base16::decode("5C6E5C30"));
    ASSERT_EQ(" ", base16::decode("20"));
    ASSERT_EQ("`", base16::decode("60"));

    ASSERT_EQ("한글", base16::decode("ED959CEAB880"));
    ASSERT_EQ("漢字", base16::decode("E6BCA2E5AD97"));
    ASSERT_EQ("汉字", base16::decode("E6B189E5AD97"));
    ASSERT_EQ("ひらがな", base16::decode("E381B2E38289E3818CE381AA"));
    ASSERT_EQ("カタカナ", base16::decode("E382ABE382BFE382ABE3838A"));

    {  // initializer_list<uint8_t>
        ASSERT_EQ("한", base16::decode({ 'E', 'D', '9', '5', '9', 'C' }));  // '한'
        ASSERT_EQ(" ", base16::decode({ '2', '0' }));
    }

    {  // std::vector<uint8_t>
        std::vector<uint8_t> vec_empty{};
        ASSERT_EQ("", base16::decode(vec_empty));

        std::vector<uint8_t> vec_1{};
        vec_1.reserve(100);
        ASSERT_EQ("", base16::decode(vec_1));

        std::vector<uint8_t> vec_2{ '6', '1', '4', '1' };  // "aA"
        ASSERT_EQ("aA", base16::decode(vec_2));

        std::vector<uint8_t> vec_3{ 'E', 'D', '9', '5', '9', 'C' };  // '한'
        ASSERT_EQ("한", base16::decode(vec_3));
    }

    {  // multiple encode & decode
        using namespace ::BaseXX::_16_;
        ASSERT_EQ("한글", decode(encode("한글")));
        ASSERT_EQ("ひらがな",
            decode(decode(
            encode(encode(
            decode(
            encode("ひらがな")))))));
    }

    {  // exception
        ASSERT_THROW(base16::decode("F"), std::runtime_error);
        ASSERT_THROW(base16::decode("GG"), std::runtime_error);
    }
}  // TEST(Base16, decode)


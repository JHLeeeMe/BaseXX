/// rfc4648
/// https://datatracker.ietf.org/doc/html/rfc4648
///

#ifndef BASEXX_H
#define BASEXX_H


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
    inline std::string encode_from_string(
            const std::string& str, const unsigned char* encoder_table = base64_table)
    {
        std::string encoded{};
        unsigned char arr_3[3] = {0,};
        unsigned char arr_4[4] = {0,};

        int i = 0;
        for (size_t pos = 0; pos < str.size(); pos++)
        {
            arr_3[i++] = str[pos];
            if (i == 3)
            {
                arr_4[0] = (arr_3[0] & 0b1111'1100) >> 2;
                arr_4[1] = ((arr_3[0] & 0b0000'0011) << 4) + ((arr_3[1] & 0b1111'0000) >> 4);
                arr_4[2] = ((arr_3[1] & 0b0000'1111) << 2) + ((arr_3[2] & 0b1100'0000) >> 6);
                arr_4[3] = arr_3[2] & 0b0011'1111;

                for (size_t k = 0; k < 4; k++)
                {
                    encoded += encoder_table[arr_4[k]];
                }

                i = 0;
            }
        }

        if (i)  // i == 1 or i == 2
        {
            memset(arr_3 + i, 0x00, 3 - i);

            arr_4[0] = (arr_3[0] & 0b1111'1100) >> 2;
            arr_4[1] = ((arr_3[0] & 0b0000'0011) << 4) + ((arr_3[1] & 0b1111'0000) >> 4);
            arr_4[2] = ((arr_3[1] & 0b0000'1111) << 2) + ((arr_3[2] & 0b1100'0000) >> 6);

            for (size_t j = 0; j < i + 1; j++)
            {
                encoded += encoder_table[arr_4[j]];
            }

            while (i++ < 3)
            {
                encoded += '=';
            }
        }

        return encoded;
    }

    inline std::string encode(const std::string& str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_from_string(str);
    }

    inline std::string encode_urlsafe(const std::string& str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_from_string(str, base64_urlsafe_table);
    }

    inline std::string encode(const std::initializer_list<unsigned char>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_from_string(std::string(list.begin(), list.end()));
    }

    inline std::string encode_urlsafe(const std::initializer_list<unsigned char>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_from_string(
                  std::string(list.begin(), list.end()), base64_urlsafe_table);
    }

    template<size_t N>
    inline std::string encode(const std::array<unsigned char, N>& arr)
    {
        return (arr.size() == 0)
            ? std::string("")
            : encode_from_string(std::string(arr.begin(), arr.end()));
    }

    template<size_t N>
    inline std::string encode_urlsafe(const std::array<unsigned char, N>& arr)
    {
        return (arr.size() == 0)
            ? std::string("")
            : encode_from_string(
                  std::string(arr.begin(), arr.end()), base64_urlsafe_table);
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

namespace base64 = ::BaseXX::_64_;
namespace base32 = ::BaseXX::_32_;
namespace base16 = ::BaseXX::_16_;


#endif  // BASEXX_H


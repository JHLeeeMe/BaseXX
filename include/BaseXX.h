/// rfc4648
/// https://datatracker.ietf.org/doc/html/rfc4648
///

#ifndef BASEXX_H
#define BASEXX_H


#include <cstdint>  // uint8_t
#include <functional>  // std::function
#include <stdexcept>  // std::runtime_error
#include <string>
#include <vector>
#if __cplusplus >= 201703L
#    include <string_view>
#endif  // __cplusplus >= 201703L

#define FALLTHROUGH do{} while (false)

namespace BaseXX
{
#if __cplusplus >= 201703L
    using StringType = std::string_view;
#else
    using StringType = const std::string&;
#endif  // __cplusplus >= 201703L

namespace _64_
{
    /// The Base 64 Alphabet Table
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-4
    ///
    static constexpr uint8_t encoding_table[64] = {
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
    static constexpr uint8_t urlsafe_encoding_table[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',  // 24 ~ 31
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  // 32 ~ 39
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',  // 40 ~ 47
        'w', 'x', 'y', 'z', '0', '1', '2', '3',  // 48 ~ 55
        '4', '5', '6', '7', '8', '9', '-', '_',  // 56 ~ 63
    };

    inline std::string encode_base(const char* data,
        const size_t data_len, const uint8_t* table = encoding_table)
    {
        std::string encoded{};
        encoded.reserve(data_len);

        uint8_t arr_3[3] = {0,};
        uint8_t arr_4[4] = {0,};

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            arr_3[i++] = data[pos];
            if (i == 3)
            {
                arr_4[0] = (arr_3[0] & 0xFC) >> 2;
                arr_4[1] = ((arr_3[0] & 0x03) << 4) + ((arr_3[1] & 0xF0) >> 4);
                arr_4[2] = ((arr_3[1] & 0x0F) << 2) + ((arr_3[2] & 0xC0) >> 6);
                arr_4[3] = arr_3[2] & 0x3F;

                for (size_t k = 0; k < 4; k++)
                {
                    encoded += table[arr_4[k]];
                }

                i = 0;
            }
        }

        if (i)  // i == 1 or i == 2
        {
            memset(arr_3 + i, 0x00, 3 - i);

            arr_4[0] = (arr_3[0] & 0xFC) >> 2;
            arr_4[1] = ((arr_3[0] & 0x03) << 4) + ((arr_3[1] & 0xF0) >> 4);
            arr_4[2] = (arr_3[1] & 0x0F) << 2;

            for (size_t j = 0; j < i + 1; j++)
            {
                encoded += table[arr_4[j]];
            }

            size_t tmp = encoded.length() % 4;
            while (tmp++ != 4)
            {
                encoded += '=';
            }
        }

        return encoded;
    }


    /// ========================================================================
    /// Helper Functions
    /// ========================================================================

    inline std::string encode(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.size());
    }

    inline std::string encode_urlsafe(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.size(), urlsafe_encoding_table);
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string encode_urlsafe(
        const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), urlsafe_encoding_table);
    }

    inline std::string encode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string encode_urlsafe(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), urlsafe_encoding_table);
    }
}  // namespace BaseXX::_64_

namespace _32_
{
    enum class eEncodedType
    {
        Standard = 0,
        Hex,
    };

    /// The Base 32 Alphabet Table
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-6
    ///
    static constexpr uint8_t encoding_table[32] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
        'Y', 'Z', '2', '3', '4', '5', '6', '7',  // 24 ~ 31
    };

    /// The Base 32 Alphabet Table (Extended Hex)
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-7
    ///
    static constexpr uint8_t hex_encoding_table[32] = {
        '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',  // 16 ~ 23
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',  // 24 ~ 31
    };

    inline bool is_valid_encoded_text(const char* encoded_text,
        const size_t text_len, eEncodedType encoded_type)
    {
        if (text_len % 8 != 0)
        {
            throw std::runtime_error("Invalid Base32 encoded length.");
        }

        size_t padding_cnt = 0;
        for (size_t i = text_len - 1; i >= 0; i--)
        {
            if (encoded_text[i] != '=')
            {
                break;
            }

            padding_cnt++;
        }

        // max padding count is 6
        if (padding_cnt > 6)
        {
            return false;
        }

        if (encoded_type == eEncodedType::Standard)
        {
            for (size_t i = 0; i < text_len - padding_cnt; i++)
            {
                const char c = encoded_text[i];
                if (!('A' <= c && c <= 'Z') &&
                    !('2' <= c && c <= '7'))
                {
                    return false;
                }
            }
        }
        else if (encoded_type == eEncodedType::Hex)
        {
            for (size_t i = 0; i < text_len - padding_cnt; i++)
            {
                const char c = encoded_text[i];
                if (!('0' <= c && c <= '9') &&
                    !('A' <= c && c <= 'V'))
                {
                    return false;
                }
            }
        }

        return true;
    }

    inline const uint8_t decode_char(const char c)
    {
        if (c >= 'A' && c <= 'Z')
        {
            return c - 'A';
        }
        else if (c >= '2' && c <= '7')
        {
            return 26 + (c - '2');
        }

        throw std::runtime_error("Invalid character in Base32 Encoding.");
    }

    inline const uint8_t hex_decode_char(const char c)
    {
        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
        else if (c >= 'A' && c <= 'V')
        {
            return 10 + (c - 'A');
        }

        throw std::runtime_error("Invalid character in Base32-Hex Encoding.");
    }

    inline std::string encode_base(const char* data,
        const size_t data_len, const uint8_t* table = encoding_table)
    {
        std::string encoded{};
        encoded.reserve(data_len * 2);

        uint8_t arr_5[5] = { 0, };
        uint8_t arr_8[8] = { 0, };

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            arr_5[i++] = data[pos];
            if (i == 5)
            {
                arr_8[0] = (arr_5[0] & 0xF8) >> 3;
                arr_8[1] = ((arr_5[0] & 0x07) << 2) + ((arr_5[1] & 0xC0) >> 6);
                arr_8[2] = (arr_5[1] & 0x3E) >> 1;
                arr_8[3] = ((arr_5[1] & 0x01) << 4) + ((arr_5[2] & 0xF0) >> 4);
                arr_8[4] = ((arr_5[2] & 0x0F) << 1) + ((arr_5[3] & 0x80) >> 7);
                arr_8[5] = (arr_5[3] & 0x7C) >> 2;
                arr_8[6] = ((arr_5[3] & 0x03) << 3) + ((arr_5[4] & 0xE0) >> 5);
                arr_8[7] = arr_5[4] & 0x1F;

                for (size_t k = 0; k < 8; k++)
                {
                    encoded += table[arr_8[k]];
                }
                
                i = 0;
            }
        }

        if (i)  // i == (1 ~ 4)
        {
            memset(arr_5 + i, 0x00, 5 - i);

            switch (i)
            {
            case 4:
                arr_8[6] = (arr_5[3] & 0x03) << 3;
                arr_8[5] = (arr_5[3] & 0x7C) >> 2;
                FALLTHROUGH;
            case 3:
                arr_8[4] = ((arr_5[2] & 0x0F) << 1) + ((arr_5[3] & 0x80) >> 7);
                FALLTHROUGH;
            case 2:
                arr_8[3] = ((arr_5[1] & 0x01) << 4) + ((arr_5[2] & 0xF0) >> 4);
                arr_8[2] = (arr_5[1] & 0x3E) >> 1;
                FALLTHROUGH;
            case 1:
                arr_8[1] = ((arr_5[0] & 0x07) << 2) + ((arr_5[1] & 0xC0) >> 6);
                arr_8[0] = (arr_5[0] & 0xF8) >> 3;
                FALLTHROUGH;
            default:
                break;
            }

            size_t k = 0;
            if (i == 1)
            {
                k = i + 1;
            }
            else if (i == 4)
            {
                k = i + 3;
            }
            else
            {
                k = i + 2;
            }

            for (size_t j = 0; j < k; j++)
            {
                encoded += table[arr_8[j]];
            }

            size_t tmp = encoded.length() % 8;
            while (tmp++ != 8)
            {
                encoded += '=';
            }
        }

        return encoded;
    }

    inline std::string decode_base(const char* data,
        const size_t data_len, eEncodedType encoded_type)
    {
        if (!is_valid_encoded_text(data, data_len, encoded_type))
        {
            throw std::runtime_error("Invalid Base32[-Hex] encoded text.");
        }

        std::function<const uint8_t(const char)> decode_char_func{};
        if (encoded_type == eEncodedType::Standard)
        {
            decode_char_func = &decode_char;
        }
        else
        {
            decode_char_func = &hex_decode_char;
        }

        std::string decoded{};
        decoded.reserve(data_len * 5 / 8);

        uint8_t arr_8[8] = { 0, };
        uint8_t arr_5[5] = { 0, };

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            if (data[pos] == '=')
            {
                break;
            }

            arr_8[i++] = decode_char_func(data[pos]);

            if (i == 8)
            {
                arr_5[0] = (arr_8[0] << 3) + (arr_8[1] >> 2);
                arr_5[1] = ((arr_8[1] & 0x03) << 6) +
                    (arr_8[2] << 1) + (arr_8[3] >> 4);
                arr_5[2] = ((arr_8[3] & 0x0F) << 4) + (arr_8[4] >> 1);
                arr_5[3] = ((arr_8[4] & 0x01) << 7) +
                    (arr_8[5] << 2) + (arr_8[6] >> 3);
                arr_5[4] = ((arr_8[6] & 0x07) << 5) + arr_8[7];

                for (size_t j = 0; j < 5; ++j)
                {
                    decoded.push_back(arr_5[j]);
                }

                i = 0;
            }
        }

        if (i)  // i == (1 ~ 7)
        {
            /*
            switch (i)
            {
            case 2: // +1 byte
                arr_5[0] = (arr_8[0] << 3) | (arr_8[1] >> 2);
                decoded.push_back(arr_5[0]);
                break;
            case 4: // +2 bytes
                arr_5[0] = (arr_8[0] << 3) | (arr_8[1] >> 2);
                arr_5[1] = ((arr_8[1] & 0x03) << 6) | (arr_8[2] << 1) | (arr_8[3] >> 4);
                decoded.push_back(arr_5[0]);
                decoded.push_back(arr_5[1]);
                break;
            case 5: // +2 bytes, 마지막 비트는 무시
                arr_5[0] = (arr_8[0] << 3) | (arr_8[1] >> 2);
                arr_5[1] = ((arr_8[1] & 0x03) << 6) | (arr_8[2] << 1) | (arr_8[3] >> 4);
                arr_5[2] = ((arr_8[3] & 0x0F) << 4) | (arr_8[4] >> 1);
                decoded.push_back(arr_5[0]);
                decoded.push_back(arr_5[1]);
                decoded.push_back(arr_5[2]);
                break;
            case 7: // +4 bytes, 마지막 비트는 무시
                arr_5[0] = (arr_8[0] << 3) | (arr_8[1] >> 2);
                arr_5[1] = ((arr_8[1] & 0x03) << 6) | (arr_8[2] << 1) | (arr_8[3] >> 4);
                arr_5[2] = ((arr_8[3] & 0x0F) << 4) | (arr_8[4] >> 1);
                arr_5[3] = ((arr_8[4] & 0x01) << 7) | (arr_8[5] << 2) | (arr_8[6] >> 3);
                decoded.push_back(arr_5[0]);
                decoded.push_back(arr_5[1]);
                decoded.push_back(arr_5[2]);
                decoded.push_back(arr_5[3]);
                break;
            }
            */

            size_t k = 0;
            switch (i)
            {
            case 7:
                arr_5[3] = ((arr_8[4] & 0x01) << 7) | (arr_8[5] << 2) | (arr_8[6] >> 3);
                k++;
                FALLTHROUGH;
            case 5:
                arr_5[2] = ((arr_8[3] & 0x0F) << 4) | (arr_8[4] >> 1);
                k++;
                FALLTHROUGH;
            case 4:
                arr_5[1] = ((arr_8[1] & 0x03) << 6) | (arr_8[2] << 1) | (arr_8[3] >> 4);
                k++;
                FALLTHROUGH;
            case 2:
                arr_5[0] = (arr_8[0] << 3) | (arr_8[1] >> 2);
                k++;
                FALLTHROUGH;
            default:
                break;
            }

            for (size_t idx = 0; idx < k; idx++)
            {
                decoded.push_back(arr_5[idx]);
            }
        }

        return decoded;
    }


    /// ========================================================================
    /// Helper Functions
    /// ========================================================================

    inline std::string encode(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.length());
    }

    inline std::string encode_hex(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.length(), hex_encoding_table);
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string encode_hex(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), hex_encoding_table);
    }

    inline std::string encode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string encode_hex(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), hex_encoding_table);
    }

    inline std::string decode(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size(), eEncodedType::Standard);
    }

    inline std::string decode_hex(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size(), eEncodedType::Hex);
    }

    inline std::string decode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), eEncodedType::Standard);
    }

    inline std::string decode_hex(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), eEncodedType::Hex);
    }

    inline std::string decode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), eEncodedType::Standard);
    }

    inline std::string decode_hex(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), eEncodedType::Hex);
    }
}  // namespace BaseXX::_32_

namespace _16_
{
    /// The Base 16 Alphabet Table
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-8
    ///
    static constexpr uint8_t encoding_table[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
    };

    inline const uint8_t decode_char(const char c)
    {
        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            return 10 + (c - 'A');
        }

        throw std::runtime_error("Invalid character in Base16 encoding");
    }

    inline std::string encode_base(const char* data,
        const size_t data_len, const uint8_t* table = encoding_table)
    {
        std::string encoded{};
        encoded.reserve(data_len * 2);

        uint8_t arr_2[2] = { 0, };

        for (size_t pos = 0; pos < data_len; pos++)
        {
            arr_2[0] = (data[pos] & 0xF0) >> 4;
            arr_2[1] = data[pos] & 0x0F;

            for (size_t i = 0; i < 2; i++)
            {
                encoded += table[arr_2[i]];
            }
        }

        return encoded;
    }

    inline std::string decode_base(const char* data, const size_t data_len)
    {
        if (data_len % 2 != 0)
        {
            throw std::runtime_error("Invalid Base16 encoded length");
        }

        std::string decoded{};
        decoded.reserve(data_len / 2);

        for (size_t i = 0; i < data_len; i += 2)
        {
            decoded += ((decode_char(data[i]) << 4) | decode_char(data[i + 1]));
        }

        return decoded;
    }


    /// ========================================================================
    /// Helper Functions
    /// ========================================================================

    inline std::string encode(StringType str)
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.size());
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string encode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string decode(StringType str)
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size());
    }

    inline std::string decode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string decode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }
}  // namespace BaseXX::_16_
}  // namespace BaseXX

namespace base64 = ::BaseXX::_64_;
namespace base32 = ::BaseXX::_32_;
namespace base16 = ::BaseXX::_16_;


#endif  // BASEXX_H


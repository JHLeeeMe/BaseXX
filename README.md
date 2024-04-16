# BaseXX
Header-only library for various Base Encoding & Decoding algorithms.  
includes `Base64`, `Base64-URLsafe`, `Base32`, `Base32-Hex`, and `Base16`.  
Based on [RFC4648](https://datatracker.ietf.org/doc/html/rfc4648)

### Usage
```cpp
#include "BaseXX.h"

int main()
{
    std::string str{ "Hello, World!" };

    {  // Base64[-URLsafe]
        std::string encoded_text         = base64::encode(str);
        std::string urlsafe_encoded_text = base64::encode_urlsafe(str);
        std::string decoded_text         = base64::decode(encoded_text);
        std::string urlsafe_decoded_text = base64::decode_urlsafe(urlsafe_encoded_text);
        std::cout << "Base64 Encoded Text: "         << encoded_text         << std::endl;
        std::cout << "Base64 URLsafe Encoded Text: " << urlsafe_encoded_text << std::endl;
        std::cout << "Base64 Decoded Text: "         << decoded_text         << std::endl;
        std::cout << "Base64 URLsafe Decoded Text: " << urlsafe_decoded_text << std::endl;
    }
    
    {  // Base32[-Hex]
        std::string encoded_text     = base32::encode(str);
        std::string hex_encoded_text = base32::encode_hex(str);
        std::string decoded_text     = base32::decode(encoded_text);
        std::string hex_decoded_text = base32::decode_hex(hex_encoded_text);
        std::cout << "Base32 Encoded Text: "     << encoded_text     << std::endl;
        std::cout << "Base32 Hex Encoded Text: " << hex_encoded_text << std::endl;
        std::cout << "Base32 Decoded Text: "     << decoded_text     << std::endl;
        std::cout << "Base32 Hex Decoded Text: " << hex_decoded_text << std::endl;
    }
    
    {  // Base16
        std::string encoded_text = base16::encode(str);
        std::string decoded_text = base16::decode(encoded_text);
        std::cout << "Base16 Encoded Text: " << encoded_text << std::endl;
        std::cout << "Base16 Decoded Text: " << decoded_text << std::endl;
    }

    return 0;
}
```

### Dependencies
This project uses GoogleTest (gtest) for its testing framework. GoogleTest is distributed under the BSD 3-Clause "New" or "Revised" License. For more details, see [GoogleTest's GitHub repository](https://github.com/google/googletest).

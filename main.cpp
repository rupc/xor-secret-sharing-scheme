#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <bitset>

class XORSecretShares {

    public:
        // XORSecretShares() {}
        explicit XORSecretShares(size_t n): key_size(n) {
            if (!(n == 64 || n == 128 || n == 256)) {
                std::cerr << "Only 64/128/256 possible" << std::endl;
                _init = false;
            }
            _init = true;
        }

        std::string generate_random_secret(void);
        std::string get_shadows(size_t n);
        std::vector<std::string> split(size_t n);
        int get_key_size() {return key_size;}
        // std::vector<std::string> split(); // splits secret into shadows

    private:
        int key_size;
        bool _init;
        std::string secret;
        std::bitset<64> secret_64;
        std::bitset<128> secret_128;
        std::bitset<256> secret_256;
};

std::string XORSecretShares::generate_random_secret(void)
{
    if (_init == false) {
        return "ERROR";
    }

    std::string randSecret(key_size, '0');

    for (int i = 0; i < key_size; i++) {
        randSecret[i] = std::to_string(rand() % 2)[0];
    }

    std::string res;
    switch (key_size) {
        case 64: {
            std::bitset<64> _secret_64(randSecret);
            res = _secret_64.to_string();
        }
            break;
        case 128: {
            std::bitset<128> _secret_128(randSecret);
            res = _secret_128.to_string();
        }
            break;
        case 256: {
            std::bitset<256> _secret_256(randSecret);
            res = _secret_256.to_string();
        }
            break;
        default: {
            std::bitset<32> _secret_32(randSecret);
            res = _secret_32.to_string();
        }
            break;
    }
    this->secret = res;
    return res;
}

std::vector<std::string> XORSecretShares::split(size_t n) {
    std::vector<std::string> shadows(n);

    if (_init == false) {
        std::cerr << "ERROR" << std::endl;
        return shadows;
    }

    for (int i = 0; i < n - 1; i++) {
        std::string randSecret(key_size, '0');
        for (int j = 0; j < key_size; j++) {
            randSecret[j] = std::to_string(rand() % 2)[0];
        }
        shadows[i] = randSecret;
    }

    std::string last_secret = this->secret;

    for (int i = 0; i < n - 1; i++) {
        switch (key_size) {
            case 64: {
                std::bitset<64> _last_secret_64(last_secret);
                std::bitset<64> _secret_64(shadows[i]);
                last_secret = (_last_secret_64^=_secret_64).to_string();
            }
                break;
            case 128: {
                std::bitset<128> _last_secret_128(last_secret);
                std::bitset<128> _secret_128(shadows[i]);
                last_secret = (_last_secret_128^=_secret_128).to_string();
            }
                break;
            case 256: {
                std::bitset<256> _last_secret_256(last_secret);
                std::bitset<256> _secret_256(shadows[i]);
                last_secret = (_last_secret_256^=_secret_256).to_string();
            }
                break;
            default: {
                std::bitset<64> _last_secret_64(last_secret);
                std::bitset<64> _secret_64(shadows[i]);
                last_secret = (_last_secret_64^=_secret_64).to_string();
            }
                break;
        }
    }

    shadows[shadows.size() - 1] = last_secret;

    return shadows;
}

int main(int argc, char *argv[])
{
    srand (time(NULL));
    XORSecretShares xss(128);
    std::cout << "original secret" << "\n";
    std::string orig_secret = xss.generate_random_secret();
    std::cout << orig_secret << "\n\n";
    std::cout << "shadows" << "\n";
    auto shadows = xss.split(14);

    for (const auto &p : shadows) {
        std::cout << p << "\n";
    }

    std::cout << "\n";
    std::cout << "All shadows are XOR-ed" << "\n";

    auto key_size = xss.get_key_size();
    std::string xored_share = shadows[0];

    for (int i = 1; i < shadows.size(); i++) {
        switch (key_size) {
            case 64: {
                std::bitset<64> xored_share_64(xored_share);
                std::bitset<64> share_64(shadows[i]);
                xored_share = (xored_share_64^=share_64).to_string();
            }
                break;
            case 128: {
                std::bitset<128> xored_share_128(xored_share);
                std::bitset<128> share_128(shadows[i]);
                xored_share = (xored_share_128^=share_128).to_string();
            }
                break;
            case 256: {
                std::bitset<256> xored_share_256(xored_share);
                std::bitset<256> _share_256(shadows[i]);
                xored_share = (xored_share_256^=_share_256).to_string();
            }
                break;
            default: {
                std::bitset<64> xored_share_64(xored_share);
                std::bitset<64> _share_64(shadows[i]);
                xored_share = (xored_share_64^=_share_64).to_string();
            }
                break;
        }
        
    }

    std::cout << "Retrived Secret" << "\n";
    std::cout << xored_share << "\n";

    std::cout << "Original Secret" << "\n";
    std::cout << orig_secret << "\n";

    return 0;
}

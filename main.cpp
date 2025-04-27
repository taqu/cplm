#include <cstdint>
#include <iostream>

int main(void)
{
    {
        std::cout << "4bit:" << std::endl;
        for(int32_t i=0; i<16; ++i){
            int32_t x = static_cast<int32_t>(256.0f/15.0f*i + 0.5f);
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    {
        std::cout << "5bit:" << std::endl;
        for(int32_t i=0; i<32; ++i){
            int32_t x = static_cast<int32_t>(256.0f/31.0f*i + 0.5f);
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }

    {
        std::cout << "6bit:" << std::endl;
        for(int32_t i=0; i<64; ++i){
            int32_t x = static_cast<int32_t>(256.0f/63.0f*i + 0.5f);
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    {
        std::cout << "2bit:" << std::endl;
        for(int32_t i=0; i<4; ++i){
            float x = i/3.0f;
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    {
        std::cout << "3bit:" << std::endl;
        for(int32_t i=0; i<8; ++i){
            float x = i/7.0f;
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    {
        std::cout << "4bit:" << std::endl;
        for(int32_t i=0; i<16; ++i){
            float x = i/15.0f;
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
    {
        std::cout << "5bit:" << std::endl;
        for(int32_t i=0; i<32; ++i){
            float x = i/31.0f;
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }

    {
        std::cout << "6bit:" << std::endl;
        for(int32_t i=0; i<64; ++i){
            float x = i/63.0f;
            std::cout << x << ", ";
        }
        std::cout << std::endl;
    }
	return 0;
}

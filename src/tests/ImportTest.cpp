#include <fstream>
#include <string>

int main(void)
{
    std::ifstream infile("sdkljfbvsdkjfvbsdfkljbvsjkdfbvsd", std::ifstream::in);
    if (infile.is_open())
    {  
        infile.close();
        exit(1);
    }
    else
    {
        exit(0);
    }
}
#include <string>
#include <fstream>
#include <string>

int main(void)
{
    std::ofstream outfile("test", std::ofstream::out);
    if (outfile.is_open())
    {

        outfile.close();
		return 0;
    }
    else
    {
        exit(1);
    }
	
}
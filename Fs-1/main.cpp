#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
    //implemented as cat
    if (argc == 1)
    {
        std::string str;
        while (true)
        {
            std::cin >> str;
            std::cout << str << std::endl;
        }
        return 0;
    }

    for (int i = 1; i < argc ; i++)
    {

        std::fstream file(argv[i], std::ios::in);

        if (!file.is_open())
        {
            std::cout << argv[i] << ": ";
            std::cout << std::error_code{errno, std::generic_category()}.message() << std::endl;

            return errno;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::cout << line << std::endl;
        }

        file.close();
    }

}

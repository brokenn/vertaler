#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

void run (const std::string & code)
{
    std::cout << "RUN: " << code << std::endl;
}

int run_prompt ()
{
    for (std::string line; std::getline (std::cin, line); )
    {
        run (line);
    } 
    return 0;
}

int run_file (const std::string & filename)
{
    std::ifstream is (filename);
    std::string text ((std::istreambuf_iterator<char> (is)),
                       std::istreambuf_iterator<char> ());
    run (text);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        return run_prompt ();
    }
    else if (argc == 2)
    {
        return run_file (argv[1]);
    }
    else
    {
        std::cerr << "Incorrect use" << std::endl;
    }
}


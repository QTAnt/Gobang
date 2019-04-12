#include <iostream>
#include <vector>
#include <string>
int main()
{
    std::vector<std::string> v={
        "a",
        "b",
        "c",
    };

    std::cout << v[0] << std::endl;
    std::cout << v[1] << std::endl;
    std::cout << v[2] << std::endl;
    std::cout << v.size() << std::endl;
}

#include <iostream>
#include <cpr/cpr.h>

int main(int argc, char const *argv[])
{
    auto response = cpr::Post(
        cpr::Url{"http://127.0.0.1:5000/translate"},
        cpr::Body{"{\"api_key\":\"\",\"format\":\"text\",\"q\":\"hello\",\"source\":\"en\",\"target\":\"zt\"}"},
        cpr::Header{
            {"Content-Type", "application/json"}});
    std::cout << response.raw_header
              <<std::endl
              << response.text
              << std::endl;
    return 0;
}

#include <iostream>
#include <unistd.h>

int main()
{
    const char* spinChars = "-\\|/";

    int i = 0;
    while (true)
    {
        std::cout << "\r" << spinChars[i % 4] << " Processing... " << std::flush;
        usleep(100000);  // 等待一段时间，控制旋转速度
        ++i;
    }

    return 0;
}

#include <iostream>
/*Если в числе содержится только один бит со значением 1, записать в выходной
 * поток OK. Иначе записать FAIL*/

int main() {
    long int num;

    std::cin >> num;

    long int flagOfBit = 0;

    while (num) {
        if ((num & 1) == 1) {
            flagOfBit += 1;
        }
        num = num >> 1;
    }

    printf("%s", flagOfBit == 1 ? "OK" : "FAIL");

    return 0;
}
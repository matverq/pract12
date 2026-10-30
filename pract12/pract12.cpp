#include <iostream>
#include <conio.h>

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    cout << "Процесс создан\n";

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    if (num1 > num2) {
        cout << "Ошибка диапозона";
        return 1;
    }
    int maxSum = -1;
    int bestNumb = num1;

    for (int i = num1; i <= num2; i++)
    {
        int temp = i;
        int curSum = 0;
        while (temp > 0) {
            curSum += temp % 10;
            temp /= 10;
        }
        cout << "Число " << i << " сумма цифр " << curSum << "\n";
        if (curSum > maxSum) {
            maxSum = curSum;
            bestNumb = i;
        }
    }
    cout << "Число с наибольшей суммой цифр: " << bestNumb << ". Сумма с наибольшей суммой цифр " << maxSum;
    _getch();
}

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // rand, srand
#include <ctime>    // time

using namespace std;

void generate_all(int num)
{
    srand(time(NULL));  // 初始化亂數

    // 開三個檔案
    ofstream fileRand("..\\Random" + to_string(num) + ".txt", ios::out);
    ofstream fileOrder("..\\Order" + to_string(num) + ".txt", ios::out);
    ofstream fileReverse("..\\sOrder" + to_string(num) + ".txt", ios::out);

    for (int i = 0; i < num; ++i)
    {
        fileRand << rand() << endl;       // 隨機數
        fileOrder << i << endl;           // 遞增
        fileReverse << (num - i - 1) << endl;  // 遞減
    }

    // 關閉檔案
    fileRand.close();
    fileOrder.close();
    fileReverse.close();

    cout << "已生成 [Random" << num << ".txt, Order" << num << ".txt, sOrder" << num << ".txt]" << endl;
}

int main()
{
    int num;
    while (cout << "請輸入要生成的數字數量（Ctrl+C 離開）: ", cin >> num)
    {
        generate_all(num);
    }

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cstring> // memcpy, memmove
#include <cstdlib> // malloc, free

using namespace std;
using namespace chrono;

void binaryInsertionSort(vector<int>& working, const vector<int>& original)
{
    memcpy(working.data(), original.data(), sizeof(int) * original.size());

    for (int j = 1; j < working.size(); ++j)
    {
        int key = working[j];
        int left = 0, right = j;

        while (left < right)
        {
            int mid = (left + right) / 2;
            if (working[mid] <= key)
                left = mid + 1;
            else
                right = mid;
        }

        if (left != j)
        {
            memmove(&working[left + 1], &working[left], sizeof(int) * (j - left));
            working[left] = key;
        }
    }
}

vector<int> loadFile(const string& filename)
{
    ifstream infile(filename);
    vector<int> data;

    if (!infile.is_open())
    {
        cout << "無法打開檔案: " << filename << endl;
        return data;
    }

    string line;
    while (getline(infile, line))
    {
        stringstream ss(line);
        int num;
        while (ss >> num)
            data.push_back(num);
    }
    infile.close();
    return data;
}

int main()
{
    const string filename = "..\\Random1000.txt";
    const int runs = 10000;
    const int size = 1000;

    cout << "==== 測試檔案 ====" << endl;

    vector<int> original = loadFile(filename);
    if (!original.empty())
    {
        vector<int> working1(original.size());
        vector<int> working2(original.size());

        auto start = high_resolution_clock::now();

        for (int t = 0; t < runs; ++t)
        {
            if (t % 2 == 0)
                binaryInsertionSort(working1, original);
            else
                binaryInsertionSort(working2, original);
        }

        auto end = high_resolution_clock::now();
        double totalTime = duration_cast<duration<double>>(end - start).count();
        double avgTime = totalTime / runs;

        cout << "資料量 " << size << "，執行 " << runs << " 次，總耗時: " << fixed << setprecision(5) << totalTime
            << " 秒，平均每次: " << fixed << setprecision(8) << avgTime * 100000 << " 微秒" << endl;
    }

    cout << endl;

    return 0;
}
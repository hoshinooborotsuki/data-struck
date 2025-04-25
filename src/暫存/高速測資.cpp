#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cstring> // memcpy, memmove
#include <cstdlib> // malloc, free
#include <algorithm> // std::sort

using namespace std;
using namespace chrono;

// 使用內建 sort 排序
void stlSort(vector<int>& working, const vector<int>& original)
{
    memcpy(working.data(), original.data(), sizeof(int) * original.size());
    sort(working.begin(), working.end());
}
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
// Heap 子程序：維持最大堆的性質
void heap(vector<int>& arr, int n, int i)
{
    int largest = i;      // 最大值的節點
    int left = 2 * i + 1; // 左子節點
    int right = 2 * i + 2; // 右子節點

    // 如果左子節點比根節點大
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // 如果右子節點比目前最大值還大
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // 如果最大值不是根節點，則交換並繼續 heapify
    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        heap(arr, n, largest);
    }
}
// Heap Sort 排序函式
void heapSort(vector<int>& working, const vector<int>& original)
{
    memcpy(working.data(), original.data(), sizeof(int) * original.size());
    int n = working.size();

    // 建立最大堆
    for (int i = n / 2 - 1; i >= 0; i--)
        heap(working, n, i);

    // 一個一個將最大值放到最後面
    for (int i = n - 1; i > 0; i--)
    {
        swap(working[0], working[i]); // 將最大值放到最後
        heap(working, i, 0);       // 重新對縮小後的堆進行 heapify
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
    const vector<string> filePrefixes = { "Random", "Order", "sOrder" };
    const vector<int> testSizes = { 1000, 5000, 10000, 20000 };
    int runs;

    for (const auto& prefix : filePrefixes)
    {
        cout << "==== 測試檔案: " << prefix << " ====" << endl;

        for (int size : testSizes)
        {
            string filename = "..\\" + prefix + to_string(size) + ".txt";
            vector<int> original = loadFile(filename);
            if (original.empty())
                continue;

            vector<int> working1(original.size());
            vector<int> working2(original.size());

            auto start = high_resolution_clock::now();

            runs = 10000000 / size;
            for (int t = 0; t < runs; ++t)
            {
                if (t % 2 == 0)
                    heapSort(working1, original);
                else
                    heapSort(working2, original);
            }

            auto end = high_resolution_clock::now();
            double totalTime = duration_cast<duration<double>>(end - start).count();
            double avgTime = totalTime / runs;

            cout << "資料量 " << size << "，執行 " << runs << " 次，總耗時: " << fixed << setprecision(5) << totalTime
                << " 秒，平均每次: " << fixed << setprecision(8) << avgTime*100000 << " 微秒" << endl;
        }

        cout << endl;
    }

    return 0;
}

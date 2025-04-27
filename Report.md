# 第一組41243211王翔禹、41243213王維德

作業一

## 解題說明

• 本題要求實現以下問題的 worst-case 與 average-case
• 實作排序 n 個數字的函式，包含以下：
– Insertion Sort
– Quick Sort (using median-of-three method to choose pivot)
– Merge Sort (using iterative method)
– Heap Sort
– Composite Sort
• 我們使用 n=1000, 5000, 1000, 20000 時，計算每個排序函式的執行時間和記憶體，並繪製圖表
• 分析每個排序法的時間複雜度，並與測試結果進行比對，確認結果是否正確
• 找出在各個 n 時，哪個排序法最快 (以表格等形式呈現)，並寫一個結合多種排序
法的函式 (Composite Sort)，可以對應不同輸入資料，在最短的時間內完成排序
• 計時方式探討說明，包含精度等。(若使用內建函式庫的方式，可以附上簡單程式
碼說明)
• 說明 Heap Sort 的測試資料產生細節，如你使用「隨機產生」(題目要求) 幾次
• 說明 Merge Sort 的測試資料產生細節，包含你如何建立等

### 解題策略

隨機產生測試資料，將多次產生的測試資料給予排序函式，並算平均值。
每次產生的測試資料要保留給各個排序函式使用，避免因為測試資料不同而影響結果。

## 程式實作

以下為主要程式碼：

```cpp
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

            runs = 100000 / size;
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
```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 $$O(n \log n)$$ 或 $$O(n^2)$$。
2. 空間複雜度：空間複雜度為 $$O(1)$$ 或 $$O(n)$$。

## 測試與驗證

### 測試案例

### Insertion Sort

| 數量\情況 | 順序(最好) | 隨機 | 反序(最壞) |
|:--------:|:------------:|:------:|:------------:|
| 1000     | 3.771235 μs | 13.70789 μs | 7.976355 μs |
| 5000     | 23.128655 μs | 107.547145 μs | 115.036595 μs |
| 10000    | 52.5425 μs | 303.00974 μs | 402.40866 μs |
| 20000    | 114.0996 μs | 983.21562 μs | 1631.34204 μs |
![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/Insertion%20Sort.png)
時間複雜度：
- 順序：$O(n)$
- 隨機、反序：$O(n^2)$

### 內建 sort

| 數量 \ 情況 | 順序 | 隨機 | 反序 |
|:-----------:|:-----:|:-----:|:-----:|
| 1000  | 8.297352 ms | 28.923795 ms | 12.717466 ms |
| 5000  | 58.81136 ms | 165.00912 ms | 77.96431 ms |
| 10000 | 128.98077 ms | 356.61326 ms | 163.77543 ms |
| 20000 | 279.25102 ms | 761.82784 ms | 349.26926 ms |
![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/STL%20sort.png)
### Heap Sort

| 數量\情況 | 順序 | 隨機 | 反序 |
|:--------:|:------:|:------:|:------:|
| 1000     | 46.137727 μs | 48.128106 μs | 40.801393 μs |
| 5000     | 289.60007 μs | 296.141565 μs | 260.291775 μs |
| 10000    | 646.94058 μs | 624.67445 μs | 571.98586 μs |
| 20000    | 1368.08814 μs | 1344.94366 μs | 1249.88472 μs |
![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/heap%20sort.png)
時間複雜度：$O(n \log n)$。

### 空間複雜度

| 數量\情況 | 順序 | 隨機 | 反序 |
|:--------:|:------:|:------:|:------:|
|4 | 4 | 4 | 4|
|20 | 20 | 20 | 20|
|40 | 40 | 40 | 40|
|160 | 160 | 160 | 160|

### 結論

結合測試結果，Heap Sort 最穩定，內建 sort 在隨機資料上最快，Insertion Sort 對已排序資料最有效。


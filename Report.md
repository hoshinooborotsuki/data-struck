# 第一組41243211王翔禹、41243213王維德

&emsp;作業一

## 解題說明

• 本題要求實現以下問題的 worst-case 與 average-case

• 實作排序 n 個數字的函式，包含以下：

&emsp;– Insertion Sort<br>
&emsp;– Quick Sort (using median-of-three method to choose pivot)<br>
&emsp;– Merge Sort (using iterative method)<br>
&emsp;– Heap Sort<br>
&emsp;– Composite Sort<br>

• 我們使用 n=1000, 5000, 1000, 20000 時，計算每個排序函式的執行時間和記憶體，並繪製圖表

• 分析每個排序法的時間複雜度，並與測試結果進行比對，確認結果是否正確

• 找出在各個 n 時，哪個排序法最快 (以表格等形式呈現)，並寫一個結合多種排序法的函式 (Composite Sort)，可以對應不同輸入資料，在最短的時間內完成排序

• 計時方式探討說明，包含精度等。(若使用內建函式庫的方式，可以附上簡單程式碼說明)

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

            runs = 100000000 / size;
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
## STL Sort 函數
```
void stlSort(vector<int>& working, const vector<int>& original)
{
    memcpy(working.data(), original.data(), sizeof(int) * original.size());
    sort(working.begin(), working.end());
}
```
目的：使用 C++ 標準庫的 std::sort 實現排序。
參數：
working：用於執行排序的 vector<int>。
original：包含輸入資料的 const vector<int>，用於複製。
功能：
使用 memcpy 將 original 的資料複製到 working，避免修改原始資料。
呼叫 std::sort 對 working 進行升序排序。
備註：
std::sort 通常使用優化的混合演算法（例如 Introsort），結合快速排序、堆排序和插入排序。

## Binary Insertion Sort 函數
```
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
```
目的：因為教授說要最快,所以我找到了這個本體是插入排序,但使用二元來加速過程,實現了 Binary Insertion Sort，一種使用二分搜尋優化插入位置的插入排序。
參數：與 stlSort 相同。
功能：
使用 memcpy 將 original 複製到 working。
從索引 1 開始遍歷陣列：
將當前元素儲存為 key。
使用二分搜尋（left、right、mid）找到 key 的正確插入位置。
若插入位置 (left) 與當前索引 (j) 不同，則使用 memmove 移動元素以騰出空間，並插入 key。
備註：
Binary Insertion Sort 將比較次數從 O(n) 減少到 O(log n)，但仍需 O(n) 的移動操作。
適用於小型資料集，但在大型資料集上效率較低。
## Heap Sort 函數
```
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
```
目的：透過建立最大堆並反覆提取最大元素實現堆排序。
參數：與 stlSort 相同。
功能：
使用 memcpy 將 original 複製到 working。
對非葉節點 (n/2 - 1 至 0) 呼叫 heap，建立最大堆。
反覆執行：
將根節點（最大元素）與堆的最後元素交換。
縮減堆大小 (i) 並呼叫 heap 恢復最大堆性質。
備註：
堆排序的時間複雜度在所有情況下均為 O(n log n)。

## 檔案讀取函數
```
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
```
目的：從文字檔案中讀取整數並儲存到 vector<int>。
參數：
filename：輸入檔案的路徑。
功能：
使用 ifstream 開啟檔案。
若檔案無法開啟，輸出錯誤訊息並返回空向量。
逐行讀取檔案，使用 stringstream 解析每行中的整數。
將整數儲存到 vector<int> 並返回。
備註：
假設檔案包含以空格分隔的整數。
用於載入測試資料，例如 Random1000.txt。

## 
```
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

            auto start = high_resolution_clock::now();

            runs = 100000000 / size;
            for (int t = 0; t < runs; ++t)
                 heapSort(working1, original);

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
目的：協調對不同輸入檔案和資料量的堆排序效能測試。
關鍵組成部分：
檔案前綴與資料量：
filePrefixes：測試三種類型的輸入檔案：Random（隨機）、Order（已排序）、sOrder（部分排序）。
testSizes：測試資料量為 1000、5000、10000 和 20000。
檔案載入：
構建檔案名稱（例如 ..\Random1000.txt）並使用 loadFile 載入資料。
若檔案為空或無效，則跳過（continue）。
效能測試：
建立工作向量 (working1) 用於排序。
根據資料量計算執行次數 (runs = 100000 / size)，以平衡不同資料量的總執行時間。
使用 high_resolution_clock 測量 runs 次 heapSort 的執行時間。
計算總時間（秒）和平均每次執行時間（微秒）。
輸出：
為每種檔案類型和資料量輸出結果，包括資料量、執行次數、總耗時和平均每次耗時。
使用 fixed 和 setprecision 確保格式一致。
備註：
提供的程式碼僅測試 heapSort，儘管包含 stlSort 和 binaryInsertionSort。
執行次數隨資料量增加而減少（100000 / size），以保持總執行時間合理。
檔案路徑使用 ..\\，表示檔案位於上層目錄。
## 效能分析

1. 時間複雜度：程式的時間複雜度為 $$O(n \log n)$$ 或 $$O(n^2)$$。
2. 空間複雜度：空間複雜度為 $$O(1)$$ 或 $$O(n)$$。

## 測試與驗證
```
==== 測試檔案: Random ====
資料量 1000，執行 10000 次，總耗時: 5.29196 秒，平均每次: 52.91955500 微秒
資料量 5000，執行 2000 次，總耗時: 6.23591 秒，平均每次: 311.79570000 微秒
資料量 10000，執行 1000 次，總耗時: 6.74950 秒，平均每次: 674.94963000 微秒
資料量 20000，執行 500 次，總耗時: 7.07489 秒，平均每次: 1414.97796000 微秒

==== 測試檔案: Order ====
資料量 1000，執行 10000 次，總耗時: 4.88882 秒，平均每次: 48.88821300 微秒
資料量 5000，執行 2000 次，總耗時: 6.13751 秒，平均每次: 306.87569500 微秒
資料量 10000，執行 1000 次，總耗時: 6.58357 秒，平均每次: 658.35673000 微秒
資料量 20000，執行 500 次，總耗時: 7.35044 秒，平均每次: 1470.08772000 微秒

==== 測試檔案: sOrder ====
資料量 1000，執行 10000 次，總耗時: 4.25094 秒，平均每次: 42.50941000 微秒
資料量 5000，執行 2000 次，總耗時: 6.02066 秒，平均每次: 301.03312500 微秒
資料量 10000，執行 1000 次，總耗時: 7.02859 秒，平均每次: 702.85870000 微秒
資料量 20000，執行 500 次，總耗時: 10.55026 秒，平均每次: 2110.05232000 微秒
```
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
&emsp;- 順序：$O(n)$
&emsp;- 隨機、反序：$O(n^2)$

### 內建 sort

| 數量 \ 情況 | 順序 | 隨機 | 反序 |
|:-----------:|:-----:|:-----:|:-----:|
| 1000  | 8.297352 ms | 28.923795 ms | 12.717466 ms |
| 5000  | 58.81136 ms | 165.00912 ms | 77.96431 ms |
| 10000 | 128.98077 ms | 356.61326 ms | 163.77543 ms |
| 20000 | 279.25102 ms | 761.82784 ms | 349.26926 ms |

![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/STL%20sort.png)

時間複雜度：$O(n \log n)$（使用混合排序）

### Heap Sort

| 數量\情況 | 順序 | 隨機 | 反序 |
|:--------:|:------:|:------:|:------:|
| 1000     | 46.137727 μs | 48.128106 μs | 40.801393 μs |
| 5000     | 289.60007 μs | 296.141565 μs | 260.291775 μs |
| 10000    | 646.94058 μs | 624.67445 μs | 571.98586 μs |
| 20000    | 1368.08814 μs | 1344.94366 μs | 1249.88472 μs |

![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/heap%20sort.png)

&emsp;時間複雜度：$O(n \log n)$。

### 空間複雜度

| 數量\情況 | 順序 | 隨機 | 反序 |
|:--------:|:------:|:------:|:------:|
|4 | 4 | 4 | 4|
|20 | 20 | 20 | 20|
|40 | 40 | 40 | 40|
|160 | 160 | 160 | 160|


![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/Pagefile%20141751.png)


### 結論

綜合測試結果：

&emsp;Heap Sort：表現最穩定，在各種情況下時間差異不大。

&emsp;內建 sort：在隨機資料上速度最快，適合一般用途。

&emsp;Insertion Sort：對於已排序資料極為高效，但在隨機或反序資料下效能迅速下降。

# 第一組41243211王翔禹、41243213王維德

&emsp;作業一

## 解題說明

• 本題要求實現以下問題的 worst-case 與 average-case

• 實作排序 n 個數字的函式，包含以下：

&emsp;– Insertion Sort<br>
&emsp;– Quick Sort (using median-of-three method to choose pivot)<br>
&emsp;– Merge Sort <br>
&emsp;– Heap Sort<br>
&emsp;– Composite Sort<br>

• 我們使用 n=1000, 2000 ,3000 ,4000 , 5000, 6000, 7000, 8000, 9000, 10000, 15000 和 20000 時，計算每個排序函式的執行時間和記憶體，並繪製圖表

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
#define INSERTION_SORT_THRESHOLD 16 //

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

// 插入排序(作為加速quick sort用)
void insertion_sort(int arr[], int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// 自訂交換函式
inline void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

// 選三數中位數
int median_of_three(int arr[], int left, int right)
{
    int mid = left + (right - left) / 2;
    if (arr[right] < arr[left]) swap(arr[left], arr[right]);
    if (arr[mid] < arr[left]) swap(arr[mid], arr[left]);
    if (arr[right] < arr[mid]) swap(arr[right], arr[mid]);
    swap(arr[left], arr[mid]);
    return arr[left];
}

// 非遞迴版快速排序
void quicksort(int arr[], int left, int right) 
{
    std::stack<std::pair<int, int> > stk;
    stk.push(std::make_pair(left, right));

    while (!stk.empty()) {
        std::pair<int, int> range = stk.top();
        int l = range.first;
        int r = range.second;
        stk.pop();

        while (l < r) {
            if (r - l < INSERTION_SORT_THRESHOLD) 
            {
                insertion_sort(arr, l, r);
                break;
            }

            int pivot = median_of_three(arr, l, r);
            int i = l + 1;
            int j = r;

            while (i <= j) {
                while (i <= r && arr[i] <= pivot) ++i;
                while (j > l && arr[j] > pivot) --j;
                if (i < j) swap(arr[i], arr[j]);
            }
            swap(arr[l], arr[j]);

            if (j - 1 - l < r - (j + 1)) {
                if (l < j - 1) stk.push(std::make_pair(l, j - 1));
                l = j + 1;
            }
            else {
                if (j + 1 < r) stk.push(std::make_pair(j + 1, r));
                r = j - 1;
            }
        }
    }
}

void mergeSort(vector<int>& arr)
{
    int n = arr.size();
    vector<int> temp(n);

    for (int width = 1; width < n; width *= 2) {
        for (int i = 0; i < n; i += 2 * width) {
            int left = i;
            int mid = min(i + width, n);
            int right = min(i + 2 * width, n);

            int l = left, r = mid, k = left;

            while (l < mid && r < right)
                temp[k++] = (arr[l] <= arr[r]) ? arr[l++] : arr[r++];

            while (l < mid)
                temp[k++] = arr[l++];

            while (r < right)
                temp[k++] = arr[r++];
        }
        swap(arr, temp); // copy back
    }
}

}

int main()
{
    const vector<string> filePrefixes = { "Random", "Order", "sOrder" };
    const vector<int> testSizes = { 1000, 2000 ,3000 ,4000 , 5000, 6000, 7000, 8000, 9000, 10000, 15000, 20000 };
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

以下是一般的插入排序 與執行結果。
```
void insertionSort(vector<int>& working, const vector<int>& original)
{
    memcpy(working.data(), original.data(), sizeof(int) * original.size());

    for (int j = 1; j < working.size(); ++j)
    {
        int key = working[j];
        int i = j - 1;

        while (i >= 0 && working[i] > key)
        {
            working[i + 1] = working[i]; // 元素右移
            --i;
        }
        working[i + 1] = key; // 插入key
    }
}
==== 測試檔案: Random ====
資料量 1000，執行 100 次，總耗時: 0.39401 秒，平均每次: 394.01380000 微秒
資料量 2000，執行 50 次，總耗時: 0.70237 秒，平均每次: 1404.73800000 微秒
資料量 3000，執行 33 次，總耗時: 0.81289 秒，平均每次: 2463.31121212 微秒
資料量 4000，執行 25 次，總耗時: 1.09644 秒，平均每次: 4385.76760000 微秒
資料量 5000，執行 20 次，總耗時: 1.39631 秒，平均每次: 6981.53550000 微秒
資料量 6000，執行 16 次，總耗時: 1.61987 秒，平均每次: 10124.16187500 微秒
資料量 7000，執行 14 次，總耗時: 2.09615 秒，平均每次: 14972.52071429 微秒
資料量 8000，執行 12 次，總耗時: 2.38217 秒，平均每次: 19851.41666667 微秒
資料量 9000，執行 11 次，總耗時: 3.33248 秒，平均每次: 30295.22818182 微秒
資料量 10000，執行 10 次，總耗時: 5.41364 秒，平均每次: 54136.42200000 微秒
資料量 15000，執行 6 次，總耗時: 7.68360 秒，平均每次: 128059.99666667 微秒
資料量 20000，執行 5 次，總耗時: 12.06889 秒，平均每次: 241377.81800000 微秒

==== 測試檔案: Order ====
資料量 1000，執行 100 次，總耗時: 0.00196 秒，平均每次: 1.96040000 微秒
資料量 2000，執行 50 次，總耗時: 0.00471 秒，平均每次: 9.41900000 微秒
資料量 3000，執行 33 次，總耗時: 0.00183 秒，平均每次: 5.53939394 微秒
資料量 4000，執行 25 次，總耗時: 0.00308 秒，平均每次: 12.30560000 微秒
資料量 5000，執行 20 次，總耗時: 0.00235 秒，平均每次: 11.76100000 微秒
資料量 6000，執行 16 次，總耗時: 0.00227 秒，平均每次: 14.16250000 微秒
資料量 7000，執行 14 次，總耗時: 0.00214 秒，平均每次: 15.25285714 微秒
資料量 8000，執行 12 次，總耗時: 0.00149 秒，平均每次: 12.45666667 微秒
資料量 9000，執行 11 次，總耗時: 0.00163 秒，平均每次: 14.84363636 微秒
資料量 10000，執行 10 次，總耗時: 0.00234 秒，平均每次: 23.35600000 微秒
資料量 15000，執行 6 次，總耗時: 0.00220 秒，平均每次: 36.70666667 微秒
資料量 20000，執行 5 次，總耗時: 0.00171 秒，平均每次: 34.11000000 微秒

==== 測試檔案: sOrder ====
資料量 1000，執行 100 次，總耗時: 1.09170 秒，平均每次: 1091.70180000 微秒
資料量 2000，執行 50 次，總耗時: 2.02645 秒，平均每次: 4052.90700000 微秒
資料量 3000，執行 33 次，總耗時: 2.88450 秒，平均每次: 8740.92181818 微秒
資料量 4000，執行 25 次，總耗時: 4.14421 秒，平均每次: 16576.83960000 微秒
資料量 5000，執行 20 次，總耗時: 5.50207 秒，平均每次: 27510.33900000 微秒
資料量 6000，執行 16 次，總耗時: 5.44713 秒，平均每次: 34044.54250000 微秒
資料量 7000，執行 14 次，總耗時: 3.83456 秒，平均每次: 27389.69428571 微秒
資料量 8000，執行 12 次，總耗時: 4.30970 秒，平均每次: 35914.19000000 微秒
資料量 9000，執行 11 次，總耗時: 4.99243 秒，平均每次: 45385.75545455 微秒
資料量 10000，執行 10 次，總耗時: 5.49133 秒，平均每次: 54913.28600000 微秒
資料量 15000，執行 6 次，總耗時: 7.65270 秒，平均每次: 127544.93500000 微秒
資料量 20000，執行 5 次，總耗時: 10.88013 秒，平均每次: 217602.59400000 微秒
```
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

## Quick Sort 函數
```
void quicksort(int arr[], int left, int right) 
{
    std::stack<std::pair<int, int> > stk;
    stk.push(std::make_pair(left, right));

    while (!stk.empty()) {
        std::pair<int, int> range = stk.top();
        int l = range.first;
        int r = range.second;
        stk.pop();

        while (l < r) {
            if (r - l < INSERTION_SORT_THRESHOLD) 
            {
                insertion_sort(arr, l, r);
                break;
            }

            int pivot = median_of_three(arr, l, r);
            int i = l + 1;
            int j = r;

            while (i <= j) {
                while (i <= r && arr[i] <= pivot) ++i;
                while (j > l && arr[j] > pivot) --j;
                if (i < j) swap(arr[i], arr[j]);
            }
            swap(arr[l], arr[j]);

            if (j - 1 - l < r - (j + 1)) {
                if (l < j - 1) stk.push(std::make_pair(l, j - 1));
                l = j + 1;
            }
            else {
                if (j + 1 < r) stk.push(std::make_pair(j + 1, r));
                r = j - 1;
            }
        }
    }
}
```
目的：
非遞迴版本的快速排序法（Quick Sort），用以對整數陣列進行高效率排序。
透過迴圈與顯式堆疊取代傳統遞迴，來加快效能避免遞迴呼叫的低效，特別適用於大型資料集或對穩定性要求較高的應用場景。
參數：
int arr[]：待排序的整數陣列。
int left：排序範圍的起始索引（包含）。
int right：排序範圍的結束索引（包含）。
功能：
Pivot Selection：使用三中值法（Median-of-Three）選擇較佳主軸，降低資料偏態導致的性能退化。
小區間優化：若當前區間小於某閾值（如 INSERTION_SORT_THRESHOLD），則改採插入排序（Insertion Sort）進行處理，以提升局部效率。

備註：
使用三中值法（median-of-three）選擇 pivot，以降低在接近有序資料上退化為 O(n²) 的機率；
若子區間長度小於 INSERTION_SORT_THRESHOLD，則使用插入排序作為優化策略。

## Insertion Sort 函數
```
// 插入排序(作為加速quick sort用)
void insertion_sort(int arr[], int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
```
目的：作為快速排序中處理小區間的輔助排序方法，以簡單的方式對小範圍數列進行排序。

參數：與Quick Sort相同。
功能：
主要用途為輔助快速排序（Quick Sort），當子區間長度低於特定閾值時（INSERTION_SORT_THRESHOLD）
即改用插入排序進行處理，以避免快速排序在小區間內效能降低，從而整體提升排序效率。
備註：
插入排序在小型資料集下效率高，在 n 較小時常優於 Quick Sort，故常與之搭配使用以提昇整體效能。

## Merge Sort 函數
```
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        }
        else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
```
目的：
本排序演算法採用 分治策略（Divide and Conquer），透過遞迴方式將一個大型未排序陣列逐步拆解為較小的子陣列，直至每個子陣列僅包含一個元素（此時視為已排序）。
隨後，透過「合併」操作將這些已排序的子陣列依序整合為一個完整有序的陣列，從而達成整體排序的目的。
參數：
std::vector<int>& arr：待排序的整數向量，傳入時以參考方式（reference）避免複製開銷。
int left：排序範圍的起始索引（包含）。
int right：排序範圍的結束索引（包含）。
int mid（僅 merge 函數）：用於將目前排序範圍劃分為左右兩個子區間，計算方式為 (left + right) / 2。
功能：
mergeSort 函數為主要遞迴流程，負責將原始陣列不斷二分，直到每一段都為最小單位；
merge 函數則在遞迴結束回溯過程中，負責合併兩段已排序的子陣列，使其合併成更大範圍的有序區間，並最終完成整體排序。
備註：
演算法為穩定排序。
額外使用 O(n) 空間存儲臨時陣列。
適合處理大型資料且不在意額外記憶體開銷的情況。

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
testSizes：測試資料量為1000, 2000 ,3000 ,4000 , 5000, 6000, 7000, 8000, 9000, 10000, 15000 和 20000。
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
資料量 1000，執行 10000 次，總耗時: 4.67911 秒，平均每次: 46.79114600 微秒
資料量 2000，執行 5000 次，總耗時: 4.95586 秒，平均每次: 99.11727000 微秒
資料量 3000，執行 3333 次，總耗時: 5.24853 秒，平均每次: 157.47171017 微秒
資料量 4000，執行 2500 次，總耗時: 5.44341 秒，平均每次: 217.73638000 微秒
資料量 5000，執行 2000 次，總耗時: 5.62133 秒，平均每次: 281.06657000 微秒
資料量 6000，執行 1666 次，總耗時: 5.88219 秒，平均每次: 353.07291717 微秒
資料量 7000，執行 1428 次，總耗時: 5.75903 秒，平均每次: 403.29329832 微秒
資料量 8000，執行 1250 次，總耗時: 5.88504 秒，平均每次: 470.80300800 微秒
資料量 9000，執行 1111 次，總耗時: 6.00344 秒，平均每次: 540.36393339 微秒
資料量 10000，執行 1000 次，總耗時: 6.06175 秒，平均每次: 606.17467000 微秒
資料量 15000，執行 666 次，總耗時: 6.33869 秒，平均每次: 951.75545045 微秒
資料量 20000，執行 500 次，總耗時: 6.50667 秒，平均每次: 1301.33390000 微秒

==== 測試檔案: Order ====
資料量 1000，執行 10000 次，總耗時: 4.47720 秒，平均每次: 44.77200200 微秒
資料量 2000，執行 5000 次，總耗時: 4.89722 秒，平均每次: 97.94434000 微秒
資料量 3000，執行 3333 次，總耗時: 5.51826 秒，平均每次: 165.56420942 微秒
資料量 4000，執行 2500 次，總耗時: 5.42363 秒，平均每次: 216.94536400 微秒
資料量 5000，執行 2000 次，總耗時: 5.60225 秒，平均每次: 280.11241000 微秒
資料量 6000，執行 1666 次，總耗時: 5.76149 秒，平均每次: 345.82774310 微秒
資料量 7000，執行 1428 次，總耗時: 5.82400 秒，平均每次: 407.84338235 微秒
資料量 8000，執行 1250 次，總耗時: 6.00153 秒，平均每次: 480.12230400 微秒
資料量 9000，執行 1111 次，總耗時: 6.24965 秒，平均每次: 562.52504950 微秒
資料量 10000，執行 1000 次，總耗時: 6.08573 秒，平均每次: 608.57293000 微秒
資料量 15000，執行 666 次，總耗時: 6.35222 秒，平均每次: 953.78627628 微秒
資料量 20000，執行 500 次，總耗時: 6.57909 秒，平均每次: 1315.81894000 微秒

==== 測試檔案: sOrder ====
資料量 1000，執行 10000 次，總耗時: 3.96072 秒，平均每次: 39.60722700 微秒
資料量 2000，執行 5000 次，總耗時: 4.43173 秒，平均每次: 88.63455200 微秒
資料量 3000，執行 3333 次，總耗時: 4.80388 秒，平均每次: 144.13072007 微秒
資料量 4000，執行 2500 次，總耗時: 4.90542 秒，平均每次: 196.21684800 微秒
資料量 5000，執行 2000 次，總耗時: 5.05082 秒，平均每次: 252.54075500 微秒
資料量 6000，執行 1666 次，總耗時: 5.19229 秒，平均每次: 311.66182473 微秒
資料量 7000，執行 1428 次，總耗時: 5.43259 秒，平均每次: 380.43351541 微秒
資料量 8000，執行 1250 次，總耗時: 5.44772 秒，平均每次: 435.81765600 微秒
資料量 9000，執行 1111 次，總耗時: 5.46086 秒，平均每次: 491.52684968 微秒
資料量 10000，執行 1000 次，總耗時: 5.52150 秒，平均每次: 552.14999000 微秒
資料量 15000，執行 666 次，總耗時: 5.82021 秒，平均每次: 873.90570571 微秒
資料量 20000，執行 500 次，總耗時: 6.03525 秒，平均每次: 1207.05038000 微秒
```
### 測試案例
### Binary Insertion Sort

| 數量\情況 | 順序(最好) | 隨機 | 反序(最壞) |
|:------:|:----------------:|:--------------:|:----------------:|
| 1000   | 14.99324000 μs   | 5.87340200 μs  | 16.90587000 μs   |
| 2000   | 29.21662000 μs   | 14.52434200 μs | 41.51105600 μs   |
| 3000   | 50.87651365 μs   | 23.41377138 μs | 82.47122112 μs   |
| 4000   | 76.87901200 μs   | 30.64192000 μs | 133.59497200 μs  |
| 5000   | 108.58655000 μs  | 43.24525500 μs | 188.07927500 μs  |
| 6000   | 209.73731693 μs  | 55.73903962 μs | 264.55898559 μs  |
| 7000   | 339.69464986 μs  | 56.29905462 μs | 372.30793417 μs  |
| 8000   | 370.02047200 μs  | 73.73931200 μs | 500.96449600 μs  |
| 9000   | 458.27997300 μs  | 84.58099010 μs | 576.06984698 μs  |
| 10000  | 520.93220000 μs  | 115.33644000 μs| 768.75372000 μs  |
| 15000  | 1038.13148649 μs | 159.63680180 μs| 1222.45448949 μs  |
| 20000  | 1668.56192000 μs | 223.86578000 μs| 2892.84832000 μs |
### Insertion Sort

| 數量\情況 | 順序(最好) | 隨機 | 反序(最壞) |
|:------:|:----------------:|:--------------:|:----------------:|
| 1000   | 394.01380000 μs   | 1.96040000 μs  | 1091.70180000 μs |
| 2000   | 1404.73800000 μs  | 9.41900000 μs  | 4052.90700000 μs |
| 3000   | 2463.31121212 μs  | 5.53939394 μs  | 8740.92181818 μs |
| 4000   | 4385.76760000 μs  | 12.30560000 μs | 16576.83960000 μs|
| 5000   | 6981.53550000 μs  | 11.76100000 μs | 27510.33900000 μs|
| 6000   | 10124.16187500 μs | 14.16250000 μs | 34044.54250000 μs|
| 7000   | 14972.52071429 μs | 15.25285714 μs | 27389.69428571 μs|
| 8000   | 19851.41666667 μs | 12.45666667 μs | 35914.19000000 μs|
| 9000   | 30295.22818182 μs | 14.84363636 μs | 45385.75545455 μs|
| 10000  | 54136.42200000 μs | 23.35600000 μs | 54913.28600000 μs|
| 15000  | 128059.99666667 μs| 36.70666667 μs | 127544.93500000 μs|
| 20000  | 241377.81800000 μs| 34.11000000 μs | 217602.59400000 μs|



![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/Figure_2.png)
![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/Figure_3.png)

時間複雜度：
&emsp;- 順序：$O(n)$
&emsp;- 隨機、反序：$O(n^2)$

### 內建 sort

| 數量\情況 | 順序(最好) | 隨機 | 反序(最壞) |
|:------:|:----------------:|:--------------:|:----------------:|
| 1000   | 29.32899000 μs   | 8.30952000 μs  | 12.59119000 μs   |
| 2000   | 79.56834000 μs   | 18.03234000 μs | 26.94976000 μs   |
| 3000   | 125.20807808 μs  | 31.80474474 μs | 43.02642643 μs   |
| 4000   | 157.99156000 μs  | 40.68080000 μs | 58.17636000 μs   |
| 5000   | 160.67975000 μs  | 58.45725000 μs | 78.91870000 μs   |
| 6000   | 193.99963855 μs  | 69.62602410 μs | 90.72981928 μs   |
| 7000   | 241.68056338 μs  | 79.88302817 μs | 111.85992958 μs  |
| 8000   | 272.28552000 μs  | 92.60376000 μs | 125.59584000 μs  |
| 9000   | 322.02648649 μs  | 114.69927928 μs| 143.10720721 μs  |
| 10000  | 349.39270000 μs  | 128.36840000 μs| 162.22470000 μs  |
| 15000  | 539.10742424 μs  | 195.12242424 μs| 267.73424242 μs  |
| 20000  | 747.63260000 μs  | 280.02340000 μs| 354.50940000 μs  |



![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/Figure_4.png)

時間複雜度：$O(n \log n)$（使用混合排序）

### Heap Sort

| 數量\情況 | 順序(最好) | 隨機 | 反序(最壞) |
|:------:|:----------------:|:--------------:|:----------------:|
| 1000   | 46.79114600 μs   | 44.77200200 μs | 39.60722700 μs   |
| 2000   | 99.11727000 μs   | 97.94434000 μs | 88.63455200 μs   |
| 3000   | 157.47171017 μs  | 165.56420942 μs| 144.13072007 μs  |
| 4000   | 217.73638000 μs  | 216.94536400 μs| 196.21684800 μs  |
| 5000   | 281.06657000 μs  | 280.11241000 μs| 252.54075500 μs  |
| 6000   | 353.07291717 μs  | 345.82774310 μs| 311.66182473 μs  |
| 7000   | 403.29329832 μs  | 407.84338235 μs| 380.43351541 μs  |
| 8000   | 470.80300800 μs  | 480.12230400 μs| 435.81765600 μs  |
| 9000   | 540.36393339 μs  | 562.52504950 μs| 491.52684968 μs  |
| 10000  | 606.17467000 μs  | 608.57293000 μs| 552.14999000 μs  |
| 15000  | 951.75545045 μs  | 953.78627628 μs| 873.90570571 μs  |
| 20000  | 1301.33390000 μs | 1315.81894000 μs| 1207.05038000 μs |


![image](https://github.com/hoshinooborotsuki/data-struck/blob/main/src/chart/Figure_1.png)

&emsp;時間複雜度：$O(n \log n)$。

### quick

| 資料量 | Random    | Order     | worstcase |
|--------|-----------|-----------|-----------|
| 1000   | 138.11    | 76.18     | 115.34    |
| 2000   | 335.05    | 151.29    | 328.15    |
| 3000   | 543.15    | 296.78    | 514.39    |
| 4000   | 791.98    | 342.71    | 511.95    |
| 5000   | 1116.20   | 559.78    | 726.78    |
| 6000   | 1175.95   | 539.02    | 927.35    |
| 7000   | 1317.37   | 577.00    | 1210.84   |
| 8000   | 1513.44   | 582.01    | 857.79    |
| 9000   | 1532.86   | 895.24    | 1402.00   |
| 10000  | 1665.84   | 1072.16   | 1507.00   |
| 15000  | 2640.66   | 1138.93   | 2471.79   |
| 20000  | 3729.18   | 2298.05   | 2902.81   |

### merge

| 資料量 | Random    | Order     | worstcase |
|--------|-----------|-----------|-----------|
| 1000   | 138.11    | 76.18     | 115.34    |
| 2000   | 335.05    | 151.29    | 328.15    |
| 3000   | 543.15    | 296.78    | 514.39    |
| 4000   | 791.98    | 342.71    | 511.95    |
| 5000   | 1116.20   | 559.78    | 726.78    |
| 6000   | 1175.95   | 539.02    | 927.35    |
| 7000   | 1317.37   | 577.00    | 1210.84   |
| 8000   | 1513.44   | 582.01    | 857.79    |
| 9000   | 1532.86   | 895.24    | 1402.00   |
| 10000  | 1665.84   | 1072.16   | 1507.00   |
| 15000  | 2640.66   | 1138.93   | 2471.79   |
| 20000  | 3729.18   | 2298.05   | 2902.81   |


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

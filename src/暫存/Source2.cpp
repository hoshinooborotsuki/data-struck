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
#include <Windows.h>
#include <Psapi.h>

using namespace std;
using namespace chrono;

// �ϥΤ��� sort �Ƨ�
void stlSort(vector<int>& working, const vector<int>& original)
{
    working.clear(); // �M�����e����ơA�קK�v�T
    working.reserve(original.size()); // �w���t�Ŷ�
    memcpy(working.data(), original.data(), sizeof(int) * original.size());
    sort(working.begin(), working.end());
}

void binaryInsertionSort(vector<int>& working, const vector<int>& original)
{
    working.clear();
    working.reserve(original.size()); // �w���t�Ŷ�

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

// Heap �l�{�ǡG�����̤j�諸�ʽ�
void heap(vector<int>& arr, int n, int i)
{
    int largest = i;      // �̤j�Ȫ��`�I
    int left = 2 * i + 1; // ���l�`�I
    int right = 2 * i + 2; // �k�l�`�I

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        heap(arr, n, largest);
    }
}

// Heap Sort �ƧǨ禡
void heapSort(vector<int>& working, const vector<int>& original)
{
    working.clear();
    working.reserve(original.size()); // �w���t�Ŷ�
    memcpy(working.data(), original.data(), sizeof(int) * original.size());
    int n = working.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heap(working, n, i);

    for (int i = n - 1; i > 0; i--)
    {
        swap(working[0], working[i]);
        heap(working, i, 0);
    }
}

// ��ܰO����ϥα��p
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));

    cout << "Pagefile Usage: " << memInfo.PagefileUsage / 1024 << " KB" << endl;

}

vector<int> loadFile(const string& filename)
{
    ifstream infile(filename);
    vector<int> data;

    if (!infile.is_open())
    {
        cout << "�L�k���}�ɮ�: " << filename << endl;
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
    const vector<string> filePrefixes = { "sOrder" };// "Random","Order", "sOrder"
    const vector<int> testSizes = { 1000, 5000, 10000, 20000 };
    int runs;

    for (const auto& prefix : filePrefixes)
    {
        cout << "==== �����ɮ�: " << prefix << " ====" << endl;

        for (int size : testSizes)
        {
            string filename = "..\\" + prefix + to_string(size) + ".txt";
            vector<int> original = loadFile(filename);
            if (original.empty())
                continue;

            vector<int> working1;
            vector<int> working2;

            printMemoryUsage();  // ��ܪ�l�O����ϥα��p

            runs = 100000 / size;
            for (int t = 0; t < runs; ++t)
            {
                if (t % 2 == 0)
                    heapSort(working1, original);
                else
                    heapSort(working2, original);
            }

            printMemoryUsage();  // ��ܱƧǫ�O����ϥα��p
            working1.shrink_to_fit();  // �j������h�l���O����
            working2.shrink_to_fit();  // �j������h�l���O����
        }

        cout << endl;
    }

    return 0;
}

#include <bits/stdc++.h>
#include <chrono>
#include <random>
#include <thread>
using namespace std;

void insertionSort(vector<int> &arr) {
  for (int i = 1; i < arr.size(); i++) {
    int val = arr[i];

    int j = i - 1;
    while (j >= 0 && arr[j] > val) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = val;
  }
  return;
}

void mergeSort(vector<int> &arr) {
  if (arr.size() <= 1) {
    return;
  }
  size_t m = arr.size() / 2;
  vector<int> first(arr.begin(), arr.begin() + m);
  vector<int> second(arr.begin() + m, arr.end());

  mergeSort(first);
  mergeSort(second);

  merge(first.begin(), first.end(), second.begin(), second.end(), arr.begin());
  return;
}

void combineSort(vector<int> &arr) {
  if (arr.size() <= 15) {
    insertionSort(arr);
    return;
  }
  size_t m = arr.size() / 2;
  vector<int> first(arr.begin(), arr.begin() + m);
  vector<int> second(arr.begin() + m, arr.end());

  combineSort(first);
  combineSort(second);

  merge(first.begin(), first.end(), second.begin(), second.end(), arr.begin());
  return;
}

class ArrayGenerator {
private:
  const int UNSORTED_PERCENTS = 5;
  int size_;

  std::mt19937 gen_;
  std::uniform_int_distribution<> dis_;

  int randNum() { return dis_(gen_); }
  void randSwap(vector<int> &arr) {
    swap(arr[rand() % arr.size()], arr[rand() % arr.size()]);
  }

public:
  vector<int> generated;

  ArrayGenerator(int size, int minValue, int maxValue)
      : size_(size), dis_(minValue, maxValue) {}
  ArrayGenerator() : ArrayGenerator(0, 0, 0) {}

  void genRandom() {
    generated.resize(size_);
    for (size_t i = 0; i < size_; ++i) {
      generated[i] = randNum();
    }
  }

  void genReverseSorted() {
    genRandom();
    sort(generated.begin(), generated.end());
    reverse(generated.begin(), generated.end());
  }

  void genAlmostSorted() {
    genRandom();
    sort(generated.begin(), generated.end());
    int swaps = size_ * UNSORTED_PERCENTS / 100;
    while (swaps--) {
      randSwap(generated);
    }
  }

  void gen(string dataType) {
    if (dataType == "random") {
      genRandom();
    } else if (dataType == "reverse_sorted") {
      genReverseSorted();
    } else if (dataType == "almost_sorted") {
      genReverseSorted();
    } else {
      throw invalid_argument{"incorrect dataType"};
    }
    return;
  }

  vector<int> selectK(size_t k) {
    std::vector<int> indices(generated.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen_);
    sort(indices.begin(), indices.begin() + k);

    vector<int> selected(k);
    for (size_t i = 0; i < k; i++) {
      selected[i] = generated[indices[i]];
    }
    return selected;
  }
};

class SortTester {
private:
  vector<int> testData_;
  std::function<void(std::vector<int> &)> sortFunction_;

public:
  SortTester(vector<int> testData,
             const std::function<void(std::vector<int> &)> &sortFunction)
      : testData_(testData), sortFunction_(sortFunction) {}

  long long test() {
    auto start = std::chrono::high_resolution_clock::now();

    sortFunction_(testData_);

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    return msec;
  }
};

void Sleep(size_t milliseconds) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  return;
}

void query(string dataType, int repeats = 100) {
  auto generator = ArrayGenerator(10000, 0, 6000);

  vector<long double> arrSizes, mergeScores, combineScores;
  for (int repeat = 0; repeat < repeats; repeat++) {
    generator.gen(dataType);
    for (size_t size = 500, i = 0; size <= 10000; size += 100, i++) {
      auto testData = generator.selectK(size);
      auto mergeTester = SortTester(testData, mergeSort);
      auto combineTester = SortTester(testData, combineSort);

      long long mergeScore = mergeTester.test();
      long long combineScore = combineTester.test();

      if (i < arrSizes.size()) {
        mergeScores[i] += mergeScore;
        combineScores[i] += combineScore;
      } else {
        arrSizes.push_back(size);
        mergeScores.push_back(mergeScore);
        combineScores.push_back(combineScore);
      }
    }
  }

  // get average in milliseconds
  for (auto &v : mergeScores) {
    v /= (repeats);
  }
  for (auto &v : combineScores) {
    v /= (repeats);
  }

  string filename = "merge_" + dataType + ".txt";
  cout << filename << " created!\n";
  std::ofstream out(filename);
  for (auto v : arrSizes) {
    out << v << ' ';
  }
  out << '\n';
  for (auto v : mergeScores) {
    out << v << ' ';
  }
  out << '\n';

  filename = "combine_" + dataType + ".txt";
  cout << filename << " created!\n";
  out = std::ofstream(filename);
  for (auto v : arrSizes) {
    out << v << ' ';
  }
  out << '\n';
  for (auto v : combineScores) {
    out << v << ' ';
  }
  out << '\n';
  return;
}

void solve() {
  vector<string> dataTypes = {"random", "reverse_sorted", "almost_sorted"};
  for (string dataType : dataTypes) {
    query(dataType);
  }
  return;
}

int main() {
  cin.tie(nullptr);
  cout.tie(nullptr);
  ios_base::sync_with_stdio(false);

  solve();

  return 0;
}

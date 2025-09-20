// COMSC-210 | Lab 13 (STD::vector) | Chevin Jeon
// IDE used: C++11 VS Code
//
// Real-world simulation: Daily step counts for a 30-day fitness tracker.
// - Reads integers from an external file (one per line) into std::vector<int>.
// - Demonstrates std::vector member functions & iterator-based algorithms.
// - Validates file open and data length, prints specific error messages, and shows results.

#include <iostream>     // cout, cin
#include <fstream>      // ifstream
#include <vector>       // std::vector
#include <algorithm>    // min_element, max_element, sort, reverse
#include <numeric>      // accumulate
#include <iomanip>      // setw, setprecision
#include <string>       // std::string
using namespace std;

//======================== Constants ========================

const string INPUT_FILE = "steps.txt";   // external data file (≥ 30 integers, one per line)
const size_t MIN_DAYS = 30;              // minimum number of elements required
const int FIELD_W = 6;                   // setw width for printing

//==================== Function Prototypes ==================

// loadSteps() reads all integers from a file into a std::vector,
// validates that we have at least MIN_DAYS entries.
// arguments: const string& filename, vector<int>& out
// returns: bool (true if success, false otherwise)
bool loadSteps(const string& filename, vector<int>& steps);

// printVector() prints the vector in rows for readability
// arguments: const vector<int>& steps
void printVector(const vector<int>& steps);

// computeAndPrintStats() shows min, max, sum, average and front/back via member functions
// arguments: const vector<int>& steps
void computeAndPrintStats(const vector<int>& steps);

// mutateWithDataPointer() demonstrates vector::data() by incrementing via raw pointer
// arguments: vector<int>& steps, int delta
void mutateWithDataPointer(vector<int>& steps, int delta);

// demoVectorMembers() exercises size(), capacity(), reserve(), shrink_to_fit(),
// at(), front(), back(), push_back(), pop_back(), resize(), assign(), insert(), erase(), swap()
// arguments: vector<int>& steps
void demoVectorMembers(vector<int>& steps);

// printSortedTopK() copies to a temp vector, sorts it, and prints top K values
// arguments: const vector<int>& steps, size_t k
void printSortedTopK(const vector<int>& steps, size_t k);

//======================== main() ===========================

int main() {
    cout << "COMSC-210 | Lab 13 (STD::vector) | Chevin Jeon\n";
    cout << "Reading daily step counts from file and exercising std::vector...\n\n";

    vector<int> steps;                   // empty vector; we’ll fill from file
    if (!loadSteps(INPUT_FILE, steps)) { // read data + validate
        cout << "Terminating program due to input error.\n";
        return 1;
    }

    // Show raw data
    cout << "Raw step counts (from file):\n";
    printVector(steps);

    // Stats with iterator algorithms
    computeAndPrintStats(steps);

    // Demonstrate rich std::vector API
    demoVectorMembers(steps);

    // Show effect of demo mutations
    cout << "\nAfter demoVectorMembers() mutations:\n";
    printVector(steps);

    // Demonstrate pointer-based mutation via data()
    mutateWithDataPointer(steps, /*delta=*/100);
    cout << "\nAfter mutateWithDataPointer(+100) via vector::data():\n";
    printVector(steps);

    // Print Top-5 after sorting a copy (original order preserved)
    printSortedTopK(steps, 5);

    cout << "\nDone. Program completed successfully.\n";
    return 0;
}

//================= Function Definitions ====================

bool loadSteps(const string& filename, vector<int>& steps) {
    ifstream in(filename.c_str());
    if (!in) {
        cout << "ERROR: Could not open input file '" << filename
             << "'. Ensure the file exists in the working directory.\n";
        return false;
    }

    // Read *all* integers from the file.
    // If your assignment requires "exactly 30", then:
    //  - stop after 30 reads, and
    //  - error if there are fewer than 30, and
    //  - warn if there are more.
    int x;
    steps.clear();
    steps.reserve(64); // small optimization to reduce reallocations for typical sizes
    while (in >> x) {
        steps.push_back(x);
    }

    if (steps.size() < MIN_DAYS) {
        cout << "ERROR: Only read " << steps.size() << " values from '" << filename
             << "'. Expected at least " << MIN_DAYS << " integers (one per line).\n"
             << "Tip: Check for non-numeric characters or missing lines.\n";
        return false;
    }

    // If instructor wants *exactly* 30, uncomment next lines:
    // if (steps.size() > MIN_DAYS) {
    //     cout << "WARNING: File contains more than " << MIN_DAYS
    //          << " integers. Only the first " << MIN_DAYS << " will be used.\n";
    //     steps.resize(MIN_DAYS);
    // }

    return true;
}

void printVector(const vector<int>& steps) {
    const int COLS = 10;
    for (size_t i = 0; i < steps.size(); ++i) {
        cout << setw(FIELD_W) << steps[i];
        if ((i + 1) % COLS == 0) cout << '\n';
    }
    if (steps.size() % COLS != 0) cout << '\n';
}

void computeAndPrintStats(const vector<int>& steps) {
    // Sum / avg via <numeric>
    long long sum = accumulate(steps.begin(), steps.end(), 0LL);
    double avg = steps.empty()
                 ? 0.0
                 : static_cast<double>(sum) / static_cast<double>(steps.size());

    // Min/Max via <algorithm>
    auto itMin = min_element(steps.begin(), steps.end());
    auto itMax = max_element(steps.begin(), steps.end());

    cout << "\n--- Stats ---\n";
    cout << "Days (size): " << steps.size()
         << " (capacity: " << steps.capacity() << ")\n";

    if (!steps.empty()) {
        cout << "Front (day 1): " << steps.front() << "\n";
        cout << "Back  (day " << steps.size() << "): " << steps.back() << "\n";
    }

    cout << "Sum: " << sum << "\n";
    cout << "Avg: " << fixed << setprecision(1) << avg << "\n";

    if (!steps.empty()) {
        cout << "Min: " << *itMin << " (index " << (itMin - steps.begin()) << ")\n";
        cout << "Max: " << *itMax << " (index " << (itMax - steps.begin()) << ")\n";
    }

    // Safe access demo with at(): show first 3 values (bounds-checked)
    cout << "Preview via at(): ";
    for (size_t i = 0; i < 3 && i < steps.size(); ++i) {
        cout << steps.at(i) << (i + 1 < 3 ? ", " : "\n");
    }
}

void mutateWithDataPointer(vector<int>& steps, int delta) {
    // vector::data() returns pointer to contiguous storage (since C++11 guaranteed)
    int* p = steps.data();
    for (size_t i = 0; i < steps.size(); ++i) {
        p[i] += delta;
    }
}

void demoVectorMembers(vector<int>& steps) {
    cout << "\n--- Demonstrating std::vector members ---\n";

    // Show current size/capacity and reserve additional capacity to reduce reallocations
    cout << "Before reserve: size=" << steps.size() << ", capacity=" << steps.capacity() << "\n";
    steps.reserve(max(steps.capacity(), steps.size() + 32));
    cout << "After  reserve: size=" << steps.size() << ", capacity=" << steps.capacity() << "\n";

    // push_back(): append a small synthetic sequence (e.g., bonuses)
    // We keep it small so original data remains visible
    steps.push_back(steps.empty() ? 5000 : steps.back() + 50);
    steps.push_back(steps.back() + 50);
    cout << "After push_back x2: size=" << steps.size() << ", capacity=" << steps.capacity() << "\n";

    // pop_back(): remove the last appended element
    if (!steps.empty()) steps.pop_back();
    cout << "After pop_back():  size=" << steps.size() << ", capacity=" << steps.capacity() << "\n";

    // resize(): bump first and last elements by +250 using at() (bounds-checked)
    if (steps.size() >= 1) {
        steps.at(0) += 250;
        steps.at(steps.size() - 1) += 250;
        cout << "front() after +250: " << steps.front()
             << ", back() after +250: " << steps.back() << "\n";
    }

    // insert(): insert a marker value near the beginning (not at index 0 to keep day 1 visible)
    if (steps.size() >= 2) {
        steps.insert(steps.begin() + 1, 7777); // arbitrary marker
        cout << "Inserted 7777 at index 1.\n";
    }

    // erase(): if we have that marker, remove it to restore structure
    if (steps.size() >= 2 && steps[1] == 7777) {
        steps.erase(steps.begin() + 1);
        cout << "Erased marker at index 1.\n";
    }

    // assign(): create a baseline vector (all 5000s) and then swap back and forth
    vector<int> baseline;
    baseline.assign(steps.size(), 5000); // baseline filled with 5000, same length as steps

    cout << "Swapping with a baseline vector (all 5000s)...\n";
    steps.swap(baseline);

    // Show first few to verify swap
    cout << "After swap, 'steps' begins with:";
    for (size_t i = 0; i < min<size_t>(10, steps.size()); ++i) cout << setw(FIELD_W) << steps[i];
    cout << "\n";

    // Swap back to restore original data
    cout << "Swapping back to restore original data...\n";
    steps.swap(baseline);

    // shrink_to_fit(): request (non-binding) capacity reduction close to size()
    // This is implementation-defined; we show intent and print results either way.
    const size_t cap_before = steps.capacity();
    steps.shrink_to_fit();
    cout << "shrink_to_fit() requested (cap " << cap_before
         << " -> " << steps.capacity() << ").\n";
}

void printSortedTopK(const vector<int>& steps, size_t k) {
    if (k == 0 || steps.empty()) return;
    if (k > steps.size()) k = steps.size();

    // Copy and sort descending (keep original order intact)
    vector<int> copy = steps;
    sort(copy.begin(), copy.end());   // ascending
    reverse(copy.begin(), copy.end()); // now descending

    cout << "\nTop-" << k << " days (sorted, descending):\n";
    for (size_t i = 0; i < k; ++i) {
        cout << setw(FIELD_W) << copy[i];
    }
    cout << "\n";
}

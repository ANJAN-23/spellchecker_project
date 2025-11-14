#include <iostream>
using namespace std;

// Recursive function that counts depth
bool safeRecursion(unsigned long long depth, unsigned long long &maxDepth, unsigned long long step = 1000) {
    maxDepth = depth;

    // Print progress every 'step' calls
    if (depth % step == 0) {
        cout << "Reached depth: " << depth << endl;
    }

    // Try recursive call inside a try-catch block
    try {
        return safeRecursion(depth + 1, maxDepth, step);
    } catch (...) {
        // Stop safely if something goes wrong
        return false;
    }
}

int main() {
    unsigned long long maxDepth = 0;
    cout << "Starting computer ability test (recursion depth test)..." << endl;

    // Run the safe recursion
    safeRecursion(1, maxDepth);

    cout << "\nComputer ability test complete!" << endl;
    cout << "Maximum recursion depth your system allows: " << maxDepth << endl;
    return 0;
}

#include <iostream>
#include <vector>
#include <numeric>

int main() {
    std::vector<int> v{1,2,3,4,5};
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Hello from C++17 on BeaglePlay! Sum=" << sum << "\n";
    std::cout << "Imi iubesc sotia pentru ca m-a lasat sa lucrez cu placuta azi" << "\n";
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
#define ll long long

int main() {
    //input cin >> myArray
    vector<int> nums = {2, 5, 3, 9, 5, 3}; // array, staticly defined
    int n = nums.size();

    if (n == 1) {
        cout << 0 << endl;
        return 0;
    }

    ll sum = accumulate(nums.begin(), nums.end(), 0);
    ll prefix = 0;
    int minDiff = 1000000; 
    int idx = 0;

    for (int i = 0; i < nums.size(); ++i) {
      prefix += nums[i];
      ll leftAvg = prefix / (i + 1);

      ll rightAvg = 0;

      if (i < n - 1) {
          rightAvg = (sum - prefix) / (n - i - 1);
      }

      int diff = abs(leftAvg - rightAvg);

      if (diff < minDiff) {
          minDiff = diff;
          idx = i;
      }
      
    }

    cout << idx << endl;

    return 0;
}

class Solution {
public:
    int maxRotateFunction(vector<int>& nums) {
    int mx, sum = 0, current, previous, initial = 0, n = nums.size();
    //initial is in our case f0. calculate the first case according to task and then we will modify in the next loop

    for(int i = 0; i < n; i++) {
        sum += nums[i];
        initial += (i * nums[i]);
    }

    mx = initial;
    previous = initial; //since previous is already calculated answer, now we can start for loop from 1 to given arr (vector) size;

    for(int i = 1; i < n; i++) {
        current = previous + sum - n * nums[n - i];
        previous = current;

        mx = max(mx, current);
    }
        return mx;
    }
};
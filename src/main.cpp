#include <iostream>
int main() {
    int ans = 0;
    int n = 0;
    std :: cin >> n;
    for (int i = 1; i <= n; i++) {
        ans += i;
        std :: cout << ans << " ";
        std :: fflush(stdout);
    }
    std :: cout << "hello world!!!";
    return 0;
}
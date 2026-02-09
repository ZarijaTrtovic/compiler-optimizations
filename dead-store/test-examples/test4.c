int main() {
    int x = 1;      // Dead - prepisuje se
    int y = 2;      // Live - koristi se u return
    int z = 3;      // Dead - nikad se ne koristi
    
    x = 10;         // Dead - nikad se ne koristi posle
    z = 30;         // Dead - nikad se ne koristi posle
    
    return y;
}
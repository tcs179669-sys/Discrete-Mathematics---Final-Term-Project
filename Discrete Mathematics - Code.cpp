#include <iostream>
#include <string>
using namespace std;


/*   INPUT VALIDATION FUNCTION
   ---------------------------------------------------------
   Makes sure the user enters a NUMBER between 1 and 9
   If they enter something wrong (like a letter), it asks again
*/
int validID() {
    int id;
    while (true) {
        cin >> id;

        if (cin.fail()) {           // user typed something that is not a number
            cin.clear();            // reset the error
            cin.ignore(1000, '\n'); // remove the wrong input from buffer
            cout << "Invalid Option. Try again: ";
        } 
        else if (id >= 1 && id <= 9) { // valid number
            return id;
        } 
        else { // number out of range
            cout << "Invalid Option. Try again: ";
        }
    }
}

/* 
   CONVERT CHARACTER TO NUMBER
   A->0, B->1, ..., Z->25
   If it's not a letter, returns -1
*/
int convertToNum(char a) {
    if (a >= 'A' && a <= 'Z') return a - 'A';
    if (a >= 'a' && a <= 'z') return a - 'a';
    return -1; // non-letter
}

/*
   CONVERT NUMBER BACK TO CHARACTER
   0->A, 1->B, ..., 25->Z
   Handles negative numbers safely
*/
char convertToChar(int a) {
    a = (a % 26 + 26) % 26; // if a is negative, make it positive
    return char('A' + a);
}

/*
   GREATEST COMMON DIVISOR (GCD)
   Returns the largest number that divides both a and b
   Used to check if keys are valid (must be coprime)
*/
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* 
   EXTENDED GCD
   Finds integers x and y such that: a*x + b*y = gcd(a,b)
   Used to find modular inverse in RSA & Affine
*/
int extendedGCD(int a, int b, int &x, int &y) {
    if (b == 0) { // base case
        x = 1;
        y = 0;
        return a;
    }

    int x1, y1;
    int g = extendedGCD(b, a % b, x1, y1);

    // update x and y using the formula
    x = y1;
    y = x1 - (a / b) * y1;

    return g;
}

/*
   FAST MODULAR EXPONENTIATION
   Calculates (base^exp) % mod efficiently
   Needed for RSA because powers get very big
*/
int modPow(int base, int exp, int mod) {
    int result = 1;        // start with 1
    base %= mod;           // in case base > mod

    while (exp > 0) {
        if (exp % 2 == 1)  // if exponent is odd
            result = (result * base) % mod;

        exp /= 2;          // divide exponent by 2
        base = (base * base) % mod; // square the base
    }

    return result;
}

/* 
   CAESAR ENCRYPTION
   Moves each letter forward by K positions
   Example: HELLO, K=3 -> KHOOR
*/
string CaesarEncrypt() {
    cout << "**********************************************************\n";
    cout << "           Welcome To Caesar Encryption                  \n";
    cout << "**********************************************************\n";

    string input, result = "";
    int K;

    cin.ignore(); // remove leftover newline from previous input
    cout << "Enter a String to Encrypt: ";
    getline(cin, input);

    cout << "Enter K: ";
    cin >> K;

    for (char c : input) {
        int p = convertToNum(c);

        if (p == -1)           // non-letter, keep it as is
            result += c;
        else
            result += convertToChar(p + K);
    }

    return result;
}

/* 
   CAESAR DECRYPTION
   Moves each letter BACK by K positions
*/
string CaesarDecrypt() {
    cout << "**********************************************************\n";
    cout << "           Welcome To Caesar Decryption                  \n";
    cout << "**********************************************************\n";

    string input, result = "";
    int K;

    cin.ignore();
    cout << "Enter a String to Decrypt: ";
    getline(cin, input);

    cout << "Enter K: ";
    cin >> K;

    for (char c : input) {
        int p = convertToNum(c);

        if (p == -1)
            result += c;
        else
            result += convertToChar(p - K);
    }

    return result;
}

/*
   AFFINE ENCRYPTION
   Formula: C = (a*P + b) mod 26
*/
string affineEncrypt() {
    cout << "**********************************************************\n";
    cout << "           Welcome To Affine Encryption                  \n";
    cout << "**********************************************************\n";

    string input, result = "";
    int a, b;

    cin.ignore();
    cout << "Enter a String to Encrypt: ";
    getline(cin, input);

    cout << "Enter a: ";
    cin >> a;
    cout << "Enter b: ";
    cin >> b;

    if (gcd(a, 26) != 1) {
        cout << "Invalid a (must be coprime with 26)\n";
        return "";
    }

    for (char c : input) {
        int p = convertToNum(c);

        if (p == -1)
            result += c;
        else
            result += convertToChar(a * p + b);
    }

    return result;
}

/* 
   AFFINE DECRYPTION
   Uses modular inverse of 'a'
*/
string affineDecrypt() {
    cout << "**********************************************************\n";
    cout << "           Welcome To Affine Decryption                  \n";
    cout << "**********************************************************\n";

    string input, result = "";
    int a, b, x, y;

    cin.ignore();
    cout << "Enter a String to Decrypt: ";
    getline(cin, input);

    cout << "Enter a: ";
    cin >> a;
    cout << "Enter b: ";
    cin >> b;

    if (gcd(a, 26) != 1) {
        cout << "Inverse does not exist\n";
        return "";
    }

    extendedGCD(a, 26, x, y);
    int invA = (x % 26 + 26) % 26; // positive modular inverse

    for (char c : input) {
        int p = convertToNum(c);

        if (p == -1)
            result += c;
        else
            result += convertToChar(invA * (p - b));
    }

    return result;
}

/*
   RSA ENCRYPTION
   Encrypts text using public key (e,n)
   Breaks text into blocks if needed
*/
void RSAEncrypt() {
    cout << "**********************************************************\n";
    cout << "           Welcome To RSA Encryption                     \n";
    cout << "**********************************************************\n";

    int p, q, e;
    cout << "Enter prime p: "; cin >> p;
    cout << "Enter prime q: "; cin >> q;

    int n = p * q;
    int phi = (p - 1) * (q - 1);

    cout << "Enter public key e: "; cin >> e;

    if (gcd(e, phi) != 1) {
        cout << "Invalid e, not coprime with phi\n";
        return;
    }

    string text;
    cout << "Enter plaintext (CAPITAL LETTERS ONLY): "; cin >> text;

    // Determine how many letters can fit in one block
    int lettersPerBlock = 1;
    while (true) {
        string maxBlock = "";
        for (int i = 0; i < lettersPerBlock; i++)
            maxBlock += "25"; // max 2-digit number per letter

        long long value = stoll(maxBlock);
        if (value < n)
            lettersPerBlock++;
        else
            break;
    }
    lettersPerBlock--; // last valid size
    cout << "\nLetters per block: " << lettersPerBlock << endl;

    // Encrypt each block
    cout << "\nEncrypted Blocks:\n";
    for (int i = 0; i < text.length(); i += lettersPerBlock) {
        string blockStr = "";
        for (int j = 0; j < lettersPerBlock && (i + j) < text.length(); j++) {
            int num = convertToNum(text[i + j]);
            if (num < 10) blockStr += "0"; // leading zero
            blockStr += to_string(num);
        }

        int m = stoi(blockStr);      // plaintext block
        int c = modPow(m, e, n);     // c = m^e mod n
        cout << c << " ";
    }
    cout << "\n";
}

/*
   RSA DECRYPTION
   Decrypts blocks back to text using private key
*/
void RSADecrypt() {
    cout << "**********************************************************\n";
    cout << "           Welcome To RSA Decryption                     \n";
    cout << "**********************************************************\n";

    int p, q, e;
    cout << "Enter prime p: "; cin >> p;
    cout << "Enter prime q: "; cin >> q;

    int n = p * q;
    int phi = (p - 1) * (q - 1);

    cout << "Enter public key e: "; cin >> e;

    // find private key d
    int x, y;
    extendedGCD(e, phi, x, y);
    int d = (x % phi + phi) % phi;

    int blockCount;
    cout << "Enter number of cipher blocks: "; cin >> blockCount;

    long long cipherBlocks[100];
    for (int i = 0; i < blockCount; i++) {
        cout << "Enter cipher block " << i + 1 << ": ";
        cin >> cipherBlocks[i];
    }

    // Decrypt blocks
    cout << "Decrypted Text: ";
    for (int i = 0; i < blockCount; i++) {
        long long m = modPow(cipherBlocks[i], d, n);
        string mStr = to_string(m);
        if (mStr.length() % 2 != 0) mStr = "0" + mStr; // make 2-digit per letter

        for (int j = 0; j < mStr.length(); j += 2) {
            int num = stoi(mStr.substr(j, 2));
            cout << convertToChar(num);
        }
    }
    cout << "\n";
}

/*
   MAIN MENU
   Shows options and calls respective functions
   Loops until user chooses to exit
*/
int main() {
    int choice;
    while (true) {
        cout << "\n******** CRYPTOGRAPHY MENU ********\n";
        cout << "1. Caesar Encrypt\n";
        cout << "2. Caesar Decrypt\n";
        cout << "3. Affine Encrypt\n";
        cout << "4. Affine Decrypt\n";
        cout << "7. RSA Encrypt\n";
        cout << "8. RSA Decrypt\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";

        choice = validID();

        switch (choice) {
            case 1: cout << CaesarEncrypt() << endl; break;
            case 2: cout << CaesarDecrypt() << endl; break;
            case 3: cout << affineEncrypt() << endl; break;
            case 4: cout << affineDecrypt() << endl; break;
            case 7: RSAEncrypt(); break;
            case 8: RSADecrypt(); break;
            case 9: return 0;
        }
    }
}

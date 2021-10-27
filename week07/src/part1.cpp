#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

static const char *CodeSegment = "This string is in the code / data segment";

int main() {
    cout << "Code segment: " << hex << &CodeSegment << endl;

    string stackString = string("This string is on the stack");
    cout << "Stack segment: " << hex << &stackString << endl;

    string *heapString = new string("This string is on the heap");
    cout << "Heap segment: " << hex << heapString << endl;

    return 0;
}

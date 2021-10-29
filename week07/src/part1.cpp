#include <iostream>
#include <string>

using namespace std;

// Static variables are stored in the code / data segment
static const char *CodeSegment = "This string is in the code / data segment";

// Main
int main() {
    // Print the address of the static variable stored in the code segment
    cout << "Code segment: " << hex << &CodeSegment << endl;

    // Allocate and print the address of a local variable stored on the stack
    string stackString = string("This string is on the stack");
    cout << "Stack segment: " << hex << &stackString << endl;

    // Allocate and print the address of a variable stored on the heap
    string *heapString = new string("This string is on the heap");
    cout << "Heap segment: " << hex << heapString << endl;

    return 0;
}

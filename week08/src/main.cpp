#include <iostream>
#include <string>

using namespace std;

/*************************************
 * ARRAY VULNERABILTY
 * 1. There must be an array and an array index variable
 * 2. The array index variable must be reachable through external input.
 * 3. There must not be bounds checking on the array index variable.
 ****************************************/
void arrayVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * ARRAY WORKING
 * Call arrayVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arrayWorking()
{
   arrayVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
 *************************************/
void arrayExploit()
{
   arrayVulnerability(/* parameters with malicious data */);
}

/*************************************
 * ARC VULNERABILTY
 * ...
 ****************************************/
void arcVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * ARC WORKING
 * Call arcVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arcWorking()
{
   arcVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * ARC EXPLOIT
 * ...
 *************************************/
void arcExploit()
{
   arcVulnerability(/* parameters with malicious data */);
}

/*************************************
 * VTABLE VULNERABILTY
 * ...
 ****************************************/
void vtableVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * VTABLE WORKING
 * Call vtableVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void vtableWorking()
{
   vtableVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * VTABLE EXPLOIT
 * ...
 *************************************/
void vtableExploit()
{
   vtableVulnerability(/* parameters with malicious data */);
}

/*************************************
 * STACK VULNERABILTY
 * ...
 ****************************************/
void stackVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * STACK WORKING
 * Call stackVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void stackWorking()
{
   stackVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * STACK EXPLOIT
 * ...
 *************************************/
void stackExploit()
{
   stackVulnerability(/* parameters with malicious data */);
}

/*************************************
 * HEAP VULNERABILTY
 * ...
 ****************************************/
void heapVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * HEAP WORKING
 * Call heapVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void heapWorking()
{
   heapVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * HEAP EXPLOIT
 * ...
 *************************************/
void heapExploit()
{
   heapVulnerability(/* parameters with malicious data */);
}

/*************************************
 * INTEGER OVERFLOW VULNERABILTY
 * ...
 ****************************************/
void intOverflowVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * INTEGER OVERFLOW WORKING
 * Call intOverflowVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void intOverflowWorking()
{
   intOverflowVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * INTEGER OVERFLOW EXPLOIT
 * ...
 *************************************/
void intOverflowExploit()
{
   intOverflowVulnerability(/* parameters with malicious data */);
}

/*************************************
 * ANSII-UNICODE VULNERABILTY
 * ...
 ****************************************/
void ansiiToUnicodeVulnerability(/* feel free to add parameters */)
{
}

/**************************************
 * ANSII-UNICODE WORKING
 * Call ansiiToUnicodeVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void ansiiToUnicodeWorking()
{
   ansiiToUnicodeVulnerability(/* parameters with non-malicious data */);
}

/**************************************
 * ANSII-UNICODE EXPLOIT
 * ...
 *************************************/
void ansiiToUnicodeExploit()
{
   ansiiToUnicodeVulnerability(/* parameters with malicious data */);
}

static const char *SectionBoundary = "********************\n";

/**********************************************
 * MAIN : The main function.
 **********************************************/
int main()
{
   cout << "Array Index" << endl;
   cout << SectionBoundary << endl;

   arrayVulnerability();
   arrayWorking();
   arrayExploit();

   cout << "ARC" << endl;
   cout << SectionBoundary << endl;

   arcVulnerability();
   arcWorking();
   arcExploit();

   cout << "VTABLE" << endl;
   cout << SectionBoundary << endl;

   vtableVulnerability();
   vtableWorking();
   vtableExploit();

   cout << "Stack Smashing" << endl;
   cout << SectionBoundary << endl;

   stackVulnerability();
   stackWorking();
   stackExploit();

   cout << "Heap Spraying" << endl;
   cout << SectionBoundary << endl;

   heapVulnerability();
   heapWorking();
   heapExploit();

   cout << "Integer Overflow" << endl;
   cout << SectionBoundary << endl;

   intOverflowVulnerability();
   intOverflowWorking();
   intOverflowExploit();

   cout << "VTABLE" << endl;
   cout << SectionBoundary << endl;

   ansiiToUnicodeVulnerability();
   ansiiToUnicodeWorking();
   ansiiToUnicodeExploit();

   return 0;
}

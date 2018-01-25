#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include "BigInteger.h"

void TestFailed()
{
    std::cerr << "Test failed" << std::endl;
    exit(1);
}

void TestEqual(const BigInteger& lhs, const BigInteger& rhs)
{
    std::cout << "Test: " << lhs << " == " << rhs << ", ";
    bool result = (lhs == rhs);
    std::cout << std::boolalpha << result << std::endl;
    if (!result)
    {
        TestFailed();
    }
}

void TestLessThan(const BigInteger& lhs, const BigInteger& rhs)
{
    std::cout << "Test: " << lhs << " < " << rhs << ", ";
    bool result = (lhs < rhs);
    std::cout << std::boolalpha << result << std::endl;
    if (!result)
    {
        TestFailed();
    }
}

template <class T, class F>
void TestException(const F& lambda)
{
    std::cout << "Test for exception, ";
    bool caught = false;
    try
    {
        lambda();
    }
    catch (const T&)
    {
        caught = true;
    }

    std::cout << std::boolalpha << caught << std::endl;
    if (!caught)
    {
        TestFailed();
    }
}

int main(int argc, char** argv)
{
    // Test default constructor
    TestEqual("0", BigInteger());

    // Test value constructor
    TestEqual("5", BigInteger((char)5));
    TestEqual("45", BigInteger((unsigned char)45));
    TestEqual("1235", BigInteger((unsigned short)1235));
    TestEqual("12895662", BigInteger((unsigned int)12895662));
    TestEqual("5248915", BigInteger((unsigned long)5248915));
    TestEqual("519205753152", BigInteger((unsigned long long)519205753152));
    TestEqual("-45", BigInteger((signed char)-45));
    TestEqual("-1235", BigInteger((signed short)-1235));
    TestEqual("-12895662", BigInteger((signed int)-12895662));
    TestEqual("-5248915342342", BigInteger((signed long)-5248915342342));
    TestEqual("-519205753152", BigInteger((signed long long)-519205753152));

    // Test unary plus
    TestEqual("3905142830651", +BigInteger(3905142830651));         // +(+ve)
    TestEqual("-18509176298765", +BigInteger(-18509176298765));     // +(-ve)

    // Test unary minus
    TestEqual("-651892736454", -BigInteger(651892736454));          // -(+ve)
    TestEqual("7165981724659812", -BigInteger(-7165981724659812));   // -(-ve)

    // Test addition
    TestEqual("10384065733686", BigInteger(5819083357103) + BigInteger(4564982376583));                     // (+ve) + (+ve)
    TestEqual("57576635029", BigInteger(999235781624) + BigInteger(-941659146595));                         // (+ve) + (-ve)
    TestEqual("-353760973370", BigInteger(-618274569182) + BigInteger(264513595812));                       // (-ve) + (+ve)
    TestEqual("-175279796602675", BigInteger(-76823658126132) + BigInteger(-98456138476543));               // (-ve) + (-ve)
    TestEqual("0", BigInteger(-56137569187354) + BigInteger(56137569187354));                               // zero trimming
    TestEqual("69874561941049", BigInteger(12314) + BigInteger(69874561928735));                            // lhs < rhs
    TestEqual("12837544873758", BigInteger(12837501927635) + BigInteger(42946123));                         // lhs > rhs

    // Test subtraction
    TestEqual("1254100980520", BigInteger(5819083357103) - BigInteger(4564982376583));                      // (+ve) + (+ve)
    TestEqual("1940894928219", BigInteger(999235781624) - BigInteger(-941659146595));                       // (+ve) + (-ve)
    TestEqual("-882788164994", BigInteger(-618274569182) - BigInteger(264513595812));                       // (-ve) + (+ve)
    TestEqual("21632480350411", BigInteger(-76823658126132) - BigInteger(-98456138476543));                 // (-ve) + (-ve)
    TestEqual("1", BigInteger(100000000000000) - BigInteger(99999999999999));                               // borrowing
    TestEqual("-69874561916421", BigInteger(12314) - BigInteger(69874561928735));                           // lhs < rhs
    TestEqual("12837458981512", BigInteger(12837501927635) - BigInteger(42946123));                         // lhs > rhs

    // Test multiplication
    TestEqual("26564012973042635013919049", BigInteger(5819083357103) * BigInteger(4564982376583));         // (+ve) + (+ve)
    TestEqual("-940939513371243623170280", BigInteger(999235781624) * BigInteger(-941659146595));           // (+ve) + (-ve)
    TestEqual("-163542029493445979465784", BigInteger(-618274569182) * BigInteger(264513595812));           // (-ve) + (+ve)
    TestEqual("7563760722741050112617321676", BigInteger(-76823658126132) * BigInteger(-98456138476543));   // (-ve) + (-ve)
    TestEqual("0", BigInteger(100000000000000) * BigInteger(0));                                            // zero multiplication
    TestEqual("860435355590442790", BigInteger(12314) * BigInteger(69874561928735));                        // lhs < rhs
    TestEqual("551320936796949809105", BigInteger(12837501927635) * BigInteger(42946123));                  // lhs > rhs

    // Test division
    TestEqual("89", BigInteger(5819083357103) / BigInteger(64982376583));                                   // (+ve) + (+ve)
    TestEqual("-602", BigInteger(999235781624) / BigInteger(-1659146595));                                  // (+ve) + (-ve)
    TestEqual("-2", BigInteger(-618274569182) / BigInteger(264513595812));                                  // (-ve) + (+ve)
    TestEqual("0", BigInteger(-76823658126132) / BigInteger(-98456138476543));                              // (-ve) + (-ve)
    TestEqual("0", BigInteger(12314) / BigInteger(69874561928735));                                         // lhs < rhs
    TestEqual("298921", BigInteger(12837501927635) / BigInteger(42946123));
    TestEqual("10000000", BigInteger(10000000) / BigInteger(1));                                 // lhs > rhs
    TestException<std::logic_error>([]() { BigInteger(1) / BigInteger(0); });                               // test exception

    // Test modulus
    TestEqual("35651841216", BigInteger(5819083357103) % BigInteger(64982376583));                         // (+ve) + (+ve)
    TestEqual("429531434", BigInteger(999235781624) % BigInteger(-1659146595));                           // (+ve) + (-ve)
    TestEqual("-89247377558", BigInteger(-618274569182) % BigInteger(264513595812));                        // (-ve) + (+ve)
    TestEqual("-192394066908", BigInteger(-76823658126132) % BigInteger(-456138476543));                    // (-ve) + (-ve)
    TestEqual("12314", BigInteger(12314) % BigInteger(69874561928735));                                     // lhs < rhs
    TestEqual("3894352", BigInteger(12837501927635) % BigInteger(42946123));                                // lhs > rhs
    TestException<std::logic_error>([]() { BigInteger(1) % BigInteger(0); });                               // test exeption

    // Test digit shift left
    TestEqual("462000", BigInteger(462) << 3);
    TestEqual("-156400000", BigInteger(-1564) << 5);
    TestEqual("0", BigInteger(0) << 10);
    TestEqual("42", BigInteger(42) << 0);

    // Test digit shift right
    TestEqual("457", BigInteger(45700) >> 2);
    TestEqual("-189", BigInteger(-1891265481) >> 7);
    TestEqual("0", BigInteger(3156436) >> 10);
    TestEqual("0", BigInteger(0) >> 0);

    // Test less than
    TestLessThan(BigInteger(13758135), BigInteger(135763987561));
    TestLessThan(BigInteger(-31835987), BigInteger(3746189365535));
    TestLessThan(BigInteger(-13547189365981), BigInteger(-43852945));

    return 0;
}
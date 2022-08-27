#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <chrono>

std::mutex ibanLock;
std::vector<int> validnrs;
bool stop_threads = false;

int validNumCount =0;
int numCount = 1;
bool checkIban(int ibannr, int m)
{
    int sum = 0;
    // convert to string for easy manipulation
    std::string s = std::to_string(ibannr);
    int length = s.length();
    for(int i = 0; i < length; i++)
    {   
        int c = s[i];
        // weight to calculate the sum. Weight is the position of the char
        int w = length - i;
        sum += (c* w);
    }
    if(sum % m == 0){
        return true;
    }

    return false;
}

// b: start range
// e: end range
// m: modulus
// u: modus
// s optional find number.

void _checkIban(int b, int e, int m, int u, int s)
{

    for(int i = b; i < e; i++)
    {
        if(stop_threads)
        {
            return;
        }

        if(u == 2)
        {   
            // this is inefficient, @Gert we had email contact about this (13-06-2022)
            if(i == s)
            {
                std::cout << i << "is" << (checkIban(s, m) ? "valid" : "invalid");
            }
        }

        if(checkIban(i, m)){
            if(u == 0){
                ibanLock.lock();
                validNumCount++;
                ibanLock.unlock();
            }
            if(u == 1){
                
                ibanLock.lock();
                std::cout << numCount << " " << i << std::endl;
                numCount++;
                ibanLock.unlock();
            }
        }
    }
}

// b: start range
// e: end range
// m: modulus
// p: thread count.
// u: modus
// s optional find number.
void checkIbans(int b, int e, int m, int p, int u, int s)
{

    std::vector<std::thread> threads;

    int totalItems = e - b;
    int threadSpread = totalItems / p;
    int startIndex = b;
    int endIndex = startIndex + threadSpread; 

    int rest = totalItems % p;

    for(int i = 1; i <= p; i++)
    {
        // divide the rest over the threads // even distribution:

        //E.G: 20 items, 3 threads
        // thread 1: 7 items
        // thread 2: 7 items
        // thread 3: 6 items

        if(rest > 0)
        {
            endIndex +=1;
            rest--;
        }

        threads.emplace_back(_checkIban, startIndex, endIndex, m, u, s);
        startIndex = endIndex;
        endIndex += threadSpread;
    }

    for (auto& t : threads)  
    {
        if (t.joinable()) {
            t.join();
        }
    }

    if(u == 0)
    {
        std::cout << validNumCount << std::endl;
    }

}

int main () {
    
    // assign thread
    const unsigned int maxThreads = std::thread::hardware_concurrency() * 2;
    auto startTimeMulti = std::chrono::high_resolution_clock::now();
    checkIbans(274856170, 274856190, 11, maxThreads, 0, 0);
    auto endTimeMulti = std::chrono::high_resolution_clock::now();

    std::cout << "multithread duration time: " << std::chrono::duration<double, std::milli>(endTimeMulti-startTimeMulti).count() << "ms\n";
    
    return 0;
}

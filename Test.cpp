#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>
#include <math.h>
#include <vector>
#include <stdlib.h>


std::vector<unsigned int> primeVect;
std::mutex primeVectLock;


int isPrime(int num) 
{
    if (num <=1)
        return false;

    for (int i = 2; i <= sqrt(num); i++)
    {
        if ( num % i == 0 ) 
            return false;
    }

    return true;
}

void FindPrimes(int start, int end)
{
    for(unsigned int x = start; x <= end; x +=1) {
        
        if(isPrime(x))
        {
            primeVectLock.lock();
            primeVect.push_back(x);
            primeVectLock.unlock();
        }
        
    }
}

void FindPrimesThreaded(int start, int end, int numThreads)
{
    std::vector<std::thread> threads;

    // determine how much each thread needs to calculate.
    unsigned int blockEnd = end / numThreads;
    unsigned int newEnd = start + blockEnd - 1;

    for ( unsigned int x = 0; x < numThreads; x++)
    {
        threads.emplace_back(FindPrimes, start, newEnd);
        start += blockEnd;
        newEnd += blockEnd;

    }

    for (auto& t : threads)  
    {
        if (t.joinable()) {
            t.join();
        }
    }


}

int main () 
{

    int startTime = clock();

    int input =  0;


    int count = 0;

    std::cout << "Voer in voor welk getal je de priemgetallen wilt uitrekenen" << std::endl;
    std::cin >> input;
    system ("clear");

    FindPrimesThreaded(0, input, 2);

    int endTime = clock();
    double execTime = ( endTime - startTime  )/ double(CLOCKS_PER_SEC);

    // std::cout << primeVect.size() << std::endl;

    for (auto& prime : primeVect)
    {
        std::cout << prime << std::endl;
    }

    std::cout << "Execution time: " << execTime << std::endl;


}

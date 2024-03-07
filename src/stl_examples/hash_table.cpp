// C++ program to determine worst case
// time complexity of an unordered_map

#include <spdlog/spdlog.h>

#include <chrono>
#include <unordered_map>
#include <iostream>

using namespace std;
using namespace std::chrono;
int N = 55000;
int prime1 = 107897;
int prime2 = 126271;

void insert(int prime)
{

    // Starting the clock

    unordered_map<int, int> umap;
    spdlog::info("bucket count {}", umap.bucket_count());
    spdlog::info("max bucket count {}", umap.max_bucket_count());
    spdlog::info("load_factor {}", umap.load_factor());
    spdlog::info("max_load_factor {}", umap.max_load_factor());

    // Inserting multiples of prime
    // number as key in the map
    auto start = high_resolution_clock::now();
    for (int i = 1; i <= N; i++) umap[i * prime] = i;
    auto stop = high_resolution_clock::now();

    spdlog::info("bucket count {}", umap.bucket_count());
    spdlog::info("max_bucket_count {}", umap.max_bucket_count());
    spdlog::info("load_factor {}", umap.load_factor());
    spdlog::info("max_load_factor {}", umap.max_load_factor());

    // Stopping the clock

    // Typecasting the time to
    // milliseconds
    auto duration = duration_cast<milliseconds>(stop - start);

    // Time in seconds
    cout << "for " << prime << " : " << duration.count() / 1000.0 << " seconds "
         << endl;
}

// Driver code
int main()
{
    // Function call for prime 1
    insert(prime1);

    // Function call for prime 2
    insert(prime2);
}

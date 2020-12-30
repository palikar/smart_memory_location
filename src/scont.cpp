#include <iostream>

#include "memory.hpp"
#include "allocators.hpp"

#include "containers/vector.hpp"
#include "containers/map.hpp"
#include "containers/string.hpp"

struct Name
{
    int a;
    float b;
    unsigned c;

};


int main()
{
    MainAllocator globalAllocator;
    TempAllocator tempAllocator;
    Memory::init_memory(Megabytes(1), Megabytes(128));
    Memory::init_allocators(&globalAllocator, &tempAllocator);
    DEFER { Memory::destroy();};
    
    Map<String, float> map;
    map.init();
 
    map.push_back("Stanislav", 42);
    map.push_back("Marina", 44);
    map.push_back("Marina_1", 44);
    map.push_back("Marina_2", 445);
    map.push_back("Marina_3", 446);
    map.push_back("Marina_4", 447);
    map.push_back("Marina_5", 448);
    map.push_back("Marina_6", 449);
    map.push_back("Marina_7", 4410);
    map.push_back("Marina_8", 4411);

    std::cout << "............" << "\n";
    
    std::cout << map["Stanislav"] << '\n';
    std::cout << map["Marina"] << '\n';
    std::cout << map["Marina_1"] << '\n';
    std::cout << map["Marina_2"] << '\n';
    std::cout << map["Marina_3"] << '\n';
    std::cout << map["Marina_4"] << '\n';
    std::cout << map["Marina_5"] << '\n';
    std::cout << map["Marina_6"] << '\n';
    std::cout << map["Marina_7"] << '\n';
    std::cout << map["Marina_8"] << '\n';

    

    // auto it = map.first();
    // size_t count{1};
    // while (it) {
    //     auto& [key, value] = *it;
    //     std::cout << count++ << " --> " << key.data() << " : " << value << "\n";
    //     it = map.next(it);
    // };

    
	// Vector<int> intVec;
	// intVec.init();
	        
	// for (size_t i = 0; i < 34; ++i)
	// {
	//     intVec.push_back(i);
	// }
	
	// intVec.erase(2);
	// intVec.erase(intVec.data() + 2);
	
	// for (size_t i = 0; i < intVec.count(); ++i)
	// {
	//     std::cout << intVec[i] << "\n";
	// }

    return 0;
}

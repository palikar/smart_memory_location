#include <iostream>

#include "memory.hpp"
#include "allocators.hpp"
#include "containers.hpp"

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

    StringMap<int> m;
    m.init();

    m.push_back("Stanislav", 32);

    std::cout << m["Stanislav"] << "\n";

    // List<int> ints;
    // ints.init();

    // ints.push_back(43);
    // ints.push_back(44);
    // ints.push_back(45);

    // std::cout << ints[0] << "\n";
    // std::cout << ints[1] << "\n";
    // std::cout << ints[2] << "\n";

    // std::cout << ints.front() << "\n";
    // std::cout << ints.back() << "\n";

    // ints.destroy();

// TempVector<int> ints;
    // ints.init();

    // ints.push_back(23);
    // ints.push_back(24);
    // ints.push_back(25);
    // ints.push_back(26);

    // for (size_t i = 0; i < ints.count(); ++i)
	// {
	//     std::cout << ints[i] << "\n";
	// }

    // ints.destory();

    // TempString tr{"dslkaö"};
    // std::cout << tr.data() << "\n";
    // tr.destory();
    

    // StringView view{"this is sick"};
    // std::cout << view.data() << "\n";

    // std::cout << hash<StringView>::hash_value(view) << "\n";

    // String str{"this is so sick!"};
    // std::cout << str.data() << "\n";
    // str.destory();
    
    // TempMap<int, float> map;
    // map.init();
 
    // map.push_back(2, 2.3);
    // map.push_back(3, 23.3);
    // map.push_back(4, 42.3);
    // map.push_back(5, 52.3);

    // std::cout << map[2] << "\n";
    // std::cout << map[3] << "\n";
    // std::cout << map[4] << "\n";
    // std::cout << map[5] << "\n";

    // std::cout << "............" << "\n";
    
    // std::cout << map["Stanislav"] << '\n';
    // std::cout << map["Marina"] << '\n';
    // std::cout << map["Marina_1"] << '\n';
    // std::cout << map["Marina_2"] << '\n';
    // std::cout << map["Marina_3"] << '\n';
    // std::cout << map["Marina_4"] << '\n';
    // std::cout << map["Marina_5"] << '\n';
    // std::cout << map["Marina_6"] << '\n';
    // std::cout << map["Marina_7"] << '\n';
    // std::cout << map["Marina_8"] << '\n';

    

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

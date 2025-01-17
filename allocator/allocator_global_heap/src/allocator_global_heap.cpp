#include <not_implemented.h>

#include <format>

#include "../include/allocator_global_heap.h"

#include <sstream>

#include <iomanip>

#include <cmath>

allocator_global_heap::allocator_global_heap(logger* logger) : _logger(logger)
{
    information_with_guard("Global heap allocator: Constructor called.");
}

allocator_global_heap::~allocator_global_heap()
{
    information_with_guard("Global heap allocator: Destructor called. Bye-bye...");
}

allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) noexcept : _logger(other._logger)
{
    information_with_guard("Global heap allocator move constructor called");
}

allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&other) noexcept
{
    _logger = other._logger;
    information_with_guard("Global heap allocator move assignment called");

    return* this;
}

[[nodiscard]] void* allocator_global_heap::allocate( // * ALLOCATE
    size_t value_size,
    size_t values_count)
{
    const std::size_t meta = sizeof(size_t) + sizeof(allocator*);
    const std::size_t result_size = value_size * values_count + meta;

    information_with_guard("Global heap allocator: Trying to allocate " + std::to_string(result_size) + " bytes...");

    void* ptr = nullptr;

    try {
        ptr = ::operator new(result_size);
    } catch (const std::bad_alloc& e) {
        error_with_guard("Global heap allocator: Failed to allocate " + std::to_string(result_size) + " bytes");
        throw;

        return nullptr;
    }

    std::size_t* size_ptr = reinterpret_cast<std::size_t*>(ptr);
    *size_ptr = result_size - meta; //*  remaining memory size* 

    information_with_guard("Global heap allocator: Allocated " + std::to_string(result_size) + " bytes successfully");

    void** alloc_ptr = reinterpret_cast<void**>(static_cast<char*>(ptr) + sizeof(std::size_t));
    *(alloc_ptr) = this;

    void* user_ptr = reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) + meta);

    debug_with_guard("Global heap allocator: Memory dump: " + get_mem_dump(user_ptr));
    // std::cout << "MEMORY DUMP: " << get_mem_dump(user_ptr) << std::endl;

    return user_ptr;
}

void allocator_global_heap::deallocate( // * DEALLOCATE
    void* at)
{
    if (at == nullptr) {
        error_with_guard("Global heap allocator: nullptr pointer passed to deallocate()");
        std::invalid_argument("nullptr pointer passed to deallocate()"); 
        return;
    }

    information_with_guard("Global heap allocator: Trying to deallocate " + std::to_string(get_block_size(at)) + " bytes...");
    debug_with_guard("Global heap allocator: Memory dump: " + get_mem_dump(at));
    
    void** alloc_obj = reinterpret_cast<void**>(static_cast<char*>(at) - sizeof(allocator*));

    if (*alloc_obj != this) {
        error_with_guard("Global heap allocator: Trying to deallocate memory allocated BY OTHER ALLOCATOR");
        std::logic_error("Trying to deallocate memory allocated BY OTHER ALLOCATOR");
        return;
    }
    
    ::operator delete(reinterpret_cast<void*>(static_cast<char*>(at) - sizeof(std::size_t) - sizeof(allocator*)));
}

inline logger* allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return typeid(*this).name();
}

std::size_t allocator_global_heap::get_block_size(void* ptr) const noexcept
{
    return sizeof(*reinterpret_cast<char*>(ptr));
}

std::string allocator_global_heap::get_mem_dump(void* ptr) const noexcept
{
    std::ostringstream my_stream;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(ptr);

    my_stream << std::hex << std::setfill('0');

    std::size_t size = get_block_size(ptr);

    for (std::size_t i = 0; i < size; ++i) {
        my_stream << std::setw(2) << static_cast<unsigned>(bytes[i]) << ' ';
    }

    return my_stream.str();
}

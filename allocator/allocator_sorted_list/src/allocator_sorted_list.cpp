#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"
#include <allocator_sorted_list.h>

// * RULE OF FIVE * //

// * DONE
allocator_sorted_list::~allocator_sorted_list() // destructor 
{
    information_with_guard("Allocator sorted list: Destructor called. Bye-bye...");

    logger* lggr = get_logger();
    if (lggr != nullptr) {
        lggr->~logger(); // ! delete logger
    }

    std::mutex* mut = get_mutex();
    mut->~mutex(); // ! delete mutex

    deallocate_with_guard(_trusted_memory); // ! deallocate memory
}

// allocator_sorted_list::allocator_sorted_list(allocator_sorted_list&& other) noexcept
//     : _trusted_memory(std::move(other._trusted_memory)),
//       _logger(std::move(other._logger)) { // move constructor

//     information_with_guard("Allocator sorted list: Move constructor called.");
// }

// allocator_sorted_list &allocator_sorted_list::operator=(  // move assignment
//     allocator_sorted_list &&other) noexcept
// {
//     information_with_guard("Allocator sorted list: Move assignment called.");

//     _trusted_memory = std::move(other._trusted_memory);
//     _logger = std::move(other._logger);
    
//     return *this;
// }

// * CONSTRUCTOR * //
allocator_sorted_list::allocator_sorted_list( // ?? ?? ?? ? ?? ? ??  ?? ? ? ?
    size_t space_size,
    allocator* parent_allocator,
    logger* logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    information_with_guard("Allocator sorted list: Constructor called.");

    size_t size = space_size + allocator_meta;

    if (parent_allocator == nullptr) {
        _trusted_memory = ::operator new(size);
    } else {
        _trusted_memory = parent_allocator->allocate(size, 1);
    }

    if (_trusted_memory == nullptr) {
        error_with_guard("Allocator sorted list: Trusted memory is nullptr.");
        throw std::bad_alloc();
    }

    // * allocator -> logger -> fit_mode -> mutex -> size_ * //

    auto allctr = reinterpret_cast<allocator**>(_trusted_memory);
    *allctr = parent_allocator;

    auto lggr = reinterpret_cast<class logger**>(allctr + 1);
    *lggr = logger;

    auto fit_mode = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(lggr + 1);
    *fit_mode = allocate_fit_mode;

    auto mutex = reinterpret_cast<std::mutex**>(fit_mode + 1);
    *mutex = new std::mutex(); // ! check correctness

    auto size_ = reinterpret_cast<size_t*>(mutex + 1);
    *size_ = space_size;

    auto ptr = reinterpret_cast<void**>(size_ + 1); // * head block kind of = start of free mem pool *
    *ptr = reinterpret_cast<void*>(ptr + 1);
}

// * ALLOCATE DEALLOCATE * //
[[nodiscard]] void* allocator_sorted_list::allocate( // TODO:
    size_t value_size,
    size_t values_count)
{
    std::mutex* mut = get_mutex();
    std::lock_guard<std::mutex> lock(*mut); // ! check correctness

    size_t allocation_size = value_size * values_count + block_meta; // ! ALREADY WITH BLOCK META

    std::string msg = "Allocator sorted list: Trying to allocate " + std::to_string(allocation_size) + " bytes...";
    information_with_guard(msg);

    void** ptr_fi = get_first_mem_block();
    void** ptr_next = nullptr;
    void** ptr_prev = nullptr;
    void** ptr_best = nullptr;
    void** ptr_best_prev = nullptr;

    size_t cnt = 0, size_ptr_fi = get_block_size(ptr_fi);

    auto fit_mode = get_fit_mode();

    while (*ptr_fi != nullptr) {
        if (fit_mode == allocator_with_fit_mode::fit_mode::first_fit) { // * FIRST FIT *
            if (size_ptr_fi >= allocation_size) {
                cnt++;
                break;
            }
        } else if (fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit) { // * THE BEST FIT *
            if (size_ptr_fi >= allocation_size) {
                cnt++;
                if (ptr_best == nullptr) {
                    ptr_best = ptr_fi;
                    ptr_best_prev = ptr_prev;
                } else if (size_ptr_fi < get_block_size(ptr_best)) {
                    ptr_best = ptr_fi;
                    ptr_best_prev = ptr_prev;
                }
            }
        } else if (fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit) { // * THE WORST FIT *
            if (size_ptr_fi >= allocation_size) {
                cnt++;
                if (ptr_best == nullptr) {
                    ptr_best = ptr_fi;
                    ptr_best_prev = ptr_prev;
                } else if (size_ptr_fi > get_block_size(ptr_best)) {
                    ptr_best = ptr_fi;
                    ptr_best_prev = ptr_prev;
                }
            }
        }   
    }

    // ? why one more time
    if (fit_mode == allocator_with_fit_mode::fit_mode::first_fit) { // * FIRST FIT *
        if (size_ptr_fi >= allocation_size) {
            cnt++;
        }
    } else if (fit_mode == allocator_with_fit_mode::fit_mode::the_best_fit) { // * THE BEST FIT *
        if (size_ptr_fi >= allocation_size) {
            cnt++;
            if (ptr_best == nullptr) {
                ptr_best = ptr_fi;
                ptr_best_prev = ptr_prev;
            } else if (size_ptr_fi < get_block_size(ptr_best)) {
                ptr_best = ptr_fi;
                ptr_best_prev = ptr_prev;
            }
        }
    } else if (fit_mode == allocator_with_fit_mode::fit_mode::the_worst_fit) { // * THE WORST FIT *
        if (size_ptr_fi >= allocation_size) {
            cnt++;
            if (ptr_best == nullptr) {
                ptr_best = ptr_fi;
                ptr_best_prev = ptr_prev;
            } else if (size_ptr_fi > get_block_size(ptr_best)) {
                ptr_best = ptr_fi;
                ptr_best_prev = ptr_prev;
            }
        }
    } 

    if (cnt == 0) {
        error_with_guard("Allocator sorted list: allocation failed");
        throw std::bad_alloc();

        return nullptr; // ? do i need this though
    }

    if (ptr_best != nullptr) {
        ptr_fi = ptr_best;
        ptr_prev = ptr_best_prev;
    }

    if (size_ptr_fi == allocation_size) {
        if (*ptr_fi == nullptr) {
            if (ptr_prev != nullptr) {
                *ptr_prev = nullptr;
            } else {
                auto tmp_ptr = get_first_mem_block();
                *tmp_ptr = nullptr;
            }
        } else {
            ptr_next = reinterpret_cast<void**>(*ptr_fi);
            if (ptr_prev != nullptr) {
                *ptr_prev = ptr_next;
            } else {
                auto tmp_ptr = get_first_mem_block();
                *tmp_ptr = ptr_next;
            }
        }
    } else {
        if (*ptr_fi != nullptr) {
            ptr_next = reinterpret_cast<void**>(allocation_size + reinterpret_cast<char*>(get_next_block(ptr_fi)));
            *ptr_next = reinterpret_cast<void**>(*ptr_fi);

            size_t* sze_nxt = reinterpret_cast<size_t*>(get_block_size(ptr_next)); // * :P

            *sze_nxt = get_block_size(ptr_fi) - allocation_size;

            *(reinterpret_cast<size_t*>(get_block_size(ptr_fi))) = value_size * values_count; // !!!!!!! danger

            if (ptr_prev != nullptr) {
                *ptr_prev = ptr_next;
            } else {
                auto tmp_ptr = get_first_mem_block();
                *tmp_ptr = reinterpret_cast<void**>(ptr_next);
            }
        } else {
            ptr_next = reinterpret_cast<void**>(value_size * values_count + reinterpret_cast<char*>(get_next_block(ptr_fi)));
            *ptr_next = nullptr;
            
            size_t* sze_nxt = reinterpret_cast<size_t*>(get_block_size(ptr_next)); // * :P

            *sze_nxt = get_block_size(ptr_fi) - allocation_size;

            *(reinterpret_cast<size_t*>(get_block_size(ptr_fi))) = value_size * values_count; // !!!!!!! danger

            if (ptr_prev != nullptr) {
                *ptr_prev = reinterpret_cast<void**>(ptr_next);
            } else {
                auto tmp_ptr = get_first_mem_block();
                *tmp_ptr = reinterpret_cast<void**>(ptr_next);
            }
        }
    }

    information_with_guard("Allocator sorted list: allocation successful");

    return get_next_block(ptr_fi);
}


void allocator_sorted_list::deallocate( // TODO:
    void *at)
{
    if (at == nullptr) {
        error_with_guard("Allocator sorted list: nullptr pointer passed to deallocate()");
        return;
    }

    std::mutex* mut = get_mutex();
    std::lock_guard<std::mutex> lock(*mut);
    
    void** ptr = reinterpret_cast<void**>(reinterpret_cast<size_t*>(at) - 1);
    ptr = reinterpret_cast<void**>(ptr - 1);

    size_t sze = get_block_size(ptr);

    void** ptr_prev = nullptr;
    void** ptr_next = nullptr;

    ptr_next = get_first_mem_block();

    if (ptr_next != nullptr) {

        while (ptr_next != ptr and *ptr_next != nullptr) {
            ptr_prev = ptr_next;
            ptr_next = reinterpret_cast<void**>(*ptr_next);
        }

        if (ptr_next < ptr and *ptr_next == nullptr) {
            ptr_prev = ptr_next;
            ptr_next = nullptr;
        }   

        if (ptr_next == nullptr) { // TODO:
            // if (reinterpret_cast<void**>(reinterpret_cast<char*>((reinterpret_cast<size_t*>()))))
        } else {

        }
    } else {
        *ptr = nullptr;
        *reinterpret_cast<void**>(get_first_mem_block()) = reinterpret_cast<void**>(ptr); // !!!!
    }


    // * just try catch
}

// * SETTERS GETTERS * //
inline void allocator_sorted_list::set_fit_mode( // * DONE
    allocator_with_fit_mode::fit_mode mode)
{
    std::mutex* mut = get_mutex();
    std::lock_guard<std::mutex> lock(*mut);

    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(*reinterpret_cast<size_t*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator*) + 1) = mode;
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    return *reinterpret_cast<allocator**>(_trusted_memory); // ! check if +1 is needed
}

// TODO:
std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    information_with_guard("Allocator sorted list: get_blocks_info() called.");

    std::vector<allocator_test_utils::block_info> vector;

    return vector;
}

inline logger *allocator_sorted_list::get_logger() const // * after trustmem - alloc 
{
    information_with_guard("Allocator sorted list: get_logger() called.");
    return *reinterpret_cast<logger**>(reinterpret_cast<size_t*>(_trusted_memory) + sizeof(allocator*) + 1);
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    information_with_guard("Allocator sorted list: get_typename() called.");
    return typeid(*this).name();
}

std::mutex* allocator_sorted_list::get_mutex() const 
{
    information_with_guard("Allocator sorted list: get_mutex() called.");
    return *reinterpret_cast<std::mutex**>(reinterpret_cast<size_t*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode*) + 1);
}

inline allocator_with_fit_mode::fit_mode allocator_sorted_list::get_fit_mode() const 
{
    information_with_guard("Allocator sorted list: get_fit_mode() called.");
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(*reinterpret_cast<size_t*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator*) + 1);
}

void** allocator_sorted_list::get_first_mem_block() 
{
    return reinterpret_cast<void**>(*reinterpret_cast<size_t*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode*) + sizeof(std::mutex*) + 1);
}

size_t allocator_sorted_list::get_block_size(void** ptr) // ! without block meta
{
    return *reinterpret_cast<size_t*>(sizeof(ptr)); 
}

void* allocator_sorted_list::get_next_block(void** ptr) 
{
    return reinterpret_cast<void*>(*reinterpret_cast<size_t*>(ptr) + 1);
}

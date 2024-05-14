#include <not_implemented.h>
#include <cstddef>
#include "../include/allocator_sorted_list.h"
#include "../include/allocator_test_utils.h"

allocator_sorted_list::~allocator_sorted_list()
{
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
}

allocator_sorted_list::allocator_sorted_list(
    allocator_sorted_list &&other) noexcept
{
    _trusted_memory = std::exchange(other._trusted_memory, nullptr);
}

allocator_sorted_list &allocator_sorted_list::operator=(
    allocator_sorted_list &&other) noexcept
{
    std::swap(_trusted_memory, other._trusted_memory);
    return *this;
}

allocator_sorted_list::allocator_sorted_list(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (space_size < block_metadata_size)
    {
        throw std::logic_error("Too small space");
    }

    size_t real_size = space_size + allocator_metadata_size;

    _trusted_memory = (parent_allocator == nullptr) ? ::operator new(real_size) : parent_allocator->allocate(real_size, 1);

    auto logger_ptr = reinterpret_cast<class logger**>(_trusted_memory);

    *logger_ptr = logger;

    auto parent_allocator_ptr = reinterpret_cast<allocator**>(logger_ptr + 1);

    *parent_allocator_ptr = parent_allocator;

    auto fit_mode_ptr = reinterpret_cast<allocator_with_fit_mode::fit_mode*>(parent_allocator_ptr + 1);

    *fit_mode_ptr = allocate_fit_mode;

    auto size_ptr = reinterpret_cast<size_t*>(fit_mode_ptr + 1);

    *size_ptr = space_size;

    auto mutex_ptr = reinterpret_cast<std::mutex*>(size_ptr + 1);

    new(mutex_ptr) std::mutex();

    auto first_block_ptr = reinterpret_cast<void**>(mutex_ptr + 1);

    *first_block_ptr = reinterpret_cast<std::byte*>(_trusted_memory) + allocator_metadata_size;

    auto block_forward_ptr = reinterpret_cast<void**>(first_block_ptr + 1);

    *block_forward_ptr = nullptr;

    auto block_size_ptr = reinterpret_cast<size_t*>(block_forward_ptr + 1);

    *block_size_ptr = space_size - block_metadata_size;
}

[[nodiscard]] void *allocator_sorted_list::allocate(size_t value_size, size_t values_count)
{
    std::lock_guard lock(get_mutex());

    size_t real_size = value_size * values_count;

    debug_with_guard("Allocator sorted list started allocating " + std::to_string(real_size) + " bytes");

    void* prev_free;

    switch (get_fit_mod())
    {
        case allocator_with_fit_mode::fit_mode::first_fit:
            prev_free = get_first(real_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            prev_free = get_best(real_size);
            break;
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            prev_free = get_worst(real_size);
            break;
    }

    if (prev_free == nullptr)
    {
        error_with_guard("Allocator sorted list throwing bad_alloc while trying to allocate " + std::to_string(real_size) + " bytes");
        throw std::bad_alloc();
    }

    void* free_block_start = prev_free == _trusted_memory ? *get_first_block_ptr(_trusted_memory) : get_ptr_from_block(prev_free);

    size_t free_block_size = get_block_size(free_block_start);

    bool need_fraq = true;

    if (free_block_size < real_size + block_metadata_size)
    {
        warning_with_guard("Allocator sorted list changed allocating block size to " + std::to_string(free_block_size));
        real_size = free_block_size;
        need_fraq = false;
    }

    if (need_fraq)
    {
        void* new_free_block = reinterpret_cast<std::byte*>(free_block_start) + block_metadata_size + real_size;
        (prev_free == _trusted_memory ? *get_first_block_ptr(_trusted_memory) : *reinterpret_cast<void **>(prev_free)) = new_free_block;
        *reinterpret_cast<void **>(new_free_block) = get_ptr_from_block(free_block_start);
        auto size_ptr = reinterpret_cast<size_t*>(reinterpret_cast<std::byte*>(new_free_block) + sizeof(void*));
        *size_ptr = free_block_size - real_size - block_metadata_size;
        size_ptr = reinterpret_cast<size_t*>(reinterpret_cast<std::byte*>(free_block_start) + sizeof(void*));
        *size_ptr = real_size;
    } else
    {
        (prev_free == _trusted_memory ? *get_first_block_ptr(_trusted_memory) : *reinterpret_cast<void **>(prev_free)) = get_ptr_from_block(free_block_start);
    }

    *reinterpret_cast<void**>(free_block_start) = _trusted_memory;

    information_with_guard(std::to_string(get_free_size()));
    // debug_with_guard(print_blocks());

    return reinterpret_cast<std::byte*>(free_block_start) + block_metadata_size;
}

void allocator_sorted_list::deallocate(void *at)
{
    std::lock_guard lock(get_mutex());

    void* block_start = reinterpret_cast<std::byte*>(at) - block_metadata_size;

    if (get_ptr_from_block(block_start) != _trusted_memory)
    {
        error_with_guard("Incorrect deallocation object");
        throw std::logic_error("Incorrect deallocation object");
    }

    size_t block_size = get_block_size(block_start);

    // debug_with_guard(get_dump((char*)at, block_size));

    std::vector<void*> free_blocks = get_free_blocks();
    void* prev_free = _trusted_memory;
    void* next_free = nullptr;

    for (auto free_block : free_blocks)
    {
        if (free_block < block_start)
        {
            prev_free = free_block;
        }
        if (free_block > block_start)
        {
            next_free = free_block;
            break;
        }
    }

    if (prev_free != _trusted_memory && reinterpret_cast<std::byte*>(prev_free) + get_block_size(prev_free) + block_metadata_size == block_start)
    {
        *reinterpret_cast<void**>(prev_free) = next_free;
        *reinterpret_cast<size_t*>(reinterpret_cast<std::byte*>(prev_free) + sizeof(void*)) += block_size + block_metadata_size;
        block_start = prev_free;
    } else if (prev_free != _trusted_memory)
    {
        *reinterpret_cast<void**>(prev_free) = block_start;
    } else
    {
        *get_first_block_ptr(_trusted_memory) = block_start;
    }

    if (next_free != nullptr && reinterpret_cast<std::byte*>(block_start) + get_block_size(block_start) + block_metadata_size == next_free)
    {
        *reinterpret_cast<void**>(block_start) = get_ptr_from_block(next_free);
        *reinterpret_cast<size_t*>(reinterpret_cast<std::byte*>(block_start) + sizeof(void*)) += get_block_size(next_free) + block_metadata_size;
    } else
    {
        *reinterpret_cast<void**>(block_start) = next_free;
    }

    *reinterpret_cast<void**>(block_start) = _trusted_memory;

    information_with_guard(std::to_string(get_free_size()));
    // debug_with_guard(print_blocks());
}

inline void allocator_sorted_list::set_fit_mode(allocator_with_fit_mode::fit_mode mode)
{
    get_fit_mod() = mode;
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
    std::lock_guard lock(get_mutex());

    return get_blocks_info_inner();
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info_inner() const
{
    std::vector<allocator_test_utils::block_info> result;

    void* block = *get_first_block_ptr(_trusted_memory);

    while (block != nullptr)
    {
        auto size = get_block_size(block);

        auto start_ptr = reinterpret_cast<std::byte*>(block) + block_metadata_size;

        auto end_ptr = reinterpret_cast<std::byte*>(block) + block_metadata_size + size;

        // result.push_back(allocator_test_utils::block_info{
        //     .is_free = true,
        //     .start_ptr = start_ptr,
        //     .end_ptr = end_ptr
        // });

        block = get_ptr_from_block(block);
    }

    return result;
}

inline allocator *allocator_sorted_list::get_allocator() const
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<std::byte*>(_trusted_memory) + sizeof(logger*));
}

inline logger *allocator_sorted_list::get_logger() const
{
    return *reinterpret_cast<logger**>(_trusted_memory);
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
    return "allocator_sorted_list";
}

inline std::mutex &allocator_sorted_list::get_mutex() const noexcept
{
    return *reinterpret_cast<std::mutex*>(reinterpret_cast<std::byte*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t));
}

inline allocator_with_fit_mode::fit_mode &allocator_sorted_list::get_fit_mod() const noexcept
{
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(reinterpret_cast<std::byte*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator*));
}

inline size_t allocator_sorted_list::get_overall_size(void *trusted_memory) noexcept
{
    return *reinterpret_cast<size_t*>(reinterpret_cast<std::byte*>(trusted_memory) + sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode));
}

inline void **allocator_sorted_list::get_first_block_ptr(void *trusted_memory) noexcept
{
    return reinterpret_cast<void**>(reinterpret_cast<std::byte*>(trusted_memory) + sizeof(logger*) + sizeof(allocator*) + sizeof(allocator_with_fit_mode::fit_mode) + sizeof(size_t) + sizeof(std::mutex));
}

void *allocator_sorted_list::get_first(size_t size) const noexcept
{
    void* prev_free = _trusted_memory;
    void* free_block = *get_first_block_ptr(_trusted_memory);

    while (free_block != nullptr)
    {
        if (get_block_size(free_block) >= size)
        {
            return prev_free;
        }

        prev_free = free_block;
        free_block = get_ptr_from_block(free_block);
    }

    return nullptr;
}

void *allocator_sorted_list::get_best(size_t size) const noexcept
{
    void* prev_free = _trusted_memory;
    void* free_block = *get_first_block_ptr(_trusted_memory);

    void* best_prev_free = nullptr;
    size_t best_fit = SIZE_MAX;

    while (free_block != nullptr)
    {
        size_t free_size = get_block_size(free_block);

        if (free_size >= size && free_size < best_fit)
        {
            best_prev_free = prev_free;
            best_fit = free_size;
        }

        prev_free = free_block;
        free_block = get_ptr_from_block(free_block);
    }

    return best_prev_free;
}

void *allocator_sorted_list::get_worst(size_t size) const noexcept
{
    void* prev_free = _trusted_memory;
    void* free_block = *get_first_block_ptr(_trusted_memory);

    void* worst_prev_free = nullptr;
    size_t worst_fit = 0;

    while (free_block != nullptr)
    {
        size_t free_size = get_block_size(free_block);

        if (free_size >= size && free_size > worst_fit)
        {
            worst_prev_free = prev_free;
            worst_fit = free_size;
        }

        prev_free = free_block;
        free_block = get_ptr_from_block(free_block);
    }

    return worst_prev_free;
}

void *allocator_sorted_list::get_ptr_from_block(void *block_start) noexcept
{
    return *reinterpret_cast<void**>(block_start);
}

size_t allocator_sorted_list::get_block_size(void *block_start) noexcept
{
    return *reinterpret_cast<size_t*>(reinterpret_cast<std::byte*>(block_start) + sizeof(void*));
}

size_t allocator_sorted_list::get_free_size() const noexcept
{
    std::vector<void*> free_blocks = get_free_blocks();

    size_t result = 0;

    for (auto block : free_blocks)
    {
        result += get_block_size(block);
    }

    return result;
}

std::vector<void*> allocator_sorted_list::get_free_blocks() const noexcept
{
    std::vector<void*> free_blocks;

    void* free_block = *get_first_block_ptr(_trusted_memory);

    while (free_block != nullptr)
    {
        free_blocks.push_back(free_block);
        free_block = get_ptr_from_block(free_block);
    }

    return free_blocks;
}

std::vector<void*> allocator_sorted_list::get_all_blocks() const noexcept
{
    std::vector<void*> blocks;

    void* block = *get_first_block_ptr(_trusted_memory);

    while (block != nullptr)
    {
        blocks.push_back(block);
        block = reinterpret_cast<std::byte*>(block) + block_metadata_size + get_block_size(block);
    }

    return blocks;
}

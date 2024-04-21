#include <not_implemented.h>

#include "../include/allocator_buddies_system.h"

// * RULE OF FIVE BEGIN * //
allocator_buddies_system::~allocator_buddies_system()
{
    throw not_implemented("allocator_buddies_system::~allocator_buddies_system()", "your code should be here...");
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system const &other)
{
    throw not_implemented("allocator_buddies_system::allocator_buddies_system(allocator_buddies_system const &)", "your code should be here...");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system const &other)
{
    // TODO: implement
}

allocator_buddies_system::allocator_buddies_system(
    allocator_buddies_system &&other) noexcept
{
    throw not_implemented("allocator_buddies_system::allocator_buddies_system(allocator_buddies_system &&) noexcept", "your code should be here...");
}

allocator_buddies_system &allocator_buddies_system::operator=(
    allocator_buddies_system &&other) noexcept
{
    if (this != &other) {
        std::swap(_trusted_memory, other._trusted_memory);
    }
    return *this;
}

// * RULE OF FIVE END * //


allocator_buddies_system::allocator_buddies_system( // * CONSTRUCTOR * //
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    allocator_with_fit_mode::set_fit_mode(allocate_fit_mode);

    logger->log("Allocator has been initialized\n", logger::severity::information);

    if (parent_allocator != nullptr) {

    }

    throw not_implemented("allocator_buddies_system::allocator_buddies_system(size_t, allocator *, logger *, allocator_with_fit_mode::fit_mode)", "your code should be here...");
}

[[nodiscard]] void *allocator_buddies_system::allocate( // * ALLOCATE * //
    size_t value_size,
    size_t values_count)
{
    throw not_implemented("[[nodiscard]] void *allocator_buddies_system::allocate(size_t, size_t)", "your code should be here...");
}

void allocator_buddies_system::deallocate( // * DEALLOCATE * //
    void *at)
{    
    throw not_implemented("void allocator_buddies_system::deallocate(void *)", "your code should be here...");
}

// * GETTERS AND SETTERS BEGIN * //

inline void allocator_buddies_system::set_fit_mode(
    allocator_with_fit_mode::fit_mode mode)
{
    allocator_with_fit_mode::set_fit_mode(mode);
}

inline allocator *allocator_buddies_system::get_allocator() const
{
    return allocator_guardant::get_allocator();
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept", "your code should be here...");
}

inline logger *allocator_buddies_system::get_logger() const
{
    return logger_guardant::get_logger();
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    return typename_holder::get_typename();
}

// * GETTERS AND SETTERS END * //

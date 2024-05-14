#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <vector>
#include <mutex>

class allocator_sorted_list final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{
private:
    void *_trusted_memory;

    static constexpr const size_t allocator_metadata_size = sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode) + sizeof(size_t) + sizeof(std::mutex) + sizeof(void*);
    static constexpr const size_t block_metadata_size = sizeof(void*) + sizeof(size_t);

public:
    explicit allocator_sorted_list(
            size_t space_size,
            allocator *parent_allocator = nullptr,
            logger *logger = nullptr,
            allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

    allocator_sorted_list(allocator_sorted_list const &other) = delete;
    allocator_sorted_list &operator=(allocator_sorted_list const &other) = delete;

    allocator_sorted_list(allocator_sorted_list &&other) noexcept;
    allocator_sorted_list &operator=(allocator_sorted_list &&other) noexcept;

    ~allocator_sorted_list() override;

    [[nodiscard]] void *allocate(size_t value_size, size_t values_count) override;
    void deallocate(void *at) override;

    inline void set_fit_mode(allocator_with_fit_mode::fit_mode mode) override;

    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:
    std::vector<allocator_test_utils::block_info> get_blocks_info_inner() const;

    inline allocator *get_allocator() const override;
    inline logger *get_logger() const override;
    inline std::string get_typename() const noexcept;
    inline std::mutex& get_mutex() const noexcept;
    inline allocator_with_fit_mode::fit_mode& get_fit_mod() const noexcept;

    static inline size_t get_overall_size(void* trusted_memory) noexcept;
    static inline void** get_first_block_ptr(void* trusted_memory) noexcept;

    void* get_first(size_t size) const noexcept;
    void* get_best(size_t size) const noexcept;
    void* get_worst(size_t size) const noexcept;

    static void* get_ptr_from_block(void* block_start) noexcept;
    static size_t get_block_size(void* block_start) noexcept;
    size_t get_free_size() const noexcept;

    std::vector<void*> get_free_blocks() const noexcept;
    std::vector<void*> get_all_blocks() const noexcept;
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H

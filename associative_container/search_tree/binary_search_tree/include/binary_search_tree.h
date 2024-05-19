#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <optional>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>
#include <memory>

template<
		typename tkey,
		typename tvalue>
class binary_search_tree:
		public search_tree<tkey, tvalue>
{

protected:

	struct node
	{

	public:

		tkey key;

		tvalue value;

		node *left_subtree;

		node *right_subtree;

	public:

		explicit node(
				tkey const &key,
				tvalue const &value);

		explicit node(
				tkey const &key,
				tvalue &&value);

	public:

		virtual ~node() noexcept = default;

	};

public:

	// * region iterators definition

	class prefix_iterator;
	class prefix_const_iterator;
	class prefix_reverse_iterator;
	class prefix_reverse_const_iterator;
	class infix_iterator;
	class infix_const_iterator;
	class infix_reverse_iterator;
	class infix_reverse_const_iterator;
	class postfix_iterator;
	class postfix_const_iterator;
	class postfix_reverse_iterator;
	class postfix_reverse_const_iterator;

	struct iterator_data
	{

	public:

		unsigned int depth;

		tkey key;

		tvalue value;

	public:

		explicit iterator_data(
				unsigned int depth = 0 ,
				tkey const &key = tkey() ,
				tvalue const &value = tvalue());

		explicit iterator_data(
				unsigned int depth,
				node **src_node);

		iterator_data(
				iterator_data const &other);

		iterator_data(
				iterator_data &&other) noexcept;

		iterator_data &operator=(
				iterator_data const &other);

		iterator_data &operator=(
				iterator_data &&other) noexcept;


	public:

		virtual void update_all_data(std::stack <node**>& path) {
			if (!path.empty()) {
                key = (*(path.top()))->key;
				value = (*(path.top()))->value;
				depth = path.size() - 1;
			}
		}

		virtual iterator_data* clone_iterator_data() const
		{
			return new iterator_data(*this);
		}

		virtual ~iterator_data() = default;
	};

	class prefix_iterator final // * prefix_iterator
	{
		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;

		iterator_data* clone_data() const
		{
            if (!_ptr_info.get()) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

	private:

		explicit prefix_iterator(
				const std::stack <node**>& path, iterator_data* it_data = nullptr);

		prefix_iterator(const prefix_iterator& other);

		prefix_iterator(prefix_iterator&& other) noexcept = default;

		prefix_iterator& operator=(const prefix_iterator& other);

		prefix_iterator& operator=(prefix_iterator&& other) noexcept = default;

		virtual ~prefix_iterator() = default;

	public:

		bool operator==(
				prefix_iterator const &other) const noexcept;

		bool operator!=(
				prefix_iterator const &other) const noexcept;

		prefix_iterator &operator++();

		prefix_iterator const operator++(
				int not_used);

		iterator_data *operator*() const;

	};

	class prefix_const_iterator final // * prefix_const_iterator
	{
		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;


		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}


	public:

		explicit prefix_const_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		prefix_const_iterator(const prefix_const_iterator& other);

		prefix_const_iterator(prefix_const_iterator&& other) noexcept = default;

		prefix_const_iterator& operator=(const prefix_const_iterator& other);

		prefix_const_iterator& operator=(prefix_const_iterator&& other) noexcept = default;

		virtual ~prefix_const_iterator() = default;

	public:

		bool operator==(
				prefix_const_iterator const &other) const noexcept;

		bool operator!=(
				prefix_const_iterator const &other) const noexcept;

		prefix_const_iterator &operator++();

		prefix_const_iterator const operator++(
				int not_used);

		iterator_data const *operator*() const;

	};

	class prefix_reverse_iterator final // * prefix_reverse_iterator
	{
	protected:
		std::unique_ptr <iterator_data> _ptr_info; // * store iterator data
		std::stack <node**> _traverse_path; // * store path from root to current node

		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

		friend class binary_search_tree<tkey, tvalue>;

	private:

		explicit prefix_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		prefix_reverse_iterator(const prefix_reverse_iterator& other);

		prefix_reverse_iterator(prefix_reverse_iterator&& other) noexcept = default;

		prefix_reverse_iterator& operator=(const prefix_reverse_iterator& other);

		prefix_reverse_iterator& operator=(prefix_reverse_iterator&& other) noexcept = default;

		virtual ~prefix_reverse_iterator() = default;

	public:

		bool operator==(
				prefix_reverse_iterator const &other) const noexcept;

		bool operator!=(
				prefix_reverse_iterator const &other) const noexcept;

		prefix_reverse_iterator &operator++();

		prefix_reverse_iterator const operator++(
				int not_used);

		iterator_data *operator*() const;

	};

	class prefix_const_reverse_iterator final // * prefix_const_reverse_iterator
	{

		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;

		iterator_data* clone_data() const {
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }
            return _ptr_info->clone_iterator_data();
		}

	private:

		explicit prefix_const_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		prefix_const_reverse_iterator(const prefix_const_reverse_iterator& other);

		prefix_const_reverse_iterator(prefix_const_reverse_iterator&& other) noexcept = default;

		prefix_const_reverse_iterator& operator=(const prefix_const_reverse_iterator& other);

		prefix_const_reverse_iterator& operator=(prefix_const_reverse_iterator&& other) noexcept = default;

		virtual ~prefix_const_reverse_iterator() = default;

	public:

		bool operator==(
				prefix_const_reverse_iterator const &other) const noexcept;

		bool operator!=(
				prefix_const_reverse_iterator const &other) const noexcept;

		prefix_const_reverse_iterator &operator++();

		prefix_const_reverse_iterator const operator++(
				int not_used);

		iterator_data const *operator*() const;

	};

	class infix_iterator final // * infix_iterator
	{

		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;

		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

	public:

		explicit infix_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		infix_iterator(const infix_iterator& other);

		infix_iterator(infix_iterator&& other) noexcept = default;

		infix_iterator& operator=(const infix_iterator& other);

		infix_iterator& operator=(infix_iterator&& other) noexcept = default;

		virtual ~infix_iterator() = default;

		node** get_node() {
			return _traverse_path.top();
		}

	public:

		bool operator==(
				infix_iterator const &other) const noexcept;

		bool operator!=(
				infix_iterator const &other) const noexcept;

		infix_iterator &operator++();

		infix_iterator const operator++(
				int not_used);

		iterator_data *operator*() const;

	private:


	};

	class infix_const_iterator final // * infix_const_iterator
	{

		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;

		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

            return _ptr_info->clone_iterator_data();
		}

		friend class binary_search_tree<tkey, tvalue>;

	public:

		explicit infix_const_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		infix_const_iterator(const infix_const_iterator& other);

		infix_const_iterator(infix_const_iterator&& other) noexcept = default;

		infix_const_iterator& operator=(const infix_const_iterator& other);

		infix_const_iterator& operator=(infix_const_iterator&& other) noexcept = default;

		virtual ~infix_const_iterator() = default;

	public:

		bool operator==(
				infix_const_iterator const &other) const noexcept;

		bool operator!=(
				infix_const_iterator const &other) const noexcept;

		infix_const_iterator &operator++();

		infix_const_iterator const operator++(
				int not_used);

		iterator_data const *operator*() const;

	};

	class infix_reverse_iterator final // * infix_reverse_iterator
	{
		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;

		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

		friend class binary_search_tree<tkey, tvalue>;

	public:

		explicit infix_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		infix_reverse_iterator(const infix_reverse_iterator& other);

		infix_reverse_iterator(infix_reverse_iterator&& other) noexcept = default;

		infix_reverse_iterator& operator=(const infix_reverse_iterator& other);

		infix_reverse_iterator& operator=(infix_reverse_iterator&& other) noexcept = default;

		virtual ~infix_reverse_iterator() = default;

	public:

		bool operator==(
				infix_reverse_iterator const &other) const noexcept;

		bool operator!=(
				infix_reverse_iterator const &other) const noexcept;

		infix_reverse_iterator &operator++();

		infix_reverse_iterator const operator++(
				int not_used);

		iterator_data *operator*() const;

	};

	class infix_const_reverse_iterator final // * infix_const_reverse_iterator
	{

		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;


		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

		friend class binary_search_tree<tkey, tvalue>;

	public:

		explicit infix_const_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		infix_const_reverse_iterator(const infix_const_reverse_iterator& other);

		infix_const_reverse_iterator(infix_const_reverse_iterator&& other) noexcept = default;

		infix_const_reverse_iterator& operator=(const infix_const_reverse_iterator& other);

		infix_const_reverse_iterator& operator=(infix_const_reverse_iterator&& other) noexcept = default;

		virtual ~infix_const_reverse_iterator() = default;

	public:

		bool operator==(
				infix_const_reverse_iterator const &other) const noexcept;

		bool operator!=(
				infix_const_reverse_iterator const &other) const noexcept;

		infix_const_reverse_iterator &operator++();

		infix_const_reverse_iterator const operator++(
				int not_used);

		iterator_data const *operator*() const;

	};

	class postfix_iterator final // * postfix_iterator
	{

		std::unique_ptr <iterator_data> _ptr_info;

		std::stack <node**> _traverse_path;


		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

	public:

		explicit postfix_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		postfix_iterator(const postfix_iterator& other);

		postfix_iterator(postfix_iterator&& other) noexcept = default;

		postfix_iterator& operator=(const postfix_iterator& other);

		postfix_iterator& operator=(postfix_iterator&& other) noexcept = default;

		virtual ~postfix_iterator() = default;

	public:

		bool operator==(
				postfix_iterator const &other) const noexcept;

		bool operator!=(
				postfix_iterator const &other) const noexcept;

		postfix_iterator &operator++();

		postfix_iterator const operator++(
				int not_used);

		iterator_data *operator*() const;

	};

	class postfix_const_iterator final // * postfix_const_iterator
	{
		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;


		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

	public:

		explicit postfix_const_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		postfix_const_iterator(const postfix_const_iterator& other);

		postfix_const_iterator(postfix_const_iterator&& other) noexcept = default;

		postfix_const_iterator& operator=(const postfix_const_iterator& other);

		postfix_const_iterator& operator=(postfix_const_iterator&& other) noexcept = default;

		virtual ~postfix_const_iterator() = default;

	public:

		bool operator==(
				postfix_const_iterator const &other) const noexcept;

		bool operator!=(
				postfix_const_iterator const &other) const noexcept;

		postfix_const_iterator &operator++();

		postfix_const_iterator const operator++(
				int not_used);

		iterator_data const *operator*() const;

	};

	class postfix_reverse_iterator final // * postfix_reverse_iterator
	{
		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;


		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
		}

	public:

		explicit postfix_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		postfix_reverse_iterator(const postfix_reverse_iterator& other);

		postfix_reverse_iterator(postfix_reverse_iterator&& other) noexcept = default;

		postfix_reverse_iterator& operator=(const postfix_reverse_iterator& other);

		postfix_reverse_iterator& operator=(postfix_reverse_iterator&& other) noexcept = default;

		virtual ~postfix_reverse_iterator() = default;

		friend class binary_search_tree<tkey, tvalue>;


	public:

		bool operator==(
				postfix_reverse_iterator const &other) const noexcept;

		bool operator!=(
				postfix_reverse_iterator const &other) const noexcept;

		postfix_reverse_iterator &operator++();

		postfix_reverse_iterator const operator++(
				int not_used);

		iterator_data *operator*() const;

	};

	class postfix_const_reverse_iterator final // * postfix_const_reverse_iterator
	{
        friend class binary_search_tree<tkey, tvalue>;

		std::unique_ptr <iterator_data> _ptr_info;
		std::stack <node**> _traverse_path;

		iterator_data* clone_data() const
		{
            if (_ptr_info.get() == nullptr) {
                return nullptr;
            }

			return _ptr_info->clone_iterator_data();
        }

	public:

		explicit postfix_const_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data = nullptr);

		postfix_const_reverse_iterator(const postfix_const_reverse_iterator& other);

		postfix_const_reverse_iterator(postfix_const_reverse_iterator&& other) noexcept = default;

		postfix_const_reverse_iterator& operator=(const postfix_const_reverse_iterator& other);

		postfix_const_reverse_iterator& operator=(postfix_const_reverse_iterator&& other) noexcept = default;

		virtual ~postfix_const_reverse_iterator() = default;

	public:

		bool operator==(
				postfix_const_reverse_iterator const &other) const noexcept;

		bool operator!=(
				postfix_const_reverse_iterator const &other) const noexcept;

		postfix_const_reverse_iterator &operator++();

		postfix_const_reverse_iterator const operator++(
				int not_used);

		iterator_data const *operator*() const;

	};

	// * endregion iterators definition

public:

	// * region target operations strategies definition

	enum class insertion_of_existent_key_attempt_strategy
	{
		update_value,
		throw_an_exception
	};

	enum class disposal_of_nonexistent_key_attempt_strategy
	{
		do_nothing,
		throw_an_exception
	};

	// * endregion target operations strategies definition

	// * region target operations associated exception types

	class insertion_of_existent_key_attempt_exception final: // * class insertion_of_existent_key_attempt_exception
			public std::logic_error
	{

	private:

		tkey _key;

	public:

		explicit insertion_of_existent_key_attempt_exception(
				tkey const &key);

	public:

		tkey const &get_key() const noexcept;

	};

	class obtaining_of_nonexistent_key_attempt_exception final:
			public std::logic_error
	{

	private:

		tkey _key;

	public:

		explicit obtaining_of_nonexistent_key_attempt_exception(
				tkey const &key);

	public:

		tkey const &get_key() const noexcept;

	};

	class disposal_of_nonexistent_key_attempt_exception final:
			public std::logic_error
	{

	private:

		tkey _key;

	public:

		explicit disposal_of_nonexistent_key_attempt_exception(
				tkey const &key);

	public:

		tkey const &get_key() const noexcept;

	};

	// * endregion target operations associated exception types

	// * region template methods definition

protected:

	class template_method_basics:
			public logger_guardant
	{

	protected:

		binary_search_tree<tkey, tvalue> *_tree;

	public:

		explicit template_method_basics(
				binary_search_tree<tkey, tvalue> *tree);

	protected:

		std::stack<node **> find_path(
				tkey const &key) const
		{
			std::stack <node **> result_path;

			node **path_finder = &(_tree->_root);
			auto const &comparer = _tree->_keys_comparer;

			while (true) {
				result_path.push(path_finder);

				if (*path_finder == nullptr) {
					break;
				}

				auto comparison_res = comparer(key, (*path_finder)->key);
				if (comparison_res == 0) {
					break;
				}

				path_finder = comparison_res < 0
							  ? &((*path_finder)->left_subtree)
							  : &((*path_finder)->right_subtree);
			}

			return result_path;
		}

		virtual void balance(
				std::stack<node **> &path) {

		}

	protected:

		[[nodiscard]] logger *get_logger() const noexcept final;

	};

	class insertion_template_method:
			public template_method_basics,
			public allocator_guardant
	{

	private:

		binary_search_tree<tkey, tvalue> *_tree;

		binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strategy;

	public:

		explicit insertion_template_method(
				binary_search_tree<tkey, tvalue> *tree,
				typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

	public:

		void insert(
				tkey const &key,
				tvalue const &value);

		void insert(
				tkey const &key,
				tvalue &&value);

		void set_insertion_strategy(
				typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

	private:

		[[nodiscard]] allocator *get_allocator() const noexcept final;

	};

	class obtaining_template_method:
			public template_method_basics
	{

	public:

		explicit obtaining_template_method(
				binary_search_tree<tkey, tvalue> *tree);

	public:

		tvalue const &obtain(
				tkey const &key);

		std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
				tkey const &lower_bound,
				tkey const &upper_bound,
				bool lower_bound_inclusive,
				bool upper_bound_inclusive) {
			std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;

			std::stack<node *> path;
			node *current = this->_tree->_root;

			while (true) {
				if (current == nullptr) {
					break;
				}

				auto comparison_res = this->_tree->_keys_comparer(lower_bound, current->key);

				path.push(current);

				if (comparison_res == 0) {

					if (lower_bound_inclusive) {
						break;
					}

					current = current->right_subtree;

				} else if (comparison_res < 0) {
					current = current->left_subtree;
				} else {
					current = current->right_subtree;
				}

				if (current == nullptr and this->_tree->_keys_comparer(path.top()->key, lower_bound) < (lower_bound_inclusive ? 0 : 1)) {
					path = std::move(std::stack<node *>());
				}
			}

			auto it = infix_iterator(this->_tree, path, this->_tree->create_iterator_data());
			while ((it != this->_tree->end_infix()) and (this->_tree->_keys_comparer(upper_bound, (*it)->get_key()) > (upper_bound_inclusive
																													  ? -1
																													  : 0))) {
				range.push_back(std::move(typename associative_container<tkey, tvalue>::key_value_pair((*it)->get_key(), (*it)->get_value())));
				++it;
			}

			return range;
		}

	};

	class disposal_template_method:
			public template_method_basics,
			public allocator_guardant
	{

	private:

		binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy _disposal_strategy;

		binary_search_tree<tkey, tvalue> *_tree;

	public:

		explicit disposal_template_method(
				binary_search_tree<tkey, tvalue> *tree,
				typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

	public:

		virtual tvalue dispose(
				tkey const &key);

		void set_disposal_strategy(
				typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

	private:

		template<
				typename T>
		inline void swap(T &&one, T &&another) {
			T temp = std::move(one);
			one = std::move(another);
			another = std::move(temp);
		}

	private:

		[[nodiscard]] allocator *get_allocator() const noexcept final;

	};

	// * endregion template methods definition

protected:

	mutable node *_root;

	insertion_of_existent_key_attempt_strategy add_existing_strat;

	disposal_of_nonexistent_key_attempt_strategy delete_missing_strat;

protected:

	explicit binary_search_tree(
			typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
			typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
			typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
			std::function<int(tkey const &, tkey const &)>,
			allocator *allocator,
			logger *logger);

public:

	explicit binary_search_tree(
			std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
			allocator *allocator = nullptr,
			logger *logger = nullptr,
			typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
			typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

	binary_search_tree(
			binary_search_tree<tkey, tvalue> const &other);

	binary_search_tree(
			binary_search_tree<tkey, tvalue> &&other) noexcept;

	binary_search_tree<tkey, tvalue> &operator=(
			binary_search_tree<tkey, tvalue> const &other);

	binary_search_tree<tkey, tvalue> &operator=(
			binary_search_tree<tkey, tvalue> &&other) noexcept;

	~binary_search_tree() override;

private:

	void clear(node *&subtree_root) {

		if (subtree_root == nullptr) {
			return;
		}

		clear(subtree_root->left_subtree);
		clear(subtree_root->right_subtree);

		subtree_root->~node();

		this->deallocate_with_guard(subtree_root);

		subtree_root = nullptr;
	}

	node *copy(
			node const *subtree_root) {
		if (subtree_root == nullptr) {
			return nullptr;
		}

		node *subtree_root_copied = reinterpret_cast<node *>(this->allocate_with_guard(get_node_size(), 1));
		call_node_constructor(subtree_root_copied, subtree_root->key, subtree_root->value);
		subtree_root_copied->left_subtree = copy(subtree_root->left_subtree);
		subtree_root_copied->right_subtree = copy(subtree_root->right_subtree);

		return subtree_root_copied;
	}

	virtual size_t get_node_size() const noexcept
	{
		return sizeof(typename binary_search_tree<tkey, tvalue>::node);
	}

	virtual void call_node_constructor(
			node *raw_space,
			tkey const &key,
			tvalue const &value) {
		allocator::construct(raw_space, key, value);
	}

	virtual void call_node_constructor(
			node *raw_space,
			tkey const &key,
			tvalue &&value) {
		allocator::construct(raw_space, key, std::move(value));
	}

	virtual inline void update_node_data(node *node) const noexcept
	{
	}

	virtual void inject_additional_data(
			node *destination,
			node const *source) const
	{

	}

	virtual void inject_additional_data(
			iterator_data *destination,
			node *source) {

	}

	virtual iterator_data *create_iterator_data() const
	{
		return new iterator_data;
	}

	// virtual iterator_data *create_iterator_data(unsigned int depth, node *&src_node) const
	// {
	// 	return new iterator_data(depth, &src_node);
	// }

public:

	void insert(
			tkey const &key,
			tvalue const &value) final;

	void insert(
			tkey const &key,
			tvalue &&value) final;

	tvalue const &obtain(
			tkey const &key) final;

	std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
			tkey const &lower_bound,
			tkey const &upper_bound,
			bool lower_bound_inclusive,
			bool upper_bound_inclusive) final;

	tvalue dispose(
			tkey const &key) final;

public:

	void set_insertion_strategy(
			typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

	void set_removal_strategy(
			typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:

	// * region iterators requests definition

	prefix_iterator begin_prefix() const noexcept;

	prefix_iterator end_prefix() const noexcept;

	prefix_const_iterator cbegin_prefix() const noexcept;

	prefix_const_iterator cend_prefix() const noexcept;

	prefix_reverse_iterator rbegin_prefix() const noexcept;

	prefix_reverse_iterator rend_prefix() const noexcept;

	prefix_const_reverse_iterator crbegin_prefix() const noexcept;

	prefix_const_reverse_iterator crend_prefix() const noexcept;

	infix_iterator begin_infix() const noexcept;

	infix_iterator end_infix() const noexcept;

	infix_const_iterator cbegin_infix() const noexcept;

	infix_const_iterator cend_infix() const noexcept;

	infix_reverse_iterator rbegin_infix() const noexcept;

	infix_reverse_iterator rend_infix() const noexcept;

	infix_const_reverse_iterator crbegin_infix() const noexcept;

	infix_const_reverse_iterator crend_infix() const noexcept;

	postfix_iterator begin_postfix() const noexcept;

	postfix_iterator end_postfix() const noexcept;

	postfix_const_iterator cbegin_postfix() const noexcept;

	postfix_const_iterator cend_postfix() const noexcept;

	postfix_reverse_iterator rbegin_postfix() const noexcept;

	postfix_reverse_iterator rend_postfix() const noexcept;

	postfix_const_reverse_iterator crbegin_postfix() const noexcept;

	postfix_const_reverse_iterator crend_postfix() const noexcept;

	// * endregion iterators requests definition

protected:

	std::stack <node**> find_path_to_node(const tkey& key);

	virtual void insert_inside(const tkey& key, const tvalue& value, std::stack <node**>& stk);

	virtual void insert_inside(const tkey& key, tvalue&& value, std::stack <node**>& stk);

	virtual tvalue const& obtain_inside(std::stack <node**>& stk);

	virtual tvalue dispose_inside(std::stack <node**>& stk);

	virtual void copy_subtree(node** dest, node* src);

	virtual void destroy_subtree(node* src) noexcept;

	// * region subtree rotations definition

	void small_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void small_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void big_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void big_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void double_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool at_grandparent_first,
            bool validate = true) const;

    void double_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool at_grandparent_first,
            bool validate = true) const;

	// * endregion subtree rotations definition

};

// * region binary_search_tree<tkey, tvalue>::node methods implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
		tkey const &key,
		tvalue const &value):
		key(key),
		value(value),
		left_subtree(nullptr),
		right_subtree(nullptr)
{

}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
		tkey const &key,
		tvalue &&value):
		key(key),
		value(std::move(value)),
		left_subtree(nullptr),
		right_subtree(nullptr)
{
    // 0-0
}

// * endregion binary_search_tree<tkey, tvalue>::node methods implementation

// * region iterators implementation

// * region iterator data implementation

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(unsigned int depth, tkey const &key, tvalue const &value):
		depth(depth), key(key), value(value) {}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
		unsigned int depth,
		node **src_node):
		depth(depth)
{
	if (src_node and *src_node) {
		this->key = ((*src_node)->key);
		this->value = ((*src_node)->value);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
		iterator_data const &other):
		depth(other.depth),
		key(other.key),
		value(other.value)
{

}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
		iterator_data &&other) noexcept:
		depth(std::move(other.depth)),
		key(std::move(other.key)),
		value(std::move(other.value))
{
	other.key = nullptr;
	other.value = nullptr;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data &binary_search_tree<tkey, tvalue>::iterator_data::operator=(
		iterator_data const &other)
{
	if (this != &other) {
		depth = other.depth;

		if (other._is_related_to_tree) {
			key = other.key;
			value = other.value;

			return *this;
		}

		try {
			key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey)));
			value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue)));
		} catch (const std::bad_alloc &) {
			::operator delete(key);
			::operator delete(value);
			throw;
		}

		allocator::construct(key, *other.key);
		allocator::construct(value, *other.value);
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data &binary_search_tree<tkey, tvalue>::iterator_data::operator=(
		iterator_data &&other) noexcept
{
	if (this != &other) {
		depth = std::move(other.depth);
		key = std::move(other.key);
		value = std::move(other.value);

		other.key = nullptr;
		other.value = nullptr;
	}

	return *this;
}


// * endregion iterator data implementation

// * region prefix_iterator implementation

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(const prefix_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator& binary_search_tree<tkey, tvalue>::prefix_iterator::operator=(const binary_search_tree::prefix_iterator &other)
{
	if (*this != other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==( // !!!
		typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and (other._traverse_path).empty()) or (!_traverse_path.empty() and !(other._traverse_path).empty() and _traverse_path.top() == (other._traverse_path).top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
	if (_traverse_path.empty()) {
		return *this;
	}

	if ((*_traverse_path.top())->left_subtree) {

		_traverse_path.push(&((*_traverse_path.top())->left_subtree));

	} else if ((*_traverse_path.top())->right_subtree) {

		_traverse_path.push(&((*_traverse_path.top())->right_subtree));

	} else {
		node* child_node = *(_traverse_path.top());

		_traverse_path.pop();

		while (!_traverse_path.empty()) {
			if ((*_traverse_path.top())->left_subtree == child_node and (*_traverse_path.top())->right_subtree == nullptr) {
				child_node = *(_traverse_path.top());
				_traverse_path.pop();
				continue;
			}

			if ((*_traverse_path.top())->left_subtree == child_node and (*_traverse_path.top())->right_subtree) {
				_traverse_path.push(&((*_traverse_path.top())->right_subtree));
				break;
			}

			if ((*_traverse_path.top())->right_subtree == child_node) {
				child_node = *(_traverse_path.top());
				_traverse_path.pop();
				continue;
			}
		}
	}

	_ptr_info->update_all_data(_traverse_path);

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion prefix_iterator implementation * //

// * region prefix_const_iterator implementation * //

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(const prefix_const_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator& binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator=(const prefix_const_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
	return (_traverse_path.empty() and other._traverse_path.empty() or (!_traverse_path.empty() and !other._traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
	if (_traverse_path.empty()) {
		return *this;
	}

	if ((*_traverse_path.top())->left_subtree) {

		_traverse_path.push(&((*_traverse_path.top())->left_subtree));

	} else if ((*_traverse_path.top())->right_subtree) {

		_traverse_path.push(&((*_traverse_path.top())->right_subtree));

	} else {
		node* child_node = *(_traverse_path.top());

		_traverse_path.pop();

		//  * move to the leftmost node from the parent of child_node
		while (!_traverse_path.empty()) {
			if ((*_traverse_path.top())->left_subtree == child_node and !((*_traverse_path.top())->right_subtree)) {
				//  * leftmost node is the current node
			    child_node = *(_traverse_path.top());
				_traverse_path.pop();
				continue;
			}

			if ((*_traverse_path.top())->left_subtree == child_node and ((*_traverse_path.top())->right_subtree)) {
				//  * go to the right subtree
				_traverse_path.push(&((*_traverse_path.top())->right_subtree));
				break;
			}

			if ((*_traverse_path.top())->right_subtree == child_node) {
				//  * rightmosr node is the current node
				child_node = *(_traverse_path.top());
				_traverse_path.pop();
				continue;
			}
		}
	}

	_ptr_info->update_all_data(_traverse_path);

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion prefix_const_iterator implementation

// * region prefix_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data ) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(const prefix_reverse_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator& binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator=(const prefix_reverse_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}
template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
	return ((other._traverse_path.empty() and _traverse_path.empty()) or (!_traverse_path.empty() and !other._traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++() {
    
    if (_traverse_path.empty()) {
        return *this; 
    }

    node* current_node = *(_traverse_path.top());

    // * do if right subtree exists
    if (current_node->right_subtree) {
        _traverse_path.push(&(current_node->right_subtree));
        _ptr_info->update_all_data(_traverse_path);
        return *this;
    }

    // * do if left subtree exists
    if (current_node->left_subtree) {
        _traverse_path.push(&(current_node->left_subtree));
        _ptr_info->update_all_data(_traverse_path);
        return *this;
    }

    // * go up the tree of no subtrees
    _traverse_path.pop();

    while (!_traverse_path.empty()) {
        node* parent_node = *(_traverse_path.top());

        // * moving up from a (right child) with (no left child) ? continue going up
        if (parent_node->right_subtree == current_node && parent_node->left_subtree == nullptr) {
            current_node = parent_node;
            _traverse_path.pop();
            continue;
        }

        // * moving up from a right child with a left child ? push the left subtree
        if (parent_node->right_subtree == current_node && parent_node->left_subtree) {
            _traverse_path.push(&(parent_node->left_subtree));
            _ptr_info->update_all_data(_traverse_path);
            return *this;
        }

        // * moving up from a left child ? continue moving up
        if (parent_node->left_subtree == current_node) {
            current_node = parent_node;
            _traverse_path.pop();
            continue;
        }
    }

    _ptr_info->update_all_data(_traverse_path);

    return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion prefix_reverse_iterator implementation

// * region prefix_const_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator( const std::stack <node**>& path, iterator_data* it_data) :  _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(const prefix_const_reverse_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator=(const prefix_const_reverse_iterator& other) {
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
	return (_traverse_path.empty() and other._traverse_path.empty() or (!_traverse_path.empty() and !other._traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
		if (_traverse_path.empty()) {
			return *this;
		}

		if ((*_traverse_path.top())->right_subtree) {
			_traverse_path.push(&(*(_traverse_path.top())->right_subtree));
		} else if ((*_traverse_path.top())->left_subtree) {
			_traverse_path.push(&(*(_traverse_path.top())->left_subtree));
		} else {
			// try to find a node with a right subtree
			node* child_node = *(_traverse_path.top());
			_traverse_path.pop();

			while (!_traverse_path.empty()) {
				// * check if current node is the rightmost node with a right subtree
				if ((*_traverse_path.top())->right_subtree == child_node and (*_traverse_path.top())->left_subtree == nullptr) {
					child_node = *(_traverse_path.top());
					_traverse_path.pop();
					continue;
				}

				if ((*_traverse_path.top())->right_subtree == child_node and (*_traverse_path.top())->left_subtree) {
					_traverse_path.push(&((*_traverse_path.top())->left_subtree));
					break;
				}

				// check if  current node is the parent of the node we are looking for
				if ((*_traverse_path.top())->right_subtree == child_node) {
					child_node = *(_traverse_path.top());
					_traverse_path.pop();
					continue;
				}
			}
		}

		_ptr_info->update_all_data(_traverse_path);

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion prefix_const_reverse_iterator implementation

// * region infix_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(const infix_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename  binary_search_tree<tkey, tvalue>::infix_iterator& binary_search_tree<tkey, tvalue>::infix_iterator::operator=(const infix_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}
template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and !other._traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
	if (_traverse_path.empty()) {
		return *this;
	}

	if ((*_traverse_path.top())->right_subtree) {
		node** current_node = &((*_traverse_path.top())->right_subtree);
		while (*current_node) {
			_traverse_path.push(current_node);
			current_node = &((*current_node)->left_subtree);
		}
	} else {
		node* child_node = *(_traverse_path.top());
		_traverse_path.pop();

		if (!_traverse_path.empty() and (*_traverse_path.top())->left_subtree == child_node) {} else
		{
			while (!_traverse_path.empty() and (*_traverse_path.top())->left_subtree != child_node) {
				child_node = *(_traverse_path.top());
				_traverse_path.pop();
			}
		}
	}

	_ptr_info->update_all_data(_traverse_path);

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(
		int not_used)
{
	auto it = *this;
	++*this;
	return it;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion infix_iterator implementation

// * region infix_const_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(const infix_const_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}////////////////////////////////////////////////TODO

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator& binary_search_tree<tkey, tvalue>::infix_const_iterator::operator=(const infix_const_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}


template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
	if (_traverse_path.empty()) {
		return *this;
	}

	if ((*_traverse_path.top())->right_subtree) {
		node** current_node = &((*_traverse_path.top())->right_subtree);
		while (*current_node) {
			_traverse_path.push(current_node);
			current_node = &((*current_node)->left_subtree);
		}
	} else {
		node* child_node = *(_traverse_path.top());
		_traverse_path.pop();

		if (!_traverse_path.empty() and (*_traverse_path.top())->left_subtree == child_node) {} else
		{
			while (!_traverse_path.empty() and (*_traverse_path.top())->left_subtree != child_node) {
				child_node = *(_traverse_path.top());
				_traverse_path.pop();
			}
		}
	}

	_ptr_info->update_all_data(_traverse_path);

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion infix_const_iterator implementation

// * region infix_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(const infix_reverse_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator& binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator=(const infix_reverse_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
	if (_traverse_path.empty()) {
		return *this;
	}

	if ((*_traverse_path.top())->left_subtree) {
		node** current_node = &((*_traverse_path.top())->left_subtree);
		while (*current_node) {
			_traverse_path.push(current_node);
			current_node = &((*current_node)->right_subtree);
		}
	} else {
		node* child_node = *(_traverse_path.top());
		_traverse_path.pop();

		if (!_traverse_path.empty() and (*_traverse_path.top())->right_subtree == child_node) {} else
		{
			while (!_traverse_path.empty() and (*_traverse_path.top())->right_subtree != child_node) {
				child_node = *(_traverse_path.top());
				_traverse_path.pop();
			}
		}
	}

	_ptr_info->update_all_data(_traverse_path);

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion infix_reverse_iterator implementation

// * region infix_const_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(const infix_const_reverse_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator=(const infix_const_reverse_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
	return !(*this = nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
	if (_traverse_path.empty()) {
		return *this;
	}

	if ((*_traverse_path.top())->left_subtree) {
		node** current_node = &((*_traverse_path.top())->left_subtree);
		while (*current_node) {
			_traverse_path.push(current_node);
			current_node = &((*current_node)->right_subtree);
		}
	} else {
		node* child_node = *(_traverse_path.top());
		_traverse_path.pop();

		if (!_traverse_path.empty() and (*_traverse_path.top())->right_subtree == child_node) {} else
		{
			while (!_traverse_path.empty() and (*_traverse_path.top())->right_subtree != child_node) {
				child_node = *(_traverse_path.top());
				_traverse_path.pop();
			}
		}
	}

	_ptr_info->update_all_data(_traverse_path);

	return *this;
}


template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion infix_const_reverse_iterator implementation

// * region postfix_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(const postfix_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator& binary_search_tree<tkey, tvalue>::postfix_iterator::operator=(const postfix_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data> (other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
	node* child_node = *(_traverse_path.top());
	_traverse_path.pop();

	if (!_traverse_path.empty()) {
		if ((*_traverse_path.top())->left_subtree == child_node) {
			if ((*_traverse_path.top())->right_subtree) {

				_traverse_path.push(&((*_traverse_path.top())->right_subtree));

				while ((*_traverse_path.top())->left_subtree) {
					_traverse_path.push(&((*_traverse_path.top())->left_subtree));
				}
			}
		}

		_ptr_info->update_all_data(_traverse_path);
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion postfix_iterator implementation

// * region postfix_const_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(const postfix_const_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator& binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator=(
		const postfix_const_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
	node* child_node = *(_traverse_path.top());
	_traverse_path.pop();

	if (!_traverse_path.empty()) {
		if ((*_traverse_path.top())->left_subtree == child_node) {
			if ((*_traverse_path.top())->right_subtree) {

				_traverse_path.push(&((*_traverse_path.top())->right_subtree));

				while ((*_traverse_path.top())->left_subtree) {
					_traverse_path.push(&((*_traverse_path.top())->left_subtree));
				}
			}
		}

		_ptr_info->update_all_data(_traverse_path);
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion postfix_const_iterator implementation

// * region postfix_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(const std::stack <node**>& path, iterator_data* it_data) : _traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(const postfix_reverse_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator& binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator=(const postfix_reverse_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
	node* child_node = *(_traverse_path.top());
	_traverse_path.pop();

	if (!_traverse_path.empty()) {
		if ((*_traverse_path.top())->right_subtree == child_node) {
			if ((*_traverse_path.top())->left_subtree) {
				_traverse_path.push(&((*_traverse_path.top())->left_subtree));

				while ((*_traverse_path.top())->right_subtree) {
					_traverse_path.push(&((*_traverse_path.top())->right_subtree));
				}
			}
		}

		_ptr_info->update_all_data(_traverse_path);
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion postfix_reverse_iterator implementation

// * region postfix_const_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
		const std::stack <node**>& path, iterator_data* it_data) :
		_traverse_path(path), _ptr_info(it_data)
{
	if (!_traverse_path.empty()) {
		_ptr_info->update_all_data(_traverse_path);
	}
}

template <typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const postfix_const_reverse_iterator& other) : _traverse_path(other._traverse_path), _ptr_info(other.clone_data()) {}

template <typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator=(const postfix_const_reverse_iterator& other)
{
	if (*this == other) {
		_traverse_path = other._traverse_path;
		_ptr_info = std::unique_ptr <iterator_data>(other.clone_data());
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
	return ((_traverse_path.empty() and other._traverse_path.empty()) or (!_traverse_path.empty() and _traverse_path.top() == other._traverse_path.top()));
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
	node* child_node = *(_traverse_path.top());
	_traverse_path.pop();

	if (!_traverse_path.empty()) {
		if ((*_traverse_path.top())->right_subtree == child_node) {
			if ((*_traverse_path.top())->left_subtree) {
				_traverse_path.push(&((*_traverse_path.top())->left_subtree));

				while ((*_traverse_path.top())->right_subtree) {
					_traverse_path.push(&((*_traverse_path.top())->right_subtree));
				}
			}
		}

		_ptr_info->update_all_data(_traverse_path);
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(
		int not_used)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
	return _ptr_info.get();
}

// * endregion postfix_const_reverse_iterator implementation

// * endregion iterators implementation

// * region target operations associated exception types implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
		tkey const &key):
		std::logic_error("Attempt to insert already existing key inside the tree."), _key(key) {}

template<
		typename tkey,
		typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
	return _key;
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
		tkey const &key):
		std::logic_error("Attempt to obtain a value by non-existing key from the tree."), _key(key) {}

template<
		typename tkey,
		typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
	return _key;
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
		tkey const &key):
		std::logic_error("Attempt to dispose a value by non-existing key from the tree."), _key(key) {}

template<
		typename tkey,
		typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
	return _key;
}

// * endregion target operations associated exception types implementation

// * region template methods implementation

// * region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
		binary_search_tree<tkey, tvalue> *tree)
		: _tree(tree)
{
	throw not_implemented("template<typename tkey, typename tvalue>binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(binary_search_tree<tkey, tvalue> *)", "your code should be here...");
}

template<
		typename tkey,
		typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
	throw not_implemented("template<typename tkey, typename tvalue> [[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept", "your code should be here...");
}

// * endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// * region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
		binary_search_tree<tkey, tvalue> *tree,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
		binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree),
		_insertion_strategy(insertion_strategy)
{
	throw not_implemented("template<typename tkey, typename tvalue>binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(binary_search_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_strategy)", "your code should be here...");
}

template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
		tkey const &key,
		tvalue const &value)
{
	auto path = this->find_path(key);

	if (*(path.top())) {
		switch (_insertion_strategy) {
			case insertion_of_existent_key_attempt_strategy::throw_an_exception:
				throw insertion_of_existent_key_attempt_exception(key);
			case insertion_of_existent_key_attempt_strategy::update_value:
				(*(path.top()))->value = value;
				return;
		}
	}

	this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1)), key, value);

	this->balance(path);
}

template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
		tkey const &key,
		tvalue &&value)
{
	auto path = this->find_path(key);

	if (*(path.top())) {
		switch (_insertion_strategy) {
			case insertion_of_existent_key_attempt_strategy::throw_an_exception:
				throw insertion_of_existent_key_attempt_exception(key);
			case insertion_of_existent_key_attempt_strategy::update_value:
				(*(path.top()))->value = std::move(value);
				return;
		}
	}

	this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1)), key, std::move(value));

	this->balance(path);
}

template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
	_insertion_strategy = insertion_strategy;
}

template<
		typename tkey,
		typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
	return this->_tree->get_allocator();
}

// * endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// * region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
		binary_search_tree<tkey, tvalue> *tree):
		binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{

}

template<
		typename tkey,
		typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
		tkey const &key)
{
	auto path = this->find_path(key);
	if (*(path.top()) == nullptr) {
		throw obtaining_of_nonexistent_key_attempt_exception(key);
	}

	tvalue const &got_value = (*(path.top()))->value;

	this->balance(path);

	return got_value;
}

// * endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// * region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
		binary_search_tree<tkey, tvalue> *tree,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
		binary_search_tree<tkey, tvalue>::template_method_basics(tree)
{
	throw not_implemented("template<typename tkey, typename tvalue> binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(binary_search_tree<tkey, tvalue> *, typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_strategy)", "your code should be here...");
}

template<
		typename tkey,
		typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
		tkey const &key)
{
	auto path = this->find_path(key);
	if (*(path.top()) == nullptr) {
		switch (_disposal_strategy) {
			case disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
				throw disposal_of_nonexistent_key_attempt_exception(key);
			case disposal_of_nonexistent_key_attempt_strategy::do_nothing:
				return tvalue();
		}
	}

	if ((*(path.top()))->left_subtree and (*(path.top()))->right_subtree) {
		auto *target_to_swap = *(path.top());
		auto **current = &((*(path.top()))->left_subtree);

		while (*current) {
			path.push(current);
			current = &((*current)->right_subtree);
		}

		swap(std::move(target_to_swap->key), std::move((*(path.top()))->key));
		swap(std::move(target_to_swap->value), std::move((*(path.top()))->value));
	}

	tvalue value = std::move((*(path.top()))->value);
	node *leftover_subtree = (*(path.top()))->left_subtree == nullptr
							 ? (*(path.top()))->right_subtree
							 : (*(path.top()))->left_subtree;
	allocator::destruct(*(path.top()));
	deallocate_with_guard(*(path.top()));
	*(path.top()) = leftover_subtree;
	this->balance(path);
	return value;
}

template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
	_disposal_strategy = disposal_strategy;
}

template<
		typename tkey,
		typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
	return this->_tree->get_allocator();
}

// * endregion search_tree<tkey, tvalue>::disposal_template_method implementation

// * endregion template methods

// * region construction, assignment, destruction implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
		typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
		typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
		typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
		std::function<int(tkey const &, tkey const &)> comparer,
		allocator *allocator,
		logger *logger):
		search_tree<tkey, tvalue>(comparer, logger, allocator),
		_root(nullptr)
{

}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
		std::function<int(tkey const &, tkey const &)> keys_comparer,
		allocator *allocator,
		logger *logger,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
		search_tree<tkey, tvalue>(keys_comparer, logger, allocator), _root(nullptr), delete_missing_strat(disposal_strategy), add_existing_strat(insertion_strategy)
{
    
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
		binary_search_tree<tkey, tvalue> const &other):
		search_tree<tkey, tvalue>(other), add_existing_strat(other.add_existing_strat), delete_missing_strat(other.delete_missing_strat)
{
	try {
		copy_subtree(&_root, other._root);
	}
	catch(...) {
		destroy_subtree(_root);
		throw;
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
		binary_search_tree<tkey, tvalue> &&other) noexcept:
		search_tree<tkey, tvalue>(other), add_existing_strat(other.add_existing_strat), delete_missing_strat(other.delete_missing_strat)
{
	_root = std::exchange(other._root, nullptr);
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
		binary_search_tree<tkey, tvalue> const &other)
{
	if (this != &other) {
		search_tree<tkey, tvalue>::operator=(other);
		add_existing_strat = other.add_existing_strat;
		delete_missing_strat = other.delete_missing_strat;

		try {
			copy_subtree(&_root, other._root);
		} catch (...) {
			destroy_subtree(_root);
			throw;
		}
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
		binary_search_tree<tkey, tvalue> &&other) noexcept
{
	if (this == &other) { // !!!
		return *this;
	}

	search_tree<tkey, tvalue>::operator = (std::move(other));
	add_existing_strat = other.add_existing_strat;
	delete_missing_strat = other.delete_missing_strat;

	std::swap(_root, other._root);

	return *this;
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
	destroy_subtree(_root);
}

// * endregion construction, assignment, destruction implementation

// * region associative_container<tkey, tvalue> contract implementation

template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
		tkey const &key,
		tvalue const &value)
{
	std::stack <node**> path = find_path_to_node(key);

	if (!*path.top()) {
		insert_inside(key, value, path);
	} else {
		if (add_existing_strat == insertion_of_existent_key_attempt_strategy::throw_an_exception) {
			throw insertion_of_existent_key_attempt_exception(key);
		} else {
			(*path.top())->value = value;
		}
	}

    // _insertion_tem
}

template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
		tkey const &key,
		tvalue &&value)
{
	std::stack <node**> path = find_path_to_node(key);

	if (*path.top() == nullptr) {
		insert_inside(key, value, path);
	} else {
		if (add_existing_strat == insertion_of_existent_key_attempt_strategy::throw_an_exception) {
			throw insertion_of_existent_key_attempt_exception(key);
		} else
		{
			(*path.top())->value = std::move(value);
		}
	}
}

template<
		typename tkey,
		typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
		tkey const &key)
{
	std::stack <node**> path = find_path_to_node(key);

	if (*path.top() == nullptr) {
		throw obtaining_of_nonexistent_key_attempt_exception(key);
	}

	return obtain_inside(path);
}

template<
		typename tkey,
		typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
		tkey const &lower_bound,
		tkey const &upper_bound,
		bool lower_bound_inclusive,
		bool upper_bound_inclusive)
{
	auto path = this->find_path_to_node(lower_bound);

	if (*path.top() == nullptr) {
		path.pop();
	}

	infix_iterator iterator (path, this->create_iterator_data());
	infix_iterator end = end_infix();

	std::vector<typename associative_container<tkey, tvalue>::key_value_pair> result;

	if (iterator != end) {
		int compare = search_tree<tkey, tvalue>::_keys_comparer((*iterator)->key, lower_bound);

		if (compare < 0) {
			++iterator;
		} else if (compare == 0) {
			if (lower_bound_inclusive) {
				result.emplace_back((*iterator)->key, (*iterator)->value);
			}
			++iterator;
		}

		compare = search_tree<tkey, tvalue>::_keys_comparer((*iterator)->key, upper_bound);

		while (iterator != end and compare < 0) {
			result.emplace_back((*iterator)->key, (*iterator)->value);
			++iterator;
			compare = search_tree<tkey, tvalue>::_keys_comparer((*iterator)->key, upper_bound);
		}

		if (iterator != end and compare == 0 and upper_bound_inclusive) {
			result.emplace_back((*iterator)->key, (*iterator)->value);
		}
	}

	return result;
}

template<
		typename tkey,
		typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
		tkey const &key)
{
	std::stack <node**> path = find_path_to_node(key);

	if (*path.top() == nullptr) {
		if (delete_missing_strat == disposal_of_nonexistent_key_attempt_strategy::throw_an_exception) {
			throw disposal_of_nonexistent_key_attempt_exception(key);
		} else
		{
			if constexpr (std::is_default_constructible<tvalue>::value) {
				return {};
			}
			else
			{
				throw disposal_of_nonexistent_key_attempt_strategy(key);
			}
		}
	}

	return dispose_inside(path);
}

// * endregion associative_containers contract implementations


// * regi on help-function
template<typename tkey, typename tvalue>
std::stack<typename binary_search_tree<tkey, tvalue>::node**> binary_search_tree<tkey, tvalue>::find_path_to_node(const tkey& key)
{
	std::stack <node**> result;

	result.push(&(_root));

	node** current_node  = &(_root);

	while (*current_node) {
		int cmp = search_tree<tkey, tvalue>::_keys_comparer(key, (*current_node)->key);

		if (cmp > 0) {
			current_node = &((*current_node)->right_subtree);
		} else if (cmp < 0) {
			current_node = &((*current_node)->left_subtree);
		} else
		{
			break;
		}

		result.push(current_node);
	}

	return result;
}


template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::insert_inside(const tkey& key, tvalue&& value, std::stack <node**>& stack)
{
	(*stack.top()) =  static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node),1));
	try {
		allocator::construct(*stack.top(), key, std::move(value));
	}
	catch(...) {
		allocator_guardant::deallocate_with_guard(*stack.top());
		throw;
	}
}

template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::insert_inside(const tkey& key, const tvalue& value, std::stack <node**>& stack)
{
	(*stack.top()) =  static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node),1));
	try {
		allocator::construct(*stack.top(), key, value);
	}
	catch(...) { //!!!!
		allocator_guardant::deallocate_with_guard(*stack.top());
		throw;
	}
}

template <typename tkey, typename tvalue>
tvalue const& binary_search_tree<tkey, tvalue>::obtain_inside(std::stack <node**>& stack)
{
	return (*stack.top())->value;
}

template <typename tkey, typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose_inside(std::stack <node**>& stack)
{
	node* current_node = *stack.top();

	if (current_node->left_subtree == nullptr and current_node->right_subtree == nullptr) {
		*stack.top() = nullptr;
	} else if (current_node->right_subtree == nullptr or current_node->left_subtree == nullptr) {
		node* update_node = current_node->right_subtree ? (current_node->right_subtree) : (current_node->left_subtree);

		*stack.top() = update_node;
	} else {
		node** update = &((*stack.top())->left_subtree);

		while ((*update)->right_subtree) {
			update = &((*update)->right_subtree);
		}

		node* previous_node = *stack.top();

		*(stack.top()) = *update;

		*update = (*update)->left_subtree;

		(*stack.top())->left_subtree = previous_node->left_subtree == *(stack.top()) ? *update : previous_node->left_subtree;
		(*stack.top())->right_subtree = previous_node->right_subtree;

	}

	tvalue res = current_node->value;
	allocator::destruct(current_node);
	allocator_guardant::deallocate_with_guard(current_node);
	return res;
}

template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::copy_subtree(node** dest, node* src)
{
	if (src == nullptr) {
		*dest = nullptr;
	}

	*dest = reinterpret_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node)));

	try{
		allocator::construct(*dest, *src);
	}
	catch(...) {
		allocator_guardant::deallocate_with_guard(*dest);
		*dest = nullptr;
		throw;
	}

	(*dest)->right_subtree = nullptr;
	(*dest)->left_subtree = nullptr;

	copy_subtree(&((*dest)->left_subtree), src->left_subtree);
	copy_subtree(&((*dest)->right_subtree), src->right_subtree);
}


template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::destroy_subtree(node* src) noexcept
{
	if (src) {
		destroy_subtree(src->left_subtree);
		destroy_subtree(src->right_subtree);

		allocator::destruct(src);
		allocator_guardant::deallocate_with_guard(src);
	}
}

// end region help_function

// strategies
template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
	add_existing_strat = insertion_strategy;
}

template<
		typename tkey,
		typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
	delete_missing_strat = disposal_strategy;
}

// * region iterators requesting implementation

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
	std::stack <node**> stack;
	if (_root) {
		stack.push(&_root);
	}

	return prefix_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
	std::stack <node**> stk;
	if (_root) {
		stk.push((&_root));
	}

	return prefix_const_iterator(stk, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_const_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
	std::stack <node**> stack;
	if (_root) {
		stack.push(&(_root));
	}

	return prefix_reverse_iterator(stack, new iterator_data);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
	std::stack <node**> stack;
	if (_root) {
		stack.push(&(_root));
	}

	return prefix_const_reverse_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = &_root;
	while (*current_node) {
		stack.push(current_node);
		current_node = &((*current_node)->left_subtree);
	}

	return infix_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = (&_root);
	while (*current_node) {
		stack.push(current_node);
		current_node = (&((*current_node)->left_subtree));
	}

	return infix_const_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_const_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = &_root;
	while (*current_node) {
		stack.push(current_node);
		current_node = &((*current_node)->right_subtree);
	}

	return infix_reverse_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = &_root;
	while (*current_node) {
		stack.push(current_node);
		current_node = &((*current_node)->right_subtree);
	}

	return infix_const_reverse_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = &_root;
	while (*current_node) {
		stack.push(current_node);
		current_node = (*current_node)->left_subtree ? &((*current_node)->left_subtree) : &((*current_node)->right_subtree);
	}
	return postfix_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = (&_root);
	while (*current_node) {
		stack.push(current_node);
		current_node = (*current_node)->left_subtree ? (&((*current_node)->left_subtree)) : (&((*current_node)->right_subtree));
	}
	return postfix_const_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_const_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = &_root;
	while (*current_node) {
		stack.push(current_node);
		current_node = (*current_node)->right_subtree ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
	}
	return postfix_reverse_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(std::stack <node**>(), nullptr);
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
	std::stack <node**> stack;
	node** current_node = &_root;
	while (*current_node) {
		stack.push(current_node);
		current_node = (*current_node)->right_subtree ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
	}
	return postfix_const_reverse_iterator(stack, this->create_iterator_data());
}

template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(std::stack <node**>(), nullptr);
}

// * endregion iterators request implementation * //

// * region subtree rotations implementation * //

template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(binary_search_tree<tkey, tvalue>::node *&subtree_root, bool validate) const
{
    if (validate and (subtree_root == nullptr or subtree_root->right_subtree == nullptr)) {
        throw std::logic_error("small_left_rotation: can't rotate");
    }

    node* new_root = subtree_root->right_subtree;
    subtree_root->right_subtree = new_root->left_subtree;
    
    new_root->left_subtree = subtree_root;
    subtree_root = new_root;
}

template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(binary_search_tree<tkey, tvalue>::node *&subtree_root, bool validate) const
{
    if (validate and (subtree_root == nullptr or subtree_root->left_subtree == nullptr)) {
        throw std::logic_error("small_right_rotation: can't rotate");
    }

    node* new_root = subtree_root->left_subtree;
    subtree_root->left_subtree = new_root->right_subtree;

    new_root->right_subtree = subtree_root;
    subtree_root = new_root;
}

template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(binary_search_tree<tkey, tvalue>::node *&subtree_root,bool validate) const
{
    if (validate and (subtree_root == nullptr or subtree_root->right_subtree == nullptr or subtree_root->right_subtree->left_subtree == nullptr)) {
        throw std::logic_error("big_left_rotation: can't rotate");
    }

    small_right_rotation(subtree_root->right_subtree, false);
    small_left_rotation(subtree_root, false);
}

template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(binary_search_tree<tkey, tvalue>::node *&subtree_root, bool validate) const
{
    if (validate and (subtree_root == nullptr or subtree_root->left_subtree == nullptr or subtree_root->left_subtree->right_subtree == nullptr)) {
        throw std::logic_error("big_right_rotation: can't rotate");
    }

    small_left_rotation(subtree_root->left_subtree, false);
    small_right_rotation(subtree_root, false);
}

template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(binary_search_tree<tkey, tvalue>::node *&subtree_root, bool at_grandparent_first, bool validate) const
{

    if (validate and (subtree_root == nullptr or subtree_root->right_subtree == nullptr or subtree_root->right_subtree->right_subtree == nullptr)) {
        throw std::logic_error("double_left_rotation: can't rotate");
    }

    if (at_grandparent_first) {
        small_left_rotation(subtree_root, false);
        small_left_rotation(subtree_root, false);
    } else {
        small_left_rotation(subtree_root->right->subtree, false);
        small_left_rotation(subtree_root, false);
    }
}
template <typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(binary_search_tree<tkey, tvalue>::node *&subtree_root, bool at_grandparent_first, bool validate) const
{
    if (validate and (subtree_root == nullptr or subtree_root->left_subtree == nullptr or subtree_root->left_subtree->left_subtree == nullptr)) {
        throw std::logic_error("double_right_rotation: can't rotate");
    }

    if (at_grandparent_first) {
        small_right_rotation(subtree_root, false);
        small_right_rotation(subtree_root, false);
    } else {
        small_right_rotation(subtree_root->left_subtree, false);
        small_right_rotation(subtree_root, false);
    }
}

// * endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
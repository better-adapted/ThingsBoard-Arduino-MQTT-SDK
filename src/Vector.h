#ifndef Vector_h
#define Vector_h

// Local include.
#include "Configuration.h"

// Library includes.
#include <assert.h>


/// @brief Replacement data container for boards that do not support the C++ STL and therefore do not have the std::vector class.
/// @tparam T Type of the underlying data the list should point too.
template <typename T>
class Vector {
  public:
    /// @brief Constructor
    Vector(void)
      : m_elements(nullptr)
      , m_capacity(0U)
      , m_size(0U)
    {
        // Nothing to do
    }

    /// @brief Constructor that allows compatibility with std::vector, simply forwards call to internal insert method
    /// @param first Beginning of the elements we want to copy into our underlying data container
    /// @param last One past the end of the elements we want to copy into our underlying data container
    Vector(T const * const first, T const * const last)
      : m_elements(nullptr)
      , m_capacity(0U)
      , m_size(0U)
    {
        insert(nullptr, first, last);
    }

    /// @brief Destructor
    ~Vector() {
        delete[] m_elements;
        m_elements = nullptr;
    }

    /// @brief Method that allows compatibility with std::vector, simply forwards call to internal insert method
    /// @param first Beginning of the elements we want to copy into our underlying data container
    /// @param last One past the end of the elements we want to copy into our underlying data container
    void assign(T const * const first, T const * const last) {
        insert(nullptr, first, last);
    }

    /// @brief Returns whether there are still any element in the underlying data container
    /// @return Whether the underlying data container is empty or not
    bool empty() const {
        return m_size == 0U;
    }

    /// @brief Gets the current amount of elements in the underlying data container
    /// @return The amount of items currently in the underlying data container
    size_t const & size() const {
        return m_size;
    }

    /// @brief Gets the maximum amount of elements that can currently be stored in the underlying data container
    /// @return The maximum amount of items that can currently be stored in the underlying data container
    size_t const & capacity() const {
        return m_capacity;
    }

    /// @brief Returns a pointer to the first element of the vector
    /// @return Pointer to the first element of the vector
    T * begin() {
        return m_elements;
    }

    /// @brief Returns a constant pointer to the first element of the vector
    /// @return Constant pointer to the first element of the vector
    T const * begin() const {
        return m_elements;
    }

    /// @brief Returns a constant pointer to the first element of the vector
    /// @return Constant pointer to the first element of the vector
    T const * cbegin() const {
        return m_elements;
    }

    /// @brief Returns the last element of the vector
    /// @return Reference to the last element of the vector
    T & back() {
        assert(m_size != 0U);
        return m_elements[m_size - 1U];
    }

    /// @brief Returns a pointer to one-past-the-end element of the vector
    /// @return Pointer to one-past-the-end element of the vector
    T * end() {
        return m_elements + m_size;
    }

    /// @brief Returns a constant pointer to one-past-the-end element of the vector
    /// @return Constant pointer to one-past-the-end element of the vector
    T const * end() const {
        return m_elements + m_size;
    }

    /// @brief Returns a constant pointer to one-past-the-end element of the vector
    /// @return Constant pointer to one-past-the-end element of the vector
    T const * cend() const {
        return m_elements + m_size;
    }

    /// @brief Inserts the given element at the end of the underlying data container.
    /// Increases the size of the underlying data container if it is already full and has no remaining space for new elements.
    /// The growth is exponential meaning it doubles every time the underlying data container has to be increased.
    /// This results in an amortized insertion speed of O(1), but might require 0(n) when the container has to be reallocated to be increased
    /// @param element Element that should be inserted at the end
    void push_back(T const & element) {
        if (m_size == m_capacity) {
            m_capacity = (m_capacity == 0) ? 1 : 2 * m_capacity;
            T* newElements = new T[m_capacity];
            if (m_elements != nullptr) {
                memcpy(newElements, m_elements, m_size * sizeof(T));
                delete[] m_elements;
            }
            m_elements = newElements;
        }
        m_elements[m_size] = element;
        m_size++;
    }

    /// @brief Inserts all element from the given start to the given end iterator into the underlying data container. Simply calls push_back on each element
    /// @param position Attribute is not used and can be left as nullptr, simply there to keep compatibility with std::vector insert method
    /// @param first Beginning of the elements we want to copy into our underlying data container
    /// @param last One past the end of the elements we want to copy into our underlying data container
    void insert(T const * const position, T const * first, T const * const last) {
        while (first < last) {
            push_back(*first);
            first++;
        }
    }

    /// @brief Removes the element at the given iterator, has to move all element one to the left if the index is not at the end of the array
    /// @param iterator Iterator the element should be removed at from the underlying data container
    void erase(T const * const iterator) {
        size_t const index = Helper::distance(cbegin(), iterator);
        // Check if the given index is bigger or equal than the actual amount of elements if it is we can not erase that element because it does not exist
        if (index < m_size) {
            // Move all elements after the index one position to the left
            for (size_t i = index; i < m_size - 1; ++i) {
                m_elements[i] = m_elements[i + 1];
            }
            // Decrease the size of the vector to remove the last element, because either it was moved one index to the left or was the element we wanted to delete
            m_size--;
        }
    }

    /// @brief Method to access an element at a given index,
    /// ensures the device crashes if we attempted to access in an invalid location
    /// @param index Index we want to get the corresponding element for
    T & at(size_t const & index) {
        assert(index < m_size);
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index.
    /// Does not do any bounds checks, meaning the access is more efficient but it is possible to read out of bounds data
    /// @param index Index we want to get the corresponding element for
    T & operator[](size_t const & index) {
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index.
    /// Does not do any bounds checks, meaning the access is more efficient but it is possible to read out of bounds data
    /// @param index Index we want to get the corresponding element for
    T const & operator[](size_t const & index) const {
        return m_elements[index];
    }

    /// @brief Clears the given underlying data container.
    /// Simply sets the underlying size to 0, data will only be cleared in the destructor
    void clear() {
        m_size = 0;
    }

  private:
    T      *m_elements; // Pointer to the start of our elements
    size_t m_capacity;  // Allocated capacity that shows how many elements we could hold
    size_t m_size;      // Used size that shows how many elements we entered
};

#endif // Vector_h

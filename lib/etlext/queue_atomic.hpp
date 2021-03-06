#pragma once

#include <lib/etl/queue.h>
#include <lib/etl/atomic.h>
#include <cmath>

namespace etlext {

//***************************************************************************
/// The base class for all queues.
///\ingroup queue
//***************************************************************************
class queue_atomic_base
{
public:

  typedef etl::atomic_size_t size_type; ///< The type used for determining the size of queue.

  //*************************************************************************
  /// Returns the current number of items in the queue.
  //*************************************************************************
  size_type size() const
  {
    return current_size;
  }

  //*************************************************************************
  /// Returns the maximum number of items that can be queued.
  //*************************************************************************
  size_type max_size() const
  {
    return CAPACITY;
  }

  //*************************************************************************
  /// Checks to see if the queue is empty.
  /// \return <b>true</b> if the queue is empty, otherwise <b>false</b>
  //*************************************************************************
  bool empty() const
  {
    return current_size == 0;
  }

  //*************************************************************************
  /// Checks to see if the queue is full.
  /// \return <b>true</b> if the queue is full, otherwise <b>false</b>
  //*************************************************************************
  bool full() const
  {
    return current_size == CAPACITY;
  }

  //*************************************************************************
  /// Returns the remaining capacity.
  ///\return The remaining capacity.
  //*************************************************************************
  size_t available() const
  {
    return max_size() - size();
  }

protected:

  //*************************************************************************
  /// The constructor that is called from derived classes.
  //*************************************************************************
  queue_atomic_base(size_type max_size_)
    : in(0),
      out(0),
      current_size(0),
      CAPACITY(max_size_)
  {
  }

  //*************************************************************************
  /// Increments (and wraps) the 'in' index value to record a queue addition.
  //*************************************************************************
  void add_in()
  {
    if (++in == CAPACITY)
    {
      in = 0;
    }

    ++current_size;
    ++construct_count;
  }

  //*************************************************************************
  /// Decrements (and wraps) the 'out' index value to record a queue deletion.
  //*************************************************************************
  void del_out()
  {
    if (++out == CAPACITY)
    {
      out = 0;
    }
    --current_size;
    --construct_count;
  }

  //*************************************************************************
  /// Clears the indexes.
  //*************************************************************************
  void index_clear()
  {
    in = 0;
    out = 0;
    current_size = 0;
    construct_count.clear();
  }

  size_type in;                     ///< Where to input new data.
  size_type out;                    ///< Where to get the oldest data.
  size_type current_size;           ///< The number of items in the queue.
  const size_type CAPACITY;         ///< The maximum number of items in the queue.
  etl::debug_count construct_count; ///< For internal debugging purposes.
};

//***************************************************************************
///\ingroup queue
///\brief This is the base for all queues that contain a particular type.
///\details Normally a reference to this type will be taken from a derived queue.
///\code
/// etl::queue<int, 10> myQueue;
/// etl::iqueue<int>& iQueue = myQueue;
///\endcode
/// \warning This queue cannot be used for concurrent access from multiple threads.
/// \tparam T The type of value that the queue holds.
//***************************************************************************
template <typename T>
class iqueue_atomic : public etlext::queue_atomic_base
{
public:

  typedef T                     value_type;      ///< The type stored in the queue.
  typedef T&                    reference;       ///< A reference to the type used in the queue.
  typedef const T&              const_reference; ///< A const reference to the type used in the queue.
  typedef T*                    pointer;         ///< A pointer to the type used in the queue.
  typedef const T*              const_pointer;   ///< A const pointer to the type used in the queue.
  typedef queue_atomic_base::size_type size_type;///< The type used for determining the size of the queue.

private:

  typedef typename etl::parameter_type<T>::type parameter_t;
  typedef queue_atomic_base              base_t;

public:

  //*************************************************************************
  /// Destructor.
  //*************************************************************************
  ~iqueue_atomic()
  {
    if (!empty())
    {
      clear();
    }
  }

  //*************************************************************************
  /// Gets a reference to the value at the front of the queue.<br>
  /// \return A reference to the value at the front of the queue.
  //*************************************************************************
  reference front()
  {
    return p_buffer[out];
  }

  //*************************************************************************
  /// Gets a const reference to the value at the front of the queue.<br>
  /// \return A const reference to the value at the front of the queue.
  //*************************************************************************
  const_reference front() const
  {
    return p_buffer[out];
  }

  //*************************************************************************
  /// Gets a reference to the value at the back of the queue.<br>
  /// \return A reference to the value at the back of the queue.
  //*************************************************************************
  reference back()
  {
    return p_buffer[in == 0 ? CAPACITY - 1 : in - 1];
  }

  //*************************************************************************
  /// Gets a const reference to the value at the back of the queue.<br>
  /// \return A const reference to the value at the back of the queue.
  //*************************************************************************
  const_reference back() const
  {
    return p_buffer[in == 0 ? CAPACITY - 1 : in - 1];
  }

  //*************************************************************************
  /// Adds a value to the queue.
  /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
  ///\param value The value to push to the queue.
  //*************************************************************************
  void push(parameter_t value)
  {
#if defined(ETL_CHECK_PUSH_POP)
    ETL_ASSERT(!full(), ETL_ERROR(queue_full));
#endif
    ::new (&p_buffer[in]) T(value);
    base_t::add_in();
  }

  //*************************************************************************
  /// Allows a possibly more efficient 'push' by moving to the next input value
  /// and returning a reference to it.
  /// This may eliminate a copy by allowing direct construction in-place.<br>
  /// If asserts or exceptions are enabled, throws an etl::queue_full is the queue is already full.
  /// \return A reference to the position to 'push' to.
  //*************************************************************************
  reference push()
  {
    const size_type next = in;

#if defined(ETL_CHECK_PUSH_POP)
    ETL_ASSERT(!full(), ETL_ERROR(queue_full));
#endif
    base_t::add_in();

    return p_buffer[next];
  }

  //*************************************************************************
  /// Constructs a value in the queue 'in place'.
  /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
  ///\param value The value to use to construct the item to push to the queue.
  //*************************************************************************
  template <typename T1>
  void emplace(const T1& value1)
  {
#if defined(ETL_CHECK_PUSH_POP)
    ETL_ASSERT(!full(), ETL_ERROR(queue_full));
#endif
    ::new (&p_buffer[in]) T(value1);
    base_t::add_in();
  }

  //*************************************************************************
  /// Constructs a value in the queue 'in place'.
  /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
  ///\param value The value to use to construct the item to push to the queue.
  //*************************************************************************
  template <typename T1, typename T2>
  void emplace(const T1& value1, const T2& value2)
  {
#if defined(ETL_CHECK_PUSH_POP)
    ETL_ASSERT(!full(), ETL_ERROR(queue_full));
#endif
    ::new (&p_buffer[in]) T(value1, value2);
    base_t::add_in();
  }

  //*************************************************************************
  /// Constructs a value in the queue 'in place'.
  /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
  ///\param value The value to use to construct the item to push to the queue.
  //*************************************************************************
  template <typename T1, typename T2, typename T3>
  void emplace(const T1& value1, const T2& value2, const T3& value3)
  {
#if defined(ETL_CHECK_PUSH_POP)
    ETL_ASSERT(!full(), ETL_ERROR(queue_full));
#endif
    ::new (&p_buffer[in]) T(value1, value2, value3);
    base_t::add_in();
  }

  //*************************************************************************
  /// Constructs a value in the queue 'in place'.
  /// If asserts or exceptions are enabled, throws an etl::queue_full if the queue if already full.
  ///\param value The value to use to construct the item to push to the queue.
  //*************************************************************************
  template <typename T1, typename T2, typename T3, typename T4>
  void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
  {
#if defined(ETL_CHECK_PUSH_POP)
    ETL_ASSERT(!full(), ETL_ERROR(queue_full));
#endif
    ::new (&p_buffer[in]) T(value1, value2, value3, value4);
    base_t::add_in();
  }

  //*************************************************************************
  /// Clears the queue to the empty state.
  //*************************************************************************
  void clear()
  {
    if ETL_IF_CONSTEXPR(etl::is_trivially_destructible<T>::value)
    {
      base_t::index_clear();
    }
    else
    {
      while (current_size > 0)
      {
        p_buffer[out].~T();
        base_t::del_out();
      }

      in = 0;
      out = 0;
    }
  }

  //*************************************************************************
  /// Removes the oldest value from the back of the queue.
  /// Does nothing if the queue is already empty.
  /// If asserts or exceptions are enabled, throws an etl::queue_empty if the queue is empty.
  //*************************************************************************
  void pop()
  {
#if defined(ETL_CHECK_PUSH_POP)
    ETL_ASSERT(!empty(), ETL_ERROR(queue_empty));
#endif
    p_buffer[out].~T();
    base_t::del_out();
  }

  //*************************************************************************
  /// Gets the oldest value and removes it from the front of the queue.
  /// If asserts or exceptions are enabled, throws an etl::queue_empty if the queue is empty.
  //*************************************************************************
  void pop_into(reference destination)
  {
    destination = front();
    pop();
  }

  //*************************************************************************
  /// Gets the oldest value and removes it from the front of the queue and
  /// pushes it to the destination container.
  /// If asserts or exceptions are enabled, throws an etl::queue_empty if the queue is empty.
  /// NOTE: The destination must support a push(T) member function.
  //*************************************************************************
  template <typename TContainer>
  void pop_into(TContainer& destination)
  {
    destination.push(front());
    pop();
  }

  //*************************************************************************
  /// Assignment operator.
  //*************************************************************************
  iqueue_atomic& operator = (const iqueue_atomic& rhs)
  {
    if (&rhs != this)
    {
      clear();
      clone(rhs);
    }

    return *this;
  }

protected:

  //*************************************************************************
  /// Make this a clone of the supplied queue
  //*************************************************************************
  void clone(const iqueue_atomic& other)
  {
    clear();

    size_t index = other.out;

    for (size_t i = 0; i < other.size(); ++i)
    {
      push(other.p_buffer[index]);
      index = (index == (CAPACITY - 1)) ? 0 : index + 1;
    }
  }

  //*************************************************************************
  /// The constructor that is called from derived classes.
  //*************************************************************************
  iqueue_atomic(T* p_buffer_, size_type max_size_)
    : queue_atomic_base(max_size_),
      p_buffer(p_buffer_)
  {
  }

private:

  // Disable copy construction.
  iqueue_atomic(const iqueue_atomic&);

  T* p_buffer; ///< The internal buffer.
};

//***************************************************************************
///\ingroup queue
/// A fixed capacity queue.
/// This queue does not support concurrent access by different threads.
/// \tparam T    The type this queue should support.
/// \tparam SIZE The maximum capacity of the queue.
//***************************************************************************
template <typename T, const size_t SIZE>
class queue_atomic : public etlext::iqueue_atomic<T>
{
public:

  static const size_t MAX_SIZE = SIZE;

  //*************************************************************************
  /// Default constructor.
  //*************************************************************************
  queue_atomic()
    : iqueue_atomic<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
  {
  }

  //*************************************************************************
  /// Copy constructor
  //*************************************************************************
  queue_atomic(const queue_atomic& rhs)
    : iqueue_atomic<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
  {
      iqueue_atomic<T>::clone(rhs);
  }

  //*************************************************************************
  /// Destructor.
  //*************************************************************************
  ~queue_atomic()
  {
    iqueue_atomic<T>::clear();
  }

  //*************************************************************************
  /// Assignment operator.
  //*************************************************************************
  queue_atomic& operator = (const queue_atomic& rhs)
  {
    if (&rhs != this)
    {
        iqueue_atomic<T>::clone(rhs);
    }

    return *this;
  }

private:

  /// The uninitialised buffer of T used in the stack.
  typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[SIZE];
};

}

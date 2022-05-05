#include <algorithm>
#include <atomic>
#include <memory>
#include <type_traits>

class imemory_block_allocator {
 public:
  imemory_block_allocator() {}

  virtual ~imemory_block_allocator() {}

 private:
  imemory_block_allocator(const imemory_block_allocator&) = delete;
  imemory_block_allocator& operator=(const imemory_block_allocator&) = delete;
};

template <size_t VBlock_Size, size_t VAlignment, size_t VSize>
struct fixed_sized_memory_block_allocator : public imemory_block_allocator {};

typedef uint_least8_t message_id_t;

class imessage {
 public:
  virtual ~imessage() {}
};

template <message_id_t ID_, typename TParent = imessage>
class message : public TParent {
 public:
  enum { ID = ID_ };
};

class ireference_counted_message;

class ireference_counted_message_pool {
 public:
  virtual ~ireference_counted_message_pool() {}
};

typedef std::atomic<int> atomic_int;
typedef std::atomic<int32_t> atomic_int32_t;

class ireference_counter {
 public:
  virtual ~ireference_counter() {}
};

template <typename TCounter>
class reference_counter : public ireference_counter {
 public:
  reference_counter() : reference_count(0) {}

 private:
  TCounter reference_count;
};

template <>
class reference_counter<void> : public ireference_counter {
 public:
  reference_counter() {}
};

class ireference_counted_object {
 public:
  virtual ~ireference_counted_object() {}
};

template <typename TObject, typename TCounter>
class reference_counted_object : public ireference_counted_object {
 public:
  typedef TObject value_type;
  typedef TCounter counter_type;

  reference_counted_object(const TObject& object_) : object(object_) {}

 private:
  reference_counted_object(const reference_counted_object&) = delete;
  reference_counted_object& operator=(const reference_counted_object&) = delete;

  TObject object;
  reference_counter<TCounter> reference_counter_;
};

class ireference_counted_message {
 public:
  virtual ~ireference_counted_message() {}
};

template <typename TMessage, typename TCounter>
class reference_counted_message : public ireference_counted_message {
 public:
  typedef TMessage message_type;
  typedef TCounter counter_type;

  reference_counted_message(const TMessage& msg_,
                            ireference_counted_message_pool& owner_)
      : rc_object(msg_), owner(owner_) {}

 private:
  reference_counted_object<TMessage, TCounter> rc_object;
  ireference_counted_message_pool& owner;
};

template <typename TMessage>
using atomic_counted_message =
    reference_counted_message<TMessage, atomic_int32_t>;

template <typename TCounter>
class reference_counted_message_pool : public ireference_counted_message_pool {
 public:
  reference_counted_message_pool(
      imemory_block_allocator& memory_block_allocator_)
      : memory_block_allocator(memory_block_allocator_) {}

  template <typename TMessage1, typename... TMessages>
  struct pool_message_parameters {
   private:
    static constexpr size_t size1 =
        sizeof(reference_counted_message<TMessage1, TCounter>);

    static constexpr size_t size2 =
        pool_message_parameters<TMessages...>::max_size;

    static constexpr size_t alignment1 = std::alignment_of<
        reference_counted_message<TMessage1, TCounter>>::value;

    static constexpr size_t alignment2 =
        pool_message_parameters<TMessages...>::max_alignment;

   public:
    static constexpr size_t max_size = (size1 < size2) ? size2 : size1;

    static constexpr size_t max_alignment =
        (alignment1 < alignment2) ? alignment2 : alignment1;
  };

  template <typename TMessage1>
  struct pool_message_parameters<TMessage1> {
   public:
    static constexpr size_t max_size =
        sizeof(reference_counted_message<TMessage1, TCounter>);

    static constexpr size_t max_alignment = std::alignment_of<
        reference_counted_message<TMessage1, TCounter>>::value;
  };

 private:
  imemory_block_allocator& memory_block_allocator;

  reference_counted_message_pool(const reference_counted_message_pool&) =
      delete;
  reference_counted_message_pool& operator=(
      const reference_counted_message_pool&) = delete;
};

using atomic_counted_message_pool = reference_counted_message_pool<atomic_int>;

constexpr message_id_t MessageId1 = 1U;
constexpr message_id_t MessageId2 = 2U;

struct Message1 : public message<MessageId1> {
  Message1(int i_) : i(i_) {}

  Message1(const Message1& oth) { i = oth.i; }

  ~Message1() override {}

  int i;
};

struct Message2 : public message<MessageId2> {
  ~Message2() override {}
};

int main() {
  using pool_message_parameters =
      atomic_counted_message_pool::pool_message_parameters<Message1, Message2>;

  using pool_message_parameters =
      atomic_counted_message_pool::pool_message_parameters<Message1, Message2>;

  fixed_sized_memory_block_allocator<pool_message_parameters::max_size,
                                     pool_message_parameters::max_alignment, 4U>
      memory_allocator;

  atomic_counted_message_pool message_pool(memory_allocator);

  Message1 message1(6);
  reference_counted_message<Message1, atomic_int> temp(message1, message_pool);
  const ireference_counted_message& rcmessage = temp;
  rcmessage.~ireference_counted_message();
}

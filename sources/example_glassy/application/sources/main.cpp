#include <atomic>
#include <memory>

typedef uint_least8_t message_id_t;

class imessage {
 public:
  virtual ~imessage() {}
};

template <message_id_t ID_, typename TParent = imessage>
class message : public TParent {};

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

  reference_counted_message(const TMessage& msg_) : rc_object(msg_) {}

 private:
  reference_counted_object<TMessage, TCounter> rc_object;
};

constexpr message_id_t MessageId1 = 1U;

struct Message1 : public message<MessageId1> {
  Message1() {}
  ~Message1() override {}
  Message1(const Message1&) {}
};

int main() {
  Message1 message1;
  reference_counted_message<Message1, atomic_int> temp(message1);
  const ireference_counted_message& rcmessage = temp;
  rcmessage.~ireference_counted_message();
}

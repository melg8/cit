#include <stddef.h>
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>
#include <tuple>
#include <type_traits>

namespace etl {

template <typename T>
class successor {
 public:
  typedef T successor_type;

  successor() : p_successor(nullptr) {}

  successor(successor_type& s) : p_successor(&s) {}

  void set_successor(successor_type& s) { p_successor = &s; }

  successor_type& get_successor() const { return *p_successor; }

  bool has_successor() const { return (p_successor != nullptr); }

 private:
  successor_type* p_successor;
};
}  // namespace etl

namespace etl {

class imemory_block_allocator : public successor<imemory_block_allocator> {
 public:
  imemory_block_allocator() {}

  virtual ~imemory_block_allocator() {}

 private:
  imemory_block_allocator(const etl::imemory_block_allocator&) = delete;
  imemory_block_allocator& operator=(const imemory_block_allocator&) = delete;
};
}  // namespace etl

namespace etl {

class exception {
 public:
  typedef const char* string_type;
  typedef int numeric_type;

  exception(string_type reason_, string_type file_, numeric_type line_)
      : reason_text(reason_), file_text(file_), line(line_) {}

  string_type what() const { return reason_text; }

  string_type file_name() const { return file_text; }

  numeric_type line_number() const { return line; }

 private:
  string_type reason_text;

  string_type file_text;

  numeric_type line;
};
}  // namespace etl

namespace etl {

template <typename TParameter>
class ifunction {
 public:
  typedef TParameter parameter_type;

  virtual ~ifunction() {}

  virtual void operator()(TParameter) const = 0;
};

template <>
class ifunction<void> {
 public:
  typedef void parameter_type;

  virtual ~ifunction() {}

  virtual void operator()() const = 0;
};

template <typename TObject, typename TParameter>
class function : public ifunction<TParameter> {
 public:
  typedef TObject object_type;
  typedef TParameter parameter_type;

  function(TObject& object_, void (TObject::*p_function_)(TParameter))
      : p_object(&object_), p_function(p_function_) {}

  virtual void operator()(TParameter data) const override {
    (p_object->*p_function)(data);
  }

 private:
  TObject* p_object;
  void (TObject::*p_function)(TParameter);
};

template <typename TObject>
class function<TObject, void> : public ifunction<void> {
 public:
  function(TObject& object_, void (TObject::*p_function_)(void))
      : p_object(&object_), p_function(p_function_) {}

  virtual void operator()() const override { (p_object->*p_function)(); }

 private:
  TObject* p_object;
  void (TObject::*p_function)();
};

template <typename TParameter>
class function<void, TParameter> : public ifunction<TParameter> {
 public:
  explicit function(void (*p_function_)(TParameter))
      : p_function(p_function_) {}

  virtual void operator()(TParameter data) const override {
    (*p_function)(data);
  }

 private:
  void (*p_function)(TParameter);
};

template <>
class function<void, void> : public ifunction<void> {
 public:
  explicit function(void (*p_function_)(void)) : p_function(p_function_) {}

  virtual void operator()() const override { (*p_function)(); }

 private:
  void (*p_function)();
};

template <typename TObject,
          typename TParameter,
          void (TObject::*Function)(TParameter)>
class function_mp : public ifunction<TParameter> {
 public:
  typedef TObject object_type;
  typedef TParameter parameter_type;

  explicit function_mp(TObject& object_) : p_object(&object_) {}

  virtual void operator()(TParameter data) const override {
    (p_object->*Function)(data);
  }

 private:
  TObject* p_object;
};

template <typename TObject, void (TObject::*Function)(void)>
class function_mv : public ifunction<void> {
 public:
  typedef TObject object_type;
  typedef void parameter_type;

  explicit function_mv(TObject& object_) : p_object(&object_) {}

  virtual void operator()() const override { (p_object->*Function)(); }

 private:
  TObject* p_object;
};

template <typename TObject,
          typename TParameter,
          TObject& Instance,
          void (TObject::*Function)(TParameter)>
class function_imp : public ifunction<TParameter> {
 public:
  typedef TObject object_type;
  typedef TParameter parameter_type;

  virtual void operator()(TParameter data) const override {
    (Instance.*Function)(data);
  }
};

template <typename TObject, TObject& Instance, void (TObject::*Function)(void)>
class function_imv : public ifunction<void> {
 public:
  typedef TObject object_type;
  typedef void parameter_type;

  virtual void operator()() const override { (Instance.*Function)(); }
};

template <typename TParameter, void (*Function)(TParameter)>
class function_fp : public ifunction<TParameter> {
 public:
  typedef TParameter parameter_type;

  function_fp() {}

  virtual void operator()(TParameter data) const override { (*Function)(data); }
};

template <void (*Function)(void)>
class function_fv : public ifunction<void> {
 public:
  typedef void parameter_type;

  function_fv() {}

  virtual void operator()() const override { (*Function)(); }
};

}  // namespace etl

namespace etl {

class error_handler {
 public:
  struct free_function : public etl::function<void, const etl::exception&> {
    explicit free_function(void (*p_function_)(const etl::exception&))
        : etl::function<void, const etl::exception&>(p_function_) {}
  };

  template <typename TObject>
  struct member_function
      : public etl::function<TObject, const etl::exception&> {
    member_function(TObject& object_,
                    void (TObject::*p_function_)(const etl::exception&))
        : etl::function<TObject, const etl::exception&>(object_, p_function_) {}
  };

  static void set_callback(ifunction<const etl::exception&>& f) {
    create((void*)(&f), ifunction_stub);
  }

  template <void (*Method)(const etl::exception&)>
  static void set_callback() {
    create(nullptr, function_stub<Method>);
  }

  template <typename T, void (T::*Method)(const etl::exception&)>
  static void set_callback(T& instance) {
    create((void*)(&instance), method_stub<T, Method>);
  }

  template <typename T, void (T::*Method)(const etl::exception&) const>
  static void set_callback(const T& instance) {
    create((void*)(&instance), const_method_stub<T, Method>);
  }

  template <typename T, T& Instance, void (T::*Method)(const etl::exception&)>
  static void set_callback() {
    create(method_instance_stub<T, Instance, Method>);
  }

  template <typename T,
            T const& Instance,
            void (T::*Method)(const etl::exception&) const>
  static void set_callback() {
    create(const_method_instance_stub<T, Instance, Method>);
  }

  static void error(const etl::exception& e) {
    invocation_element& invocation = get_invocation_element();

    if (invocation.stub != nullptr) {
      (*invocation.stub)(invocation.object, e);
    }
  }

 private:
  typedef void (*stub_type)(void* object, const etl::exception&);

  struct invocation_element {
    invocation_element() : object(nullptr), stub(nullptr) {}

    void* object;
    stub_type stub;
  };

  static invocation_element& get_invocation_element() {
    static invocation_element invocation;

    return invocation;
  }

  static void create(void* object, stub_type stub) {
    invocation_element& invocation = get_invocation_element();

    invocation.object = object;
    invocation.stub = stub;
  }

  static void create(stub_type stub) {
    invocation_element& invocation = get_invocation_element();

    invocation.object = nullptr;
    invocation.stub = stub;
  }

  template <typename T, void (T::*Method)(const etl::exception&)>
  static void method_stub(void* object, const etl::exception& e) {
    T* p = static_cast<T*>(object);
    return (p->*Method)(e);
  }

  template <typename T, void (T::*Method)(const etl::exception&) const>
  static void const_method_stub(void* object, const etl::exception& e) {
    T* const p = static_cast<T*>(object);
    return (p->*Method)(e);
  }

  template <typename T, T& Instance, void (T::*Method)(const etl::exception&)>
  static void method_instance_stub(void*, const etl::exception& e) {
    return (Instance.*Method)(e);
  }

  template <typename T,
            const T& Instance,
            void (T::*Method)(const etl::exception&) const>
  static void const_method_instance_stub(void*, const etl::exception& e) {
    (Instance.*Method)(e);
  }

  template <void (*Method)(const etl::exception&)>
  static void function_stub(void*, const etl::exception& e) {
    (Method)(e);
  }

  static void ifunction_stub(void* object, const etl::exception& e) {
    etl::ifunction<const etl::exception&>* p =
        static_cast<etl::ifunction<const etl::exception&>*>(object);
    p->operator()(e);
  }
};
}  // namespace etl

namespace etl {

template <size_t VBlock_Size, size_t VAlignment, size_t VSize>
class fixed_sized_memory_block_allocator : public imemory_block_allocator {
 public:
  fixed_sized_memory_block_allocator() {}
};
}  // namespace etl

namespace etl {

typedef uint_least8_t message_id_t;

class imessage {
 public:
  virtual ~imessage() {}
};

template <etl::message_id_t ID_, typename TParent = etl::imessage>
class message : public TParent {
 public:
  enum { ID = ID_ };
};
}  // namespace etl

namespace etl {
class ireference_counted_message;

class ireference_counted_message_pool {
 public:
  virtual ~ireference_counted_message_pool() {}
};
}  // namespace etl

namespace etl {

typedef std::atomic<int> atomic_int;
typedef std::atomic<int32_t> atomic_int32_t;

}  // namespace etl

namespace etl {

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
class reference_counted_object : public etl::ireference_counted_object {
 public:
  typedef TObject value_type;
  typedef TCounter counter_type;

  reference_counted_object(const TObject& object_) : object(object_) {}

 private:
  reference_counted_object(const reference_counted_object&) = delete;
  reference_counted_object& operator=(const reference_counted_object&) = delete;

  TObject object;
  etl::reference_counter<TCounter> reference_counter;
};

}  // namespace etl

namespace etl {

class ireference_counted_message {
 public:
  virtual ~ireference_counted_message() {}
};

template <typename TMessage, typename TCounter>
class reference_counted_message : public etl::ireference_counted_message {
 public:
  typedef TMessage message_type;
  typedef TCounter counter_type;

  reference_counted_message(const TMessage& msg_,
                            etl::ireference_counted_message_pool& owner_)
      : rc_object(msg_), owner(owner_) {}

 private:
  etl::reference_counted_object<TMessage, TCounter> rc_object;
  etl::ireference_counted_message_pool& owner;
};

template <typename TMessage>
using atomic_counted_message =
    etl::reference_counted_message<TMessage, etl::atomic_int32_t>;

}  // namespace etl

namespace etl {

template <typename TCounter>
class reference_counted_message_pool
    : public etl::ireference_counted_message_pool {
 public:
  reference_counted_message_pool(
      etl::imemory_block_allocator& memory_block_allocator_)
      : memory_block_allocator(memory_block_allocator_) {}

  template <typename TMessage1, typename... TMessages>
  struct pool_message_parameters {
   private:
    static constexpr size_t size1 =
        sizeof(etl::reference_counted_message<TMessage1, TCounter>);

    static constexpr size_t size2 =
        pool_message_parameters<TMessages...>::max_size;

    static constexpr size_t alignment1 = std::alignment_of<
        etl::reference_counted_message<TMessage1, TCounter>>::value;

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
        sizeof(etl::reference_counted_message<TMessage1, TCounter>);

    static constexpr size_t max_alignment = std::alignment_of<
        etl::reference_counted_message<TMessage1, TCounter>>::value;
  };

 private:
  imemory_block_allocator& memory_block_allocator;

  reference_counted_message_pool(const reference_counted_message_pool&) =
      delete;
  reference_counted_message_pool& operator=(
      const reference_counted_message_pool&) = delete;
};

using atomic_counted_message_pool =
    reference_counted_message_pool<etl::atomic_int>;

}  // namespace etl

constexpr etl::message_id_t MessageId1 = 1U;
constexpr etl::message_id_t MessageId2 = 2U;

struct Message1 : public etl::message<MessageId1> {
  Message1(int i_) : i(i_) {}

  Message1(const Message1& oth) { i = oth.i; }

  ~Message1() override {}

  int i;
};

struct Message2 : public etl::message<MessageId2> {
  ~Message2() override {}
};

int main() {
  using pool_message_parameters =
      etl::atomic_counted_message_pool::pool_message_parameters<Message1,
                                                                Message2>;

  using pool_message_parameters =
      etl::atomic_counted_message_pool::pool_message_parameters<Message1,
                                                                Message2>;

  etl::fixed_sized_memory_block_allocator<
      pool_message_parameters::max_size, pool_message_parameters::max_alignment,
      4U>
      memory_allocator;

  etl::atomic_counted_message_pool message_pool(memory_allocator);

  Message1 message1(6);
  etl::reference_counted_message<Message1, etl::atomic_int> temp(message1,
                                                                 message_pool);
  const etl::ireference_counted_message& rcmessage = temp;
  rcmessage.~ireference_counted_message();
}

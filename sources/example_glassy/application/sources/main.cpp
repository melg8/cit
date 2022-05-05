struct imessage {
  virtual ~imessage() {}
};

struct message : imessage {
  virtual ~message() override {}
};

struct Message1 : message {
  Message1() {}
  ~Message1() override {}
  Message1(const Message1&) {}
};

struct ireference_counter {
  virtual ~ireference_counter() {}
};

struct reference_counter : ireference_counter {
  virtual ~reference_counter() override {}
};

class ireference_counted_object {
 public:
  virtual ~ireference_counted_object() {}
};

class reference_counted_object : public ireference_counted_object {
 public:
  reference_counted_object(const Message1& object_) : object(object_) {}

 private:
  reference_counted_object(const reference_counted_object&) = delete;
  reference_counted_object& operator=(const reference_counted_object&) = delete;

  Message1 object;
  reference_counter reference_counter_;
};

class ireference_counted_message {
 public:
  virtual ~ireference_counted_message() {}
};

struct reference_counted_message : ireference_counted_message {
  reference_counted_message(const Message1& msg_) : rc_object(msg_) {}

 private:
  reference_counted_object rc_object;
};

int main() {
  Message1 message1;
  reference_counted_message temp(message1);
  const ireference_counted_message& rcmessage = temp;
  rcmessage.~ireference_counted_message();
}

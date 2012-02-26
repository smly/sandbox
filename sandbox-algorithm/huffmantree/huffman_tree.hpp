#ifndef CRYPT_HUFFMAN_H
#define CRYPT_HUFFMAN_H

namespace Crypt
{
  class Message;

  typedef std::vector<Message*> Messages;

  class Message {
  public:
      double prob;
      std::string message;
      std::string code;
      Message() {}
      Message(std::string msg, double p)
          : message(msg), code(""), prob(p) {}
      ~Message() {}
  };

  class HuffmanTree {
  private:
  public:
      HuffmanTree* lt;
      HuffmanTree* rt;
      Message* msg;
      HuffmanTree()
          : lt(NULL), rt(NULL), msg(NULL) {}
      HuffmanTree(HuffmanTree* l, HuffmanTree* r)
          : lt(l), rt(r) {
          msg = new Message();
          msg->prob = l->msg->prob + r->msg->prob;
      }
      HuffmanTree(Message* msg)
          : lt(NULL), rt(NULL), msg(msg) {}
      ~HuffmanTree() { clear(); }
      void clear();
      Messages retrieve();
  };

  class HuffmanTreeComparison {
  public:
      bool operator () (const HuffmanTree* lhs, const HuffmanTree* rhs) const {
          return lhs->msg->prob > rhs->msg->prob;
      }
  };

  void view_table(Messages);
  void view_2blocked_table(Messages);
  void view_3blocked_table(Messages);

}

#endif


#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include "huffman_tree.hpp"

namespace Crypt
{
  typedef Messages::iterator It;

  void view_3blocked_table(Messages msgs) {
      Messages n_msgs;
      for (It i=msgs.begin(); i != msgs.end(); i++) {
          for (It j=msgs.begin(); j != msgs.end(); j++) {
              for (It k=msgs.begin(); k != msgs.end(); k++) {
                  Message *msg = new Message((*i)->message +
                                             (*j)->message +
                                             (*k)->message,
                                             (*i)->prob *
                                             (*j)->prob *
                                             (*k)->prob);
                  n_msgs.push_back(msg);
              }
          }
      }
      view_table(n_msgs);
      for (It i = n_msgs.begin(); i != n_msgs.end(); i++) {
          delete *i;
      }
  }

  void view_2blocked_table(Messages msgs) {
      Messages n_msgs;
      for (It i=msgs.begin(); i != msgs.end(); i++) {
          for (It j=msgs.begin(); j != msgs.end(); j++) {
              Message *msg = new Message((*i)->message +
                                         (*j)->message,
                                         (*i)->prob *
                                         (*j)->prob);
              n_msgs.push_back(msg);
          }
      }
      view_table(n_msgs);
      for (It i = n_msgs.begin(); i != n_msgs.end(); i++) {
          delete *i;
      }
  }

  void view_table(Messages msgs) {
      std::priority_queue<HuffmanTree*,
          std::vector<HuffmanTree*>,
          HuffmanTreeComparison> p;
      for (It i = msgs.begin(); i != msgs.end(); i++) {
          HuffmanTree* tree = new HuffmanTree(*i);
          p.push(tree);
      }
      while ( p.size() > 1 ) {
          HuffmanTree* lt = p.top(); p.pop();
          HuffmanTree* rt = p.top(); p.pop();
          HuffmanTree* mt = new HuffmanTree(lt, rt);
          p.push(mt);
      }
      HuffmanTree* tree =p.top(); p.pop();
      Messages code = tree->retrieve();
      std::cout << "-----------------------" << std::endl;
      double ave = 0;
      if (code.size() > 10) {
          std::cout << "..." << std::endl;
          for (It i = code.begin(); i != code.end(); i++) {
              ave += (double)(*i)->code.size() /
                  (double)(*i)->message.size() * (*i)->prob;
          }
      } else {
          for (It i = code.begin(); i != code.end(); i++) {
              std::cout << (*i)->message << "\t"
                        << (*i)->prob << "\t"
                        << (*i)->code << std::endl;
              ave += (double)(*i)->code.size() /
                  (double)(*i)->message.size() * (*i)->prob;
          }
      }
      std::cout << "[Ave] " << ave << std::endl;
      delete tree;
  }

  void HuffmanTree::clear() {
      if (lt != NULL) delete msg;
      delete lt;
      delete rt;
  }

  Messages HuffmanTree::retrieve() {
      std::stack<HuffmanTree*> st;
      Messages msgs;
      rt->msg->code = "1"; st.push(rt);
      lt->msg->code = "0"; st.push(lt);
      while ( !st.empty() ) {
          HuffmanTree* t = st.top(); st.pop();
          if (t->rt != NULL) {
              t->rt->msg->code += t->msg->code;
              t->rt->msg->code += "1";
              st.push(t->rt);
              t->lt->msg->code += t->msg->code;
              t->lt->msg->code += "0";
              st.push(t->lt);
          }
          if (t->rt == NULL) {
              msgs.push_back(t->msg);
          }
      }
      return msgs;
  }
}

/* 2008-10-06 whitespace interpreter implemented in C++ */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <bitset>
#include <unistd.h>
using namespace std;

typedef enum OptionFlag
{
  MODE_HELP,
  OPTION_GENOME,
  OPTION_EMIT,
  OPTION_LABELS,
  OPTION_STACKPRINT,
  OptionFlagNum
} OptionFlag;

/* whitespace instructions
 * A means [Space]
 * B means [Tab]
 * C means [LF]
 * P means parametes
 */
typedef enum WSCODE
{
  AAP, ACA, ACB, ACC,
  BAAA, BAAB, BAAC, BABA, BABB,
  BBA, BBB,
  CAAP, CABP, CACP, CBAP, CBBP, CBC, CCC,
  BCAA, BCAB, BCBA, BCBB
} ESCODE;

/* assembly like pseudo code */
typedef enum PCODE
{
  PUSH, DUPL, SWAP, POPL,
  ADDB, SUBB, MULB, DIVB, MODB,
  STOR, RETR,
  SETL, CALL, JUMP, JMPZ, JMPN, RETC, ENDC,
  PCHR, PNUM, RCHR, RNUM
} PCODE;

/* Whitespace Instruction Modification Parameter (IMP)
 * STACK_MANIPULATION : A
 * ARITHMETIC         : BA
 * HEAP_ACCESS        : BB
 * FLOW_CONTROL       : C
 * INPUT_OUTPUT       : BC
 */
typedef enum IMP
{ STACK_MANIPULATION,
  ARITHMETIC,
  HEAP_ACCESS,
  FLOW_CONTROL,
  INPUT_OUTPUT
} IMP;

class Token
{
  int p;        // position
  WSCODE id;    // instruction id
  string param; // parameter P

public:
  Token(int p_, WSCODE id_, string param_)
  { p = p_; id = id_; param = param_; }

  WSCODE get_id()    { return id; }
  int    get_pos()   { return p; }
  string get_param() { return param; }
};

typedef vector<Token> VT;

void usage() {
  cout << "usage: ./wsi -[hegl] <filename>" << endl;
  cout << " -h\tshow this help message" << endl;
  cout << " -e\temit specified file (put assembry like pseudo code)" << endl;
  //  cout << " -c\tcompile specified source file using gcc compiler" << endl;
  cout << " -g\tsyntax check and show pcode as genome sequence" << endl;
  cout << " -l\tshow labels" << endl;
  //  cout << " -p\tshow stack per instruction" << endl;
  exit(1);
}

int numerise(string value) {
  char c = 0;
  int s = value.size(), sign = 1;
  if (value[0] == 'B') sign = -1;

  for (int i=0; i<s; i++)
    c += (value[ s - i ] - 'A') << (i - 1);

  return sign * (int)c;
}

string fetch_param(string &p, int &pos) {
  string param;

  while (p[0] != 'C' && pos++) {
    param += p[0];
    p = p.substr(1);
  }

  p = p.substr(1);
  pos++;

  return param;
}

IMP imp_detection(string &p) {
  if (p[0] == 'A') return STACK_MANIPULATION;
  else if (p[0] == 'C') return FLOW_CONTROL;

  string imp = p.substr(0,2);
  if (imp == "BA") return ARITHMETIC;
  else if (imp == "BB") return HEAP_ACCESS;
  else if (imp == "BC") return INPUT_OUTPUT;

  /* imp detection err. some exception here */
  cout << "[wsi] imp detection err" << endl;
  exit(1);
}

void forwarding(string &p, int &pos, int i) {
  p = p.substr(i); pos+=i;
}

Token lexer(string &p, int &pos) {
  string param, cmd;
  int now_pos = pos;

  switch ( imp_detection(p) ) {
  case STACK_MANIPULATION:
    cmd = p.substr(0,2);
    forwarding(p, pos, 2);
    if (cmd == "AA") return Token(now_pos, AAP, fetch_param(p, pos));
    if (cmd == "AC") {

      char flag = p[0];
      forwarding(p, pos, 1);
      switch(flag) {
      case 'A': return Token(now_pos, ACA, param);
      case 'B': return Token(now_pos, ACB, param);
      case 'C': return Token(now_pos, ACC, param);
      }
    }

    break;

  case ARITHMETIC:
    cmd = p.substr(0,4);
    forwarding(p, pos, 4);
    if (cmd == "BAAA") return Token(now_pos, BAAA, param);
    if (cmd == "BAAB") return Token(now_pos, BAAB, param);
    if (cmd == "BAAC") return Token(now_pos, BAAC, param);
    if (cmd == "BABA") return Token(now_pos, BABA, param);
    if (cmd == "BABB") return Token(now_pos, BABB, param);

    break;

  case HEAP_ACCESS:
    cmd = p.substr(0,3);
    forwarding(p, pos, 3);
    if (cmd == "BBA") return Token(now_pos, BBA, param);
    if (cmd == "BBB") return Token(now_pos, BBB, param);

    break;

  case FLOW_CONTROL:
    cmd = p.substr(0,3);
    forwarding(p, pos, 3);
    if (cmd == "CAA") return Token(now_pos, CAAP, fetch_param(p, pos));
    if (cmd == "CAB") return Token(now_pos, CABP, fetch_param(p, pos));
    if (cmd == "CAC") return Token(now_pos, CACP, fetch_param(p, pos));
    if (cmd == "CBA") return Token(now_pos, CBAP, fetch_param(p, pos));
    if (cmd == "CBB") return Token(now_pos, CBBP, fetch_param(p, pos));
    if (cmd == "CBC") return Token(now_pos, CBC, param);
    if (cmd == "CCC") return Token(now_pos, CCC, param);

    break;

  case INPUT_OUTPUT:
    cmd = p.substr(0,4);
    forwarding(p, pos, 4);
    if (cmd=="BCAA") return Token(now_pos, BCAA, param);
    if (cmd=="BCAB") return Token(now_pos, BCAB, param);
    if (cmd=="BCBA") return Token(now_pos, BCBA, param);
    if (cmd=="BCBB") return Token(now_pos, BCBB, param);

    break;
  }

  /* lexer err. some exception here */
  cout << "[wsi] lexer err" << endl;
  exit(1);
}

/* debug */
void print_token (VT::iterator it, stack<int> st, stack<int> pst, map<int,int> &heap) {
  cout << setw(4) << it->get_pos() << " / stacksize=" << st.size() << " [";
  while(!st.empty()) {
    int tmp = st.top(); st.pop();
    cout << tmp;
    if (!st.empty()) cout << ",";
  }
  cout << "]" << endl;
}

/* interprete */
void interprete (VT tokens, map<string, int> labels) {
  stack<int> st, pst;
  map<int,int> heap;

  VT::iterator tok;
  for (tok=tokens.begin(); tok != tokens.end(); tok++) {
    // print_token(tok, st, pst, heap);
    int ppos = tok->get_pos();

    int left, right, tmp, jumpto;
    map<int, int>::iterator mit;
    VT::iterator t;
    switch(tok->get_id()) {
    case PUSH: st.push( numerise( tok->get_param() ) ); break;
    case DUPL: st.push( st.top() ); break;
    case SWAP:
      left  = st.top(); st.pop();
      right = st.top(); st.pop();
      st.push(left); st.push(right); break;
    case POPL: st.pop(); break;
    case ADDB:
      right = st.top(); st.pop();
      left = st.top(); st.pop();
      st.push(left + right); break;
    case SUBB:
      right = st.top(); st.pop();
      left = st.top(); st.pop();
      st.push(left - right); break;
    case MULB:
      right = st.top(); st.pop();
      left = st.top(); st.pop();
      st.push(left * right); break;
    case DIVB:
      right = st.top(); st.pop();
      left = st.top(); st.pop();
      st.push((int)(left / right)); break;
    case MODB:
      right = st.top(); st.pop();
      left = st.top(); st.pop();
      st.push(left % right); break;
    case STOR:
      left = st.top(); st.pop();
       right = st.top(); st.pop();
      heap[ right ] = left; break;
    case RETR:
      tmp = st.top(); st.pop();
      mit = heap.find( tmp );
      if (mit != heap.end()) st.push( mit->second );
      else st.push( 0 ); break;
    case SETL: break;
    case CALL:
      pst.push( ppos );
      jumpto = labels[ tok->get_param() ];

      for (t=tokens.begin(); t != tokens.end(); t++) {
        if (t->get_pos() == jumpto) { break; }
      }
      tok = t;
      break;
    case JUMP:
      jumpto = labels[ tok->get_param() ];

      for (t=tokens.begin(); t != tokens.end(); t++) {
        if (t->get_pos() == jumpto) { break; }
      }
      tok = t;
      break;
    case JMPZ:
      tmp = st.top(); st.pop();
      jumpto = labels[ tok->get_param() ];

      if (tmp==0) {

        for (t=tokens.begin(); t != tokens.end(); t++) {
          if (t->get_pos() == jumpto) { break; }
        }
        tok = t;
      }
      break;
    case JMPN:
      tmp = st.top(); st.pop();
      jumpto = labels[ tok->get_param() ];

      if (tmp < 0) {

        for (t=tokens.begin(); t != tokens.end(); t++) {
          if (t->get_pos() == jumpto) { break; }
        }
        tok = t;
      }
      break;
    case RETC:
      ppos = pst.top(); pst.pop();

      for (t=tokens.begin(); t != tokens.end(); t++) {
        if (t->get_pos() == ppos) { break; }
      }
      tok = t;
      break;
    case ENDC: exit(0); break;
    case PCHR:
      tmp = st.top(); st.pop();
      cout << (char)tmp; break;
    case PNUM:
      tmp = st.top(); st.pop();
      cout << tmp; break;
    case RCHR:
      tmp = (int)getchar();
      heap[ st.top() ] = tmp;
      st.pop(); break;
    case RNUM:
      cin >> tmp;
      heap[ st.top() ] = tmp;
      st.pop(); break;
    }

    /* parse err. some exception here */
  }
}

/* show labels */
void debug_labels (VT tokens, map<string,int> labels) {
  cout << endl << "  <===--- AVAILABLE LABELS ---===>" << endl << endl;

  VT::iterator tok;
  for (tok = tokens.begin(); tok != tokens.end(); tok++) {
    if ( tok->get_id() == CAAP ) {
      labels[ tok->get_param() ] = tok->get_pos();
      cout << "L" << tok->get_pos()
           << " = " << tok->get_param() << endl;
    }
  }
}

/**/
void debug_print(VT tokens, map<string,int> marks) {
  cout << endl << "  <===--- ASSEMBLY LIKE PSEUDO CODE ---===>" << endl << endl;

  VT::iterator tok;
  for (tok=tokens.begin(); tok != tokens.end(); tok++) {
    int pos = tok->get_pos();
    string param = tok->get_param();

    cout << setw(4) << setfill('0') << pos << ": ";

    switch (tok->get_id()) {
    case AAP:  cout << "AAP\tpush\t" << param << "(" << numerise(param) << ")" << endl; break;
    case ACA:  cout << "ACA\t" << "dul\t" << endl; break;
    case ACB:  cout << "ACB\t" << endl; break;
    case ACC:  cout << "ACC\t" << "pop\t" << endl; break;
    case BBA:  cout << "BBA\t" << "store" << endl; break;
    case BBB:  cout << "BBB\t" << "retri" << endl; break;
    case BAAA: cout << "BAAA\t" << "add" << endl; break;
    case BAAB: cout << "BAAB\t" << "sub" << endl; break;
    case BAAC: cout << "BAAC\t" << endl; break;
    case BABA: cout << "BABA\t" << endl; break;
    case BABB: cout << "BABB\t" << endl; break;
    case CAAP: cout << "CAAP\t" << "label\tL" << marks[param] << endl; break;
    case CABP: cout << "CABP\t" << "call\tL" << marks[param] << endl; break;
    case CACP: cout << "CACP\t" << "jump\tL" << marks[param] << endl; break;
    case CBAP: cout << "CBAP\t" << "jmpz\tL" << marks[param] << endl; break;
    case CBBP: cout << "CBBP\t" << param << endl; break;
    case CBC:  cout << "CBC\t" << "ret" << endl; break;
    case CCC:  cout << "CCC\t" << "exit" << endl; break;
    case BCAA: cout << "BCAA" << "\t" << "puchr" << endl; break;
    case BCAB: cout << "BCAB\t" << endl; break;
    case BCBA: cout << "BCBA\t" << "rechr" << endl; break;
    case BCBB: cout << "BCBB\t" << endl; break;
    }
  }
}

/* debug print as genome sequence */
void debug_print_as_genome(VT tokens) {
  cout << endl << "  <===--- DEBUG PRINT AS GENOME SEQUENCE ---===>" << endl << endl;

  int linemanip = 0;
  VT::iterator tok;

  for (tok=tokens.begin(); tok != tokens.end(); tok++) {
    int id = tok->get_id();
    cout << (char)(id + 'a');
    linemanip++;
    if (linemanip%40==0) cout << endl;
  }

  cout << endl;
}

void test() {
  map<string, int> labels;
  VT tokens;
  int pos = 0;

  /* test program */
  string prog  =
         string("AAACAAABAABAAACBBAAAABCAAABBAABABCBBAAAABACAAA")
    + "BBABBAACBBAAAABBCAAABBABBAACBBAAAABAACAAABBABBBBCBBAAAAB"
    + "ABCAAABABBAACBBAAAABBACAAABAAAAACBBAAAABBBCAAABBBABBBCBB"
    + "AAAABAAACAAABBABBBBCBBAAAABAABCAAABBBAABACBBAAAABABACAAA"
    + "BBABBAACBBAAAABABBCAAABBAABAACBBAAAABBAACAAABAAAAACBBAAA"
    + "ABBABCAAABBABBBBCBBAAAABBBACAAABBAABBACBBAAAABBBBCAAABAA"
    + "AAACBBAAAABAAAACAAABBBAABBCBBAAAABAAABCAAABBBAAAACBBAAAA"
    + "BAABACAAABBAAAABCBBAAAABAABBCAAABBAAABBCBBAAAABABAACAAAB"
    + "BAABABCBBAAAABABABCAAABBBAABBCBBAAAABABBACAAABAAAABCBBAA"
    + "AABABBBCAAAACBBAAAAACCABABBBABBBABBBAABAABBABAABABBBABAA"
    + "ABBAABABCCABABBABBBAABBAABABABBBABBBABBABBAAABBABAABABBA"
    + "BBBAABBAABABCCCCCAAABBAAAABABBAABAAABBAABAACBAAACBCCAAAB"
    + "BBABBBABBBAABAABBABAABABBBABAAABBAABABCACABBBACACBAABBBA"
    + "BBBABBBAABAABBABAABABBBABAAABBAABABABABBBBBABBAABABABBAB"
    + "BBAABBAABAACBCAAAAABCBAAACACABBBABBBABBBAABAABBABAABABBB"
    + "ABAAABBAABABCCAAABBBABBBABBBAABAABBABAABABBBABAAABBAABAB"
    + "ABABBBBBABBAABABABBABBBAABBAABAACACCACCCBCCAAABBBAABAABB"
    + "AABABABBAAAABABBAABAACACAACABCBABBBACAAAABABACBAABCBAABB"
    + "BAABAABBAABABABBAAAABABBAABAAABABBBBBABBAABABABBABBBAABB"
    + "AABAACACCAAABCBAAACACABBBAABAABBAABABABBAAAABABBAABAACCA"
    + "AABBBAABAABBAABABABBAAAABABBAABAAABABBBBBABBAABABABBABBB"
    + "AABBAABAACACCAAABCBAAAAAAACBBACBCCAAABBABBBAABBAABABABBB"
    + "ABBBABBABBAAABBABAABABBABBBAABBAABABCAAABABACAAABBABCBCA"
    + "ABCAACBC";

  while ( !prog.empty() ) {
    Token tok = lexer(prog, pos);
    tokens.push_back(tok);

    if ( tok.get_id() == CAAP )
      labels[ tok.get_param() ] = tok.get_pos();
  }

  usage();

  debug_labels(tokens, labels);
  debug_print(tokens, labels);
  debug_print_as_genome(tokens);

  cout << endl << "  <===--- RUN PROGRAM ---===>" << endl << endl;
  interprete(tokens, labels);
}

/*
 *
 */
int main(int argc, char ** argv) {
  bitset<OptionFlagNum> mode;
  string prog;

  int opt, diff = 0;
  while (( opt = getopt(argc, argv, "hleg")) != -1) {
    switch (opt) {
    case 'h': mode.set( MODE_HELP ); break;
    case 'e': mode.set( OPTION_EMIT ); break;
    case 'l': mode.set( OPTION_LABELS ); break;
    case 'g': mode.set( OPTION_GENOME ); break;
    }
  }

  if ( mode[MODE_HELP] ) { usage(); exit(0); }

  char *name;
  for (; optind<argc; optind++)
    if (argv[optind])
      name = argv[optind];

  ifstream ifs( name );

  if (!ifs)
    if (!mode[OptionFlagNum]) { test(); exit(1); }
    else { usage(); exit(1); }

  char c;
  while (ifs.get(c)) {
    switch(c) {
    case ' ' : prog+='A'; break;
    case '\t': prog+='B'; break;
    case '\n': prog+='C'; break;
    }
  }

  map<string, int> labels;
  VT tokens;
  int pos = 0;

  while ( !prog.empty() ) {
    Token tok = lexer(prog, pos);
    tokens.push_back(tok);

    if ( tok.get_id() == CAAP )
      labels[ tok.get_param() ] = tok.get_pos();
  }

  if ( mode[OPTION_LABELS] ) debug_labels(tokens, labels);
  if ( mode[OPTION_EMIT] )   debug_print(tokens, labels);
  if ( mode[OPTION_GENOME] ) debug_print_as_genome(tokens);

  if ( mode[OPTION_LABELS] | mode[OPTION_EMIT] | mode[OPTION_GENOME] )
    cout << endl << "  <===--- RUN PROGRAM ---===>" << endl << endl;

  interprete(tokens, labels);
}

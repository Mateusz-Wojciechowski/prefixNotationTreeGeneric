#ifndef LAB4_TEP_CTREE_H
#define LAB4_TEP_CTREE_H
#include "CNode.h"
#include "COperation.h"
#include "CConstantsTree.h"
#include "string"
#include "map"
#include "set"
#include "iostream"
#include "vector"
#include "cstdlib"
#include "cctype"
#include <sstream>
using namespace std;

using namespace std;
template <typename T>
class CTree {
public:
    CTree(string s_prefix);
    CTree();
    ~CTree();
    CNode<T>* vCreateTree(string s_prefix, int &i_index);
    bool bIsOperator(string s_expression);
    CNode<T>* getRoot();
    void vPrintNode(CNode<T>* c_node, int i_depth);
    void vPrintTree();
    T tCalculateTreeValue(CNode<T>* c_node, const map<string, T>& values=map<string, T>());
    int getOperandsNeeded(const string op);
    void vCreateMap();
    string sTreeToStr(CNode<T>* c_node);
    set<string> vGetUniqueVariables(CNode<T>* c_node, set<string>& variables);
    CNode<T>* findLastLeaf(CNode<T>* c_node);
    void vJoinTrees(CNode<T>* c_new_root);
    CNode<T>* copyTree(CNode<T>* c_node);
    CTree operator+(const CTree& c_other);
    CTree& operator=(const CTree& c_other);
    CNode<T>* createDefaultNode();
    bool bIsVariable(string s_value);
    bool bIsNum(string s_value);
    bool getWasChanged();
    bool getWrongArguments();
    bool bIsTTypevalue(string s_value);
    void vCheckDivisionByZero(CNode<T>* c_node, const vector<T>& results);
    T getValueFromString(string s_value);

private:
    CNode<T>* root;
    map<string, COperation<T> > operations;
    bool b_was_changed;
    bool b_wrong_arguments;
};

using namespace std;

template<typename T>
CTree<T>::CTree(string s_prefix){
    b_was_changed = false;
    b_wrong_arguments = false;
    int i_index = 0;
    root = vCreateTree(s_prefix, i_index);
    vCreateMap();
}

template<typename T>
CTree<T>::CTree(){
    b_was_changed = false;
    b_wrong_arguments = false;
    int i_index = 0;
    root = vCreateTree(S_SPACE, i_index);
    vCreateMap();
}

template<typename T>
CTree<T>::~CTree(){
    delete root;
}

template<typename T>
CNode<T>* CTree<T>::vCreateTree(string s_prefix, int &i_index){
    while (i_index < s_prefix.size() && s_prefix[i_index] == C_SPACE){
        i_index++;
    }

    if (i_index >= s_prefix.size()){
        return NULL;
    }

    int i_start_index = i_index;
    while (i_index < s_prefix.size() && s_prefix[i_index] != C_SPACE){
        i_index++;
    }

    string s_expression = s_prefix.substr(i_start_index, i_index - i_start_index);

    if (!bIsOperator(s_expression) && !bIsVariable(s_expression) && !bIsTTypevalue(s_expression)) {
        cout << S_INVALID_VALUE_COMM << s_expression << endl;
        b_was_changed = true;
        return vCreateTree(s_prefix, i_index);
    }

    CNode<T>* c_node = new CNode<T>(s_expression);

    if(bIsTTypevalue(s_expression)){
        c_node->setTValue(getValueFromString(s_expression));
    }

    if (bIsOperator(s_expression)) {
        int operandsNeeded = getOperandsNeeded(s_expression);

        for (int i = 0; i < operandsNeeded; i++) {
            CNode<T>* c_child = NULL;

            while (i_index < s_prefix.size() && s_prefix[i_index] == C_SPACE){
                i_index++;
            }

            if (i_index < s_prefix.size()) {
                c_child = vCreateTree(s_prefix, i_index);
            }

            if (c_child == NULL) {
                c_child = createDefaultNode();
                b_was_changed = true;
            }

            c_node->vAdd(c_child);
        }
    }

    return c_node;
}

template<typename T>
int CTree<T>::getOperandsNeeded(const string op) {
    if(op==S_PLUS || op==S_SUB || op==S_MUL || op==S_DIV){
        return I_BINARY_OP_ARGS;
    }

    if(op==S_SIN || op==S_COS){
        return I_SINGLE_OP_ARGS;
    }

    else return I_OP_NOT_FOUND;
}

template<typename T>
void CTree<T>::vPrintTree() {
    vPrintNode(root, 0);
}

template<typename T>
void CTree<T>::vPrintNode(CNode<T>* node, int i_depth) {
    if (node==NULL) {
        return;
    }

    string indent = string(i_depth * I_PRINTING_OFFSET, C_SPACE);
    cout << indent << node->getValue() << endl;

    for (int i=0; i<node->getChildren().size(); i++) {
        vPrintNode(node->getChildren()[i], i_depth + 1);
    }
}

template<typename T>
T CTree<T>::tCalculateTreeValue(CNode<T> *c_node, const map<string, T> &variables){
    if (c_node == NULL) {
        return 0;
    }

    if (c_node->getChildren().empty()){
        if (bIsVariable(c_node->getValue())){
            typedef typename map<string, T>::const_iterator VarItType;
            VarItType varIt;
            varIt = variables.find(c_node->getValue());
            if (varIt != variables.end()){
                return varIt->second;
            }
        }
        return c_node->getTValue();
    }

    vector<T> results;

    const vector<CNode<T>*>& children = c_node->getChildren();
    for (typename vector<CNode<T>*>::const_iterator it = children.begin(); it != children.end(); ++it) {
        results.push_back(tCalculateTreeValue(*it, variables));
    }

    vCheckDivisionByZero(c_node, results);

    typedef typename map<string, COperation<T> >::const_iterator OP_IT;
    OP_IT op_it;
    op_it = operations.find(c_node->getValue());
    return op_it->second.execute(results);
}

template<typename T>
void CTree<T>::vCreateMap(){
    operations.insert(make_pair(string(S_PLUS), COperation<T>(COperation<T>::t_add)));
    operations.insert(make_pair(string(S_SUB), COperation<T>(COperation<T>::t_sub)));
    operations.insert(make_pair(string(S_MUL), COperation<T>(COperation<T>::t_mul)));
    operations.insert(make_pair(string(S_DIV), COperation<T>(COperation<T>::t_div)));
    operations.insert(make_pair(string(S_SIN), COperation<T>(COperation<T>::t_sin)));
    operations.insert(make_pair(string(S_COS), COperation<T>(COperation<T>::t_cos)));
}

template<typename T>
set<string> CTree<T>::vGetUniqueVariables(CNode<T> *c_node, set<string>& variables){
    if(c_node==NULL){
        return variables;
    }

    string s_value = c_node->getValue();
    if(bIsVariable(c_node->getValue())){
        variables.insert(s_value);
    }

    vector<CNode<T>*> children = c_node->getChildren();

    for(int i=0; i<children.size(); i++){
        vGetUniqueVariables(children[i], variables);
    }

    return variables;
}

template<typename T>
string CTree<T>::sTreeToStr(CNode<T> *c_node){
    b_was_changed = false;
    if(c_node==NULL){
        return S_EMPTY_STRING;
    }

    string result = c_node->getValue() + S_SPACE;

    const vector<CNode<T>*>& children = c_node->getChildren();
    for(int i=0; i<children.size(); i++){
        result += sTreeToStr(children[i]);
    }
    return result;
}

template<typename T>
CNode<T>* CTree<T>::findLastLeaf(CNode<T> *c_node){
    if(c_node==NULL){
        return NULL;
    }

    if(c_node->getChildren().empty()){
        return c_node;
    }

    vector<CNode<T>*> children = c_node->getChildren();
    return findLastLeaf(children.back());
}

template<typename T>
void CTree<T>::vJoinTrees(CNode<T>* c_new_root){
    CNode<T>* leaf = findLastLeaf(root);
    if(leaf!=NULL){
        leaf->vReplaceNode(c_new_root);
    }
}

template<typename T>
CNode<T>* CTree<T>::copyTree(CNode<T> *c_node){
    if(c_node==NULL){
        return NULL;
    }

    CNode<T>* c_new_node = new CNode<T>(c_node->getValue());
    c_new_node->setTValue(c_node->getTValue());
    const vector<CNode<T>*>& children = c_node->getChildren();

    for(int i=0; i<children.size(); i++){
        c_new_node->vAdd(copyTree(children[i]));
    }
    return c_new_node;
}

template<typename T>
CTree<T>& CTree<T>::operator=(const CTree<T> &c_other){
    if(this!=&c_other){
        root = copyTree(c_other.root);
        b_was_changed = c_other.b_was_changed;
        operations = c_other.operations;
        b_wrong_arguments = c_other.b_wrong_arguments;
    }

    return *this;
}

template<typename T>
CTree<T> CTree<T>::operator+(const CTree<T> &c_other){
    CTree<T> c_result;
    c_result.root = copyTree(this->root);

    CNode<T>* c_second_root = copyTree(c_other.root);
    c_result.vJoinTrees(c_second_root);

    return c_result;
}

template<typename T>
bool CTree<T>::bIsVariable(string s_value){
    if(!bIsOperator(s_value) && !bIsTTypevalue(s_value)){

        for(int i=0; i<s_value.size(); i++){
            if(isalpha(s_value[i])){
                return true;
            }
        }
    }

    return false;
}

template<typename T>
bool CTree<T>::bIsNum(string s_value){
    for (int i = 0; i < s_value.size(); i++){
        if (!isdigit(s_value[i])) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool CTree<T>::bIsOperator(string s_expression) {
    return s_expression==S_PLUS || s_expression==S_SUB || s_expression==S_MUL || s_expression==S_DIV || s_expression==S_SIN || s_expression==S_COS || s_expression==S_HASH;
}

template<typename T>
CNode<T>* CTree<T>::getRoot(){
    return root;
}

template<typename T>
bool CTree<T>::getWasChanged(){
    return b_was_changed;
}

template<typename T>
bool CTree<T>::getWrongArguments(){
    return b_wrong_arguments;
}

template <>
bool CTree<int>::bIsTTypevalue(string s_value){
    istringstream iss(s_value);
    int i_num;
    iss >> i_num;
    return iss.eof() && !iss.fail();
}

template <>
bool CTree<double>::bIsTTypevalue(string s_value){
    istringstream iss(s_value);
    double d_num;
    iss >> d_num;
    return iss.eof() && !iss.fail();
}

template <>
bool CTree<string>::bIsTTypevalue(string s_value){
    int i_first_quote = s_value.find('\"');
    if(i_first_quote==-1){
        return false;
    }

    int i_second_quote = s_value.find('\"', i_first_quote + 1);
    if(i_second_quote==-1){
        return false;
    }

    return true;
}

template <>
int CTree<int>::getValueFromString(string s_value){
    return atoi(s_value.c_str());
}

template <>
double CTree<double>::getValueFromString(string s_value){
    istringstream iss(s_value);
    double d_value;
    iss >> d_value;
    return d_value;
}

template <>
string CTree<string>::getValueFromString(string s_value){
    int i_first_quote = s_value.find('\"');
    int i_second_quote = s_value.find('\"', i_first_quote + 1);
    return s_value.substr(i_first_quote + 1, i_second_quote - i_first_quote - 1);
}

template<typename T>

void CTree<T>::vCheckDivisionByZero(CNode<T> *c_node, const vector<T> &results){
    if(c_node->getValue() == S_DIV && results[1] == 0){
        throw std::invalid_argument(S_DIVISION_BY_ZERO_COMM);
    }
}

template<>
void CTree<string>::vCheckDivisionByZero(CNode<string>* c_node, const vector<string>& results) {

}

template <>
CNode<int>* CTree<int>::createDefaultNode(){
    CNode<int>* c_node = new CNode<int>(S_DEFAULT_NODE_VALUE_NUMERIC);
    c_node->setTValue(getValueFromString(S_DEFAULT_NODE_VALUE_NUMERIC));
    return c_node;
}

template <>
CNode<double>* CTree<double>::createDefaultNode(){
    CNode<double>* c_node = new CNode<double>(S_DEFAULT_NODE_VALUE_DOUBLE);
    c_node->setTValue(getValueFromString(S_DEFAULT_NODE_VALUE_DOUBLE));
    return c_node;
}

template <>
CNode<string>* CTree<string>::createDefaultNode(){
    CNode<string>* c_node = new CNode<string>(S_DEFAULT_NODE_VALUE_STRING);
    c_node->setTValue(getValueFromString(S_DEFAULT_NODE_VALUE_STRING));
    return c_node;
}

#endif //LAB4_TEP_CTREE_H

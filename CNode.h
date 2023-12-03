#ifndef LAB4_TEP_CNODE_H
#define LAB4_TEP_CNODE_H
#include "vector"
#include "string"

using namespace std;

template <typename T>
class CNode {
public:
    CNode<T>(string s_Value);
    CNode<T>(string s_Value, CNode<T>* c_parent);
    ~CNode<T>();
    void vRemoveChild(CNode<T>* child);
    void vAdd(CNode<T>* child);
    CNode<T>* getParent() const;
    string getValue();
    vector<CNode<T>*> getChildren();
    void vReplaceNode(CNode<T>* c_new_node);
    void setSValue(string s_new_value);
    void setTValue(T t_new_value);
    T getTValue();
private:
    T t_value;
    string s_value;
    CNode<T>* c_parent;
    bool b_is_operator;
    vector<CNode<T>*> c_children;
};


/////////////////////////////////////////////////////////////

template<typename T>
CNode<T>::CNode(string s_Value) {
    c_parent = NULL;
    s_value = s_Value;
}

template<typename T>
CNode<T>::CNode(string s_Value, CNode<T>* parent){
    s_value = s_Value;
    c_parent = parent;
}

template<typename T>
CNode<T>::~CNode(){
    for(int i=0; i<c_children.size(); i++){
        delete c_children[i];
    }
}

template<typename T>
void CNode<T>::vRemoveChild(CNode<T> *child){
    for(int i=0; i<c_children.size(); i++){
        if(c_children[i]==child){
            c_children.erase(c_children.begin()+i);
        }
    }
}

template<typename T>
void CNode<T>::vAdd(CNode<T> *child){
    c_children.push_back(child);
    child->c_parent = this;
}

template<typename T>
CNode<T>* CNode<T>::getParent() const {
    return c_parent;
}

template<typename T>
string CNode<T>::getValue(){
    return s_value;
}

template<typename T>
vector<CNode<T>*> CNode<T>::getChildren() {
    return c_children;
}

template<typename T>
void CNode<T>::vReplaceNode(CNode<T> *c_new_node){
    if(c_parent!=NULL){
        vector<CNode<T>*>& children = c_parent->c_children;
        for(int i=0; i<c_parent->c_children.size(); i++){
            if(children[i]==this){
                children[i] = c_new_node;
            }
        }
    }

    c_new_node->c_parent = this->c_parent;
    delete this;
}

template<typename T>
void CNode<T>::setSValue(string s_new_value){
    s_value = s_new_value;
}

template<typename T>
void CNode<T>::setTValue(T t_new_value){
    t_value = t_new_value;
}

template<typename T>
T CNode<T>::getTValue(){
    return t_value;
}

#endif //LAB4_TEP_CNODE_H

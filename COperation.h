#ifndef LAB3_TEP_COPERATION_H
#define LAB3_TEP_COPERATION_H

#include <vector>
#include "string"
#include "cmath"
using namespace std;

template<typename T>
class COperation {
public:

    typedef T (*OperationFunction)(const vector<T>&);

    explicit COperation(OperationFunction opFun);
    T execute(const vector<T>& operands) const;
    static T t_add(const std::vector<T>& operands);
    static T t_sub(const std::vector<T>& operands);
    static T t_mul(const std::vector<T>& operands);
    static T t_div(const std::vector<T>& operands);
    static T t_sin(const std::vector<T>& operands);
    static T t_cos(const std::vector<T>& operands);
//    OperationFunction getOpFunction();

private:
    OperationFunction op_fun;
};

template<typename T>
COperation<T>::COperation(OperationFunction op_Fun){
    op_fun = op_Fun;
}

template<typename T>
T COperation<T>::execute(const vector<T> &operands)const{
    return op_fun(operands);
}

template<typename T>
T COperation<T>::t_add(const vector<T> &operands) {
    return operands[0] + operands[1];
}

template<typename T>
T COperation<T>::t_sub(const vector<T> &operands){
    return operands[0] - operands[1];
}

template<typename T>
T COperation<T>::t_mul(const vector<T> &operands){
    return operands[0] * operands[1];
}

template<typename T>
T COperation<T>::t_div(const vector<T> &operands){
    return operands[0]/operands[1];
}

template<typename T>
T COperation<T>::t_sin(const vector<T> &operands){
    return sin(operands[0]*M_PI/180);
}

template<typename T>
T COperation<T>::t_cos(const vector<T> &operands){
    return cos(operands[0]*M_PI/180);
}

// implementacja dla stringa bedzie ponizej

template<>
string COperation<string>::t_add(const vector<string> &operands) {
    return operands[0] + operands[1];
}

template<>
string COperation<string>::t_sub(const vector<string> &operands) {
    int i_pos = operands[0].rfind(operands[1]);
    if(i_pos!=-1){
        return operands[0].substr(0, i_pos) + operands[0].substr(i_pos + operands[1].length());
    }
    return operands[0];
}

template<>
string COperation<string>::t_mul(const vector<string> &operands) {
    if(operands[1].empty()){
        return operands[0];
    }

    string s_result = operands[0];
    char c_first_char = operands[1][0];
    string s_insert = operands[1].substr(1);

    int i_pos = 0;

    while((i_pos = s_result.find(c_first_char, i_pos)) != -1){
        s_result.insert(i_pos + 1, s_insert);
        i_pos += s_insert.length() + 1;
    }

    return s_result;
}

template<>
string COperation<string>::t_div(const vector<string> &operands) {
    if(operands[1].size()<=1){
        return operands[0];
    }

    string s_result = operands[0];
    int i_pos = 0;
    char c_first_char = operands[1][0];

    while((i_pos = s_result.find(operands[1], i_pos)) != -1){
        s_result.erase(i_pos + 1, operands[1].length() - 1);
        i_pos += 1;
    }

    return s_result;
}

template<>
string COperation<string>::t_sin(const vector<string> &operands) {

}

template<>
string COperation<string>::t_cos(const vector<string> &operands) {

}

#endif // LAB3_TEP_COPERATION_H

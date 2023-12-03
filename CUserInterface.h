#ifndef LAB4_TEP_CUSERINTERFACE_H
#define LAB4_TEP_CUSERINTERFACE_H
#include "CTree.h"
#include "CInterfaceConstants.h"
#include "sstream"
#include "cstdlib"

template <typename T>
class CUserInterface{
public:
    CUserInterface();
    void vRun();
    T getValueFromString(string s_value);
private:
    CTree<T> c_tree;
};

using namespace std;

template <typename T>
CUserInterface<T>::CUserInterface() {
    CTree<T> tree(S_SPACE1);
    c_tree = tree;
}

template <typename T>
void CUserInterface<T>::vRun() {
    string s_full_command;
    string s_command;
    string s_operation;
    string s_current_prefix;
    bool b_exit = false;

    do{
        cout << S_ENTER_COMMAND;
        getline(cin, s_full_command);
        cout << endl;

        int i_space_pos = s_full_command.find(S_SPACE1);

        if(i_space_pos==I_CHAR_MISSING_VALUE){
            s_command = s_full_command.substr(0);
            s_operation = S_EMPTY_STRING1;
        }
        else{
            s_command = s_full_command.substr(0, i_space_pos);
            s_operation = s_full_command.substr(i_space_pos + 1);
        }

        if(s_command==S_ENTER){
            s_current_prefix = s_operation;
            CTree<T> tree(s_operation);
            if(tree.getWrongArguments()){
                cout << S_WRONG_OPERANDS_COMM << endl;
                s_current_prefix = S_EMPTY_STRING1;
            }
            else{
                c_tree = tree;
                string s_new_prefix = c_tree.sTreeToStr(c_tree.getRoot());
                if(s_current_prefix + S_SPACE1 != s_new_prefix){
                    cout << S_TREE_CHANGED_COMM << endl;
                    s_current_prefix = s_new_prefix;
                    cout << s_current_prefix << endl;
                }

                else{
                    s_current_prefix = s_new_prefix;
                    cout << s_new_prefix << endl;
                }
            }
        }

        if(s_command==S_PRINT){
            cout << S_PREFIX_FORM_COMM;

            if(c_tree.getWasChanged()){
                s_current_prefix = c_tree.sTreeToStr(c_tree.getRoot());
                cout << s_current_prefix << endl;
            }
            else{
                cout << s_current_prefix << endl;
                cout << endl;
            }

            c_tree.vPrintTree();
        }

        if(s_command==S_COMP){
            vector<string> values;

            istringstream iss(s_operation);
            string token;

            while(iss >> token){
                values.push_back(token);
            }

            set<string> variables;
            variables = c_tree.vGetUniqueVariables(c_tree.getRoot(), variables);


            if(values.size()<variables.size()){
                cout << S_NOT_ENOUGH_VALUES_COMM1 << endl;
            }

            else if(values.size()>variables.size()){
                cout << S_TOO_MANY_VALUES_COMM << endl;
            }

            else{
                map<string, T> variable_map;
                set<string>::const_iterator it;
                int i_iter = 0;
                for (it=variables.begin(); it!=variables.end(); it++) {
                    variable_map[*it] = getValueFromString(values[i_iter].c_str());
                    i_iter++;
                }
                try{
                    T t_result = c_tree.tCalculateTreeValue(c_tree.getRoot(), variable_map);
                    cout << S_RESULT_COMM << S_SPACE1 << t_result << endl;
                }
                catch(invalid_argument){
                    cout << S_DIVISION_BY_ZERO << endl;
                }
            }
        }

        if(s_command==S_VARS){
            set<string> variables;
            variables = c_tree.vGetUniqueVariables(c_tree.getRoot(), variables);
            if(variables.empty()){
                cout << S_NO_VARIABLES_IN_TREE_COMM << endl;
            }

            else{
                set<string>::const_iterator it;
                for (it = variables.begin(); it != variables.end(); ++it) {
                    cout << *it << endl;
                }
            }
        }

        if(s_command==S_JOIN){
            if(s_operation!=S_SPACE){
                CTree<T> c_second_tree(s_operation);
                if(!c_second_tree.getWrongArguments()){
                    CTree<T> c_result_tree;
                    c_result_tree = c_tree + c_second_tree;
                    s_current_prefix = c_result_tree.sTreeToStr(c_result_tree.getRoot());
                    cout << S_PREFIX_FORM_COMM << s_current_prefix << endl;
                    c_tree = c_result_tree;
                    c_result_tree.vPrintTree();
                }
            }
        }

        if(s_command==S_EXIT){
            b_exit = true;
        }
    }while(!b_exit);

}

template <>
int CUserInterface<int>::getValueFromString(string s_value){
    return atoi(s_value.c_str());
}

template <>
double CUserInterface<double>::getValueFromString(string s_value){
    istringstream iss(s_value);
    double d_value;
    iss >> d_value;
    return d_value;
}

template <>
string CUserInterface<string>::getValueFromString(string s_value){
    int i_first_quote = s_value.find('\"');
    int i_second_quote = s_value.find('\"', i_first_quote + 1);
    return s_value.substr(i_first_quote + 1, i_second_quote - i_first_quote - 1);
}


#endif //LAB4_TEP_CUSERINTERFACE_H

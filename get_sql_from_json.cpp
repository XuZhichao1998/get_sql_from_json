#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <vector>
using std::map;
using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::pair;
using std::make_pair;

namespace apare {
    void print_head(const map<string,int>& mp_table_header, const vector<vector<string> >& column_names) {
        for (map<string, int>::const_iterator it = mp_table_header.begin(); it != mp_table_header.end(); ++it) {
            cout<<it->first<<":";
            int index = it -> second;
            int len = column_names[index].size();
            cout<<"[";
            for (int i = 0; i < len; ++i) {
                cout<<column_names[index][i];
                if (i != len-1) {
                    cout<<", ";
                } else {
                    cout<<"]\n";
                }
            }
        }
    }

    void print_row_data(const map<string,pair<int,int> >& mp_rows, const vector<vector<string> >& row_datas) {
        for (map<string, pair<int,int> >::const_iterator it = mp_rows.begin(); it != mp_rows.end(); ++it) {
            cout<<it->first<<": [\n";
            int fir = it->second.first;
            int sec = it->second.second;
            for (int i = fir; i < sec; ++i) {
                int len = row_datas[i].size();
                for (int j = 0; j < len; ++j) {
                    if (j == 0) {
                        cout<<"    ["<<row_datas[i][j];
                    } else {
                        cout<<", "<<row_datas[i][j];
                    }
                }
                cout<<"]";
                if (i < sec-1) {
                    cout<<",\n";
                } else {
                    cout<<"\n";
                }
            }
            cout<<"]\n\n";
        }
    }

    void get_column_names(const string& json_str, int start, int end, map<string,int>& mp, vector<vector<string> >& vec) {
        // "Employee":["Id","Name","Salary","DepartmentId"],"Department":["Id","Name"]
        int pos = 0;
        for (int i = start; i < end; ++i) {
            int j = i+1;
            string tb_name;
            while (json_str[j] != '\"') {
                if (!std::isspace(json_str[j])) {
                    tb_name += json_str[j];
                }
                ++j;
            }
            mp[tb_name] = pos++;
            vector<string> v;
            j += strlen("\":[");
            string name;
            for (; json_str[j] != ']'; ++j) {
                if (json_str[j] == '\"') {
                    continue;
                } else if (json_str[j] == ',') {
                    v.push_back(name);
                    name = "";
                } else {
                    name += json_str[j];
                }
            }  
            v.push_back(name);
            vec.push_back(v);
            i = j + 1;
        }          
        return;
    }

    void get_row_datas(const string& json_str, int start, int end, map<string, pair<int,int> >& mp, vector<vector<string> >& vec) {
        int pos = 0;
        //"Employee":[[1,"Joe",85000,1],[2,"Henry",80000,2],[3,"Sam",60000,2],[4,"Max",90000,1],[5,"Janet",69000,1],[6,"Randy",85000,1],[7,"Will",70000,1]],
        //"Department":[[1,"IT"],[2,"Sales"]]
        for (int i = start; i < end; ++i) {
            int j = i+1;
            string tb_name;
            while (json_str[j] != '\"') {
                if (!std::isspace(json_str[j])) {
                    tb_name += json_str[j];
                }
                ++j;
            }
            j += strlen("\":[");

            vector<string> column_values;
            string one_value;
            int square_bracket_count = 1;
            int pos_left = 1E9;
            for (; j < end; ++j) {
                if (json_str[j] =='[') {
                    ++square_bracket_count;
                } else if (json_str[j] == ']') {
                    --square_bracket_count;
                    column_values.push_back(one_value);
                    one_value = "";
                    pos_left = std::min(pos_left, (int)vec.size());
                    vec.push_back(column_values);
                    column_values.clear();
                    ++j;
                    if (json_str[j] == ']') {
                        break;
                    } 
                } else if (json_str[j] ==',') {
                    column_values.push_back(one_value);
                    one_value = "";
                } else {
                    one_value += json_str[j];
                }
            }
            mp[tb_name] = make_pair(pos_left, (int)vec.size());
            i = j + 1;
        }
    }

    void prase_json(const std::string& json_str) {
        int len = json_str.length();
        string header = "{\"headers\":{";
        string rows = "},\"rows\":{";
        int head_start_pos = header.length(); //[
        int head_end_pos = json_str.find(rows); //)
        int rows_start_pos = head_end_pos + rows.length();
        int rows_end_pos = len - strlen("}}");
        map<string, int> mp_table_header;
        vector<vector<string> > column_names;
        get_column_names(json_str, head_start_pos, head_end_pos, mp_table_header, column_names);
        //print_head(mp_table_header, column_names); 

        map<string, pair<int, int> > mp_rows;
        vector<vector<string> > row_datas;
        get_row_datas(json_str, rows_start_pos, rows_end_pos, mp_rows, row_datas);
        //print_row_data(mp_rows, row_datas);

        for (map<string, int>::const_iterator it = mp_table_header.begin(); it != mp_table_header.end(); ++it) {
            string tb_name = it->first;
            int index = it->second;
            int len = column_names[index].size();
            cout<<"DELETE FROM "<<tb_name<<";\n";
            cout<<"INSERT INTO "<<tb_name<<"("<<column_names[index][0];
            for (int j = 1; j < len; ++j) {
                cout<<", "<<column_names[index][j];
            }
            cout<<") VALUES\n";
            pair<int, int> pr = mp_rows[tb_name];
            int fir = pr.first;
            int sec = pr.second;
            for (int j = fir; j < sec; ++j) {
                cout<<"    ("<<row_datas[j][0];
                for (int k = 1; k < len; ++k) {
                    cout<<", "<<row_datas[j][k];
                }
                cout<<(j==sec-1?");\n":"),\n");
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }

    std::string json_str;
    std::string line;
    int curly_count = 0;
    int quotation_count = 0; //‘\"’是奇数还是偶数
    int ca = 0;
    while(getline(std::cin,line)) {
        int len = line.length();
        for(int i = 0; i < len; ++i) {
            if (line[i] == '\"') {
                quotation_count ^= 1;
            }
            if (line[i] == '{') {
                ++curly_count;
            } else if (line[i] == '}') {
                --curly_count;
            }
            if (!std::isspace(line[i])) {
                json_str += line[i];
            } else if (quotation_count == 1) { //引号个数为奇数，说明现在的字符为字符串的内容，原封不动移过去
                json_str += line[i];
            }
        }
        if (curly_count == 0 && json_str != "") {
            if (++ca > 1) {
                cout<<"\n";
            }
            std::cout<<"Case #"<<ca<<":\n";
            apare::prase_json(json_str);
            json_str = "";   
        }
    }

    if (argc == 2) {
        fclose(stdin);
    }
    return 0;
}

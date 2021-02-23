#!/usr/bin/python3

import json;
#db_json = '{"headers": {"Employee": ["Id", "Name", "Salary", "DepartmentId"], "Department": ["Id", "Name"]}, "rows": {"Employee": [[1, "Joe", 85000, 1], [2, "Henry", 80000, 2], [3, "Sam", 60000, 2], [4, "Max", 90000, 1], [5, "Janet", 69000, 1], [6, "Randy", 85000, 1], [7, "Will", 70000, 1]], "Department": [[1, "IT"], [2, "Sales"]]}}'

def get_insert_sql_from_json(db_json):
    db_obj = json.loads(db_json)
    headers = db_obj.get("headers")
    rows = db_obj.get("rows")
    for tb_name in rows.keys():
        print ('DELETE FROM ' + tb_name + ';')
        column_names = headers.get(tb_name)
        column_count = len(column_names)
        if column_count == 0:
            continue
        all_rows = rows.get(tb_name)
        line1 = 'INSERT INTO ' + tb_name + '(' + column_names[0]
        for i in range(1, column_count):
            line1 += ', ' + column_names[i]
        line1 += ') VALUES'
        print (line1)
        row_count = len(all_rows)
        for i in range(0, row_count):
            one_row = all_rows[i]
            if len(one_row) != column_count:
                continue
            line2 = '('
            line2 += '"'+one_row[0]+'"' if type(one_row[0]) == type('') else str(one_row[0]) 
            for j in range(1, column_count):
                line2 += ', '
                line2 += '"'+one_row[j]+'"' if type(one_row[j]) == type('') else str(one_row[j])
            line2 += ');' if i == row_count-1 else '),' 
            print ('   ',line2)
            

if __name__ == "__main__":
    db_json = input()
    get_insert_sql_from_json(db_json)
            
        
    

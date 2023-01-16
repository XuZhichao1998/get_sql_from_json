## Prase_Json

#### 摘要
给定一个确定格式的Json，把它解析成格式化的MySQL语句

#### 格式定义
leetcode中database题目的输入格式
```json
{"headers": {"Employee": ["Id", "Name", "Salary", "DepartmentId"], "Department": ["Id", "Name"]}, "rows": {"Employee": [[1, "Joe", 85000, 1], [2, "Henry", 80000, 2], [3, "Sam", 60000, 2], [4, "Max", 90000, 1], [5, "Janet", 69000, 1], [6, "Randy", 85000, 1], [7, "Will", 70000, 1]], "Department": [[1, "IT"], [2, "Sales"]]}}
```
格式化之后是这样的：
```json
{
	"headers": {
		"Employee": ["Id", "Name", "Salary", "DepartmentId"],
		"Department": ["Id", "Name"]
	},
	"rows": {
		"Employee": [
			[1, "Joe", 85000, 1],
			[2, "Henry", 80000, 2],
			[3, "Sam", 60000, 2],
			[4, "Max", 90000, 1],
			[5, "Janet", 69000, 1],
			[6, "Randy", 85000, 1],
			[7, "Will", 70000, 1]
		],
		"Department": [
			[1, "IT"],
			[2, "Sales"]
		]
	}
}
```
json对象由两部分组成，"headers"给出table的列名，"rows"给出行的内容。
所以上述json对象解析之后得到的格式化mysql DML如下
```sql
DELETE FROM Employee;
INSERT INTO Employee(Id, Name, Salary, DepartmentId) VALUES
    (1, "Joe", 85000, 1),
    (2, "Henry", 80000, 2),
    (3, "Sam", 60000, 2),
    (4, "Max", 90000, 1),
    (5, "Janet", 69000, 1),
    (6, "Randy", 85000, 1),
    (7, "Will", 70000, 1);
DELETE FROM Department;
INSERT INTO Department(Id, Name) VALUES
    (1, "IT"),
    (2, "Sales");
```
我们现在要做的，就是给一个字符串，转化为我们这样格式的MySQL DML  
Try it!  

**文件说明：**
- 词法分析器lex.yy.c由lex源文件clang.l使用lex自动生成，经过gcc编译得到可执行文件lex.yy。
- demo.c, demo2.c是用于测试的待解析的c文件。
- tokens.txt存放词法解析输出结果。

**使用方式：**
- 在Makefile中可修改变量LEX_FILE和INPUT_FILE。
- make build编译。
- make run运行。
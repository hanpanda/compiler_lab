**程序运行说明：**

       1. `make clean && make` 生成可执行文件 `complier`
       2. 文件夹`test\`中存放测试样例代码，如：`test1.txt，test2.txt`，需要为`txt`格式的文件
       3. 执行程序：`complier ./test/test1.txt`，生成相应的test1.asm，test1.tok, test1.qua，分别是汇编代码，tokens文件，四元式文件

**生成的asm文件运行说明：**

       1. 可以在`asm`文件中适当的地方加入调用C函数`printf`的代码以便于观察输出结果，给出的样例代码中已经加了这部分代码，但是自动生成的`asm`代码中没有
       2. 生成目标文件：`nasm -felf <input>.asm -o <output>.o` 
       3. 链接：`gcc -m32 <output>.o -o <output>`

github链接：https://github.com/hanpanda/compiler_lab
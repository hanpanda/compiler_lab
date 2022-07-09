**程序运行说明：**

       1. `make clean && make` 生成可执行文件 `complier`
       2. 文件夹`test\`中存放测试样例代码，如：`test1.txt，test2.txt`，需要为`txt`格式的文件
       3. 执行程序：`complier ./test/test1.txt`，生成相应的test1.asm，tokens.txt

**生成的asm文件运行说明：**

       1. 可以在`asm`文件中适当的地方加入调用C函数`printf`的代码以便于观察输出结果
        （参考给出的`test1.asm，test2.asm`）
       2. 生成目标文件：`nasm -felf <input>.asm -o <output>.o` 
       3. 链接：`gcc -m32 <output>.o -o <output>`

github链接：https://github.com/hanpanda/compiler_lab
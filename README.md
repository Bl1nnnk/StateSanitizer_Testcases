# StateSanitizer_Testcases 编写需求

需要完成**基础API编写**、**添加hypercall**、**静态编译与测试**三个步骤。

## 基础API编写

编写尽可能多的API调用序列，覆盖某一子系统所有的功能。如果参数支持多类型输入，则要覆盖所有类型。同时保证输入正常参数数据保证API被正确调用。

- **编写尽可能多的API调用序列，覆盖某一子系统所有的功能**
- **需要保证API和参数的正确使用**
- **覆盖所有的参数类型**
- **如果某个参数是optional，则尽可能的使用它**
- **不需要做任何变异**
- **推荐使用ChatGPT辅助编写**

比如对于文件操作类：
```
CreateFile
SetFileInformationByHandle
GetFileInformationByHandleEx
CloseHandle
```

算是一组调用序列。一组用序列可视为一个测试用例。
其中SetFileInformationByHandle第二个参数FileInformationClass包含多个类型：
```
typedef enum _FILE_INFO_BY_HANDLE_CLASS {
  FileBasicInfo,
  FileStandardInfo,
  ...
  FileNormalizedNameInfo,
  MaximumFileInfoByHandleClass
} FILE_INFO_BY_HANDLE_CLASS, *PFILE_INFO_BY_HANDLE_CLASS;
```
需要保证这些参数类型全覆盖。

我们重点关注Chrome沙箱内可访问的子系统，包括不限于：
- NTFS
- ALPC
- Named Pipe
- Thread/Process
- ETW

推荐使用Project0的工具[sandbox-attacksurface-analysis-tools](https://github.com/googleprojectzero/sandbox-attacksurface-analysis-tools)来判断哪些子系统/接口可以在Chrome沙箱内可访问。


## 静态编译运行时库

1. 选择Release编译。
2. 右键项目 => 属性 => 配置属性 => C/C++ => 代码生成 => 运行库 => 选择多线程。

![静态编译](doc/img/img0.jpeg)

## 添加hypercall

### 添加文件

在项目中添加agent.h和hypercall.asm，并正确设置hypercall.asm的编译：

1. 将[example](example/testcase0)中的文件agent.h和hypercall.asm放到项目文件夹中。
2. 在“头文件”中添加agent.h，在“源文件”中添加hypercall.asm.
3. 右键hypercall.asm，选择属性。
4. “从生成中排除”选择“否”。
5. “项类型”选择“自定义生成工具”。
6. 点击应用，右侧“配置属性”会出现“自定义生成工具”一项。
7. 点击“自定义生成工具”，在“命令行”输入`ml64 /Fo $(IntDir)%(fileName).obj /c %(fileName).asm`，在“输出”中输入`$(IntDir)%(fileName).obj`。

![生成中排除-否](doc/img/img1.jpeg)

![项类型](doc/img/img2.jpeg)

![自定义生成工具](doc/img/img3.jpeg)

### 代码插入
1. 在main文件中添加`#include "agent.h"`
2. 在mian函数的开始，添加`tp_hypercall(TP_FUNC_SUBMIT_CR3, 0);`
3. 在测试目标API之前，添加`tp_hypercall(TP_FUNC_BEGIN_FUZZ, 0);`

![代码插入](doc/img/img4.jpeg)

## 编译并测试testcase
保证测试用例中API被正确的调用。

## 欢迎建设自动化方案实现上面步骤

# Example
一个完整的测试用例[demo](example/testcase0)，里面包含了agent.h和hypercall.asm文件。

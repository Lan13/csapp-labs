# 计算机系统概论 实验报告

姓名：蓝俊玮 	学号：PB20111689

[TOC]

## 实验摘要

在本课程中，我一共完成了下列 2 个实验：

- Data Lab：实现简单的逻辑、二进制补码和浮点函数
- Shell Lab：使用 job control 实现一个简单 Unix shell 程序

## 实验环境

我使用的是 Windows 下的 WSL2

![](image/csapp-env.png)

## 实验介绍

### Data Lab

首先获取实验框架，然后解压：

```bash
wget http://csapp.cs.cmu.edu/3e/datalab-handout.tar
tar -xvf datalab-handout.tar
rm datalab-handout.tar
```

同时配置环境：

```bash
sudo apt-get install gcc-multilib
```

#### 1. `bitXor` 函数

**目标：只使用符号按位非 `~` 和符号按位与 `&` 来实现按位异或 `^`**

学过 “模拟与数字电路 / 011151” 这门课程后，我们知道：

$$
A \oplus B = A\overline{B}+\overline{A}B = \overline{\overline{A\overline{B}} \ \overline{\overline{A}B}}
$$

其中最后一步利用到了**德摩根定律**：

$$
\overline{A\overline{B}+\overline{A}B}=\overline{A\overline{B}}\ \overline{\overline{A}B}
$$

所以可以得到函数为：

```c
int bitXor(int x, int y) {
  // x ^ y = x & ~y + ~x & y = ~(~(x & ~y) & ~(~x & y))
  return ~(~(x & (~y)) & ~((~x) & y));
}
```

#### 2. `tmin` 函数

**目标：返回最小二进制补码整数**

我们知道，`int` 的字长为 32 位，而最高位表示的符号位。当最高位为 1 时，则表示为负数。而最小二进制补码整数就是 **`0x80000000`**。

所以可以得到函数为：

```c
int tmin(void) {
  // minimum two's complement integer is 0x80000000
  return 0x1 << 31;
}
```

#### 3. `isTmax` 函数

**目标：判断是否为最大二进制补码整数**

同上述，我们知道最大二进制补码整数就是 `0x7fffffff`。因此，我们的目标就是判断输入的数 `x` 是否为该数。为了对其进行判断，**我们可以使用 `~(x + 1) ^ x` 对其进行判断。**因为 `(x + 1) = 0x80000000, ~(x + 1) = 0x7fffffff, ~(x + 1) ^ x = 0x0`，所以可以通过这个特征鲜明的判断出 `0x7fffffff`。但是注意到 `x = 0xffffffff` 也满足 `~(x + 1) ^ x = 0`，因此还需要对 `0xffffffff` 进行判断。

所以可以得到函数为：

```c
int isTmax(int x) {
  // maximum two's complement number is 0x7fffffff
  // x will overflow when it plus 1, so we use `~(x + 1) ^ x` to judge
  // and to make `~(x + 1) ^ x` work we need to judge 0xffffffff
  // because 0xffffffff also satisfy `~(x + 1) ^ x`
  return !((~(x + 1) ^ x) | !(~x));
}
```

#### 4. `allOddBits` 函数

**目标：判断所有奇数位置是否都为 1**

对于 32 位的 int 数据来说，奇数位置全为 1 的数字为 `0xAAAAAAAA`，因此我们可以让输入的数 `x` 和 `0xAAAAAAAA` 进行**按位与**操作，并对操作后的结果进行判断。

所以可以得到函数为：

```c
int allOddBits(int x) {
  // we need to judge if x's every odd-numbered bit is 1
  // so we compare with 0xAAAAAAAA
  int target_number = 0xAA + (0xAA << 0x8) + (0xAA << 0x10) + (0xAA << 0x18);
  return !((x & target_number) ^ target_number);
}
```

#### 5. `negate` 函数

**目标：返回一个输入数字的相反数**

学过 “模拟与数字电路 / 011151” 这门课程后，我们知道一个数字的相反数就是其**补码的取反＋1**。

所以可以得到函数为：

```c
int negate(int x) {
  // easy:) just invert and add 1
  return ~(x) + 1;
}
```

#### 6. `isAsciiDigit` 函数

**目标：判断一个数字大小是否位于区间 `0x30 <= x <= 0x39`**

我们可以将这个目标条件**转化为两个等价的条件：`x - 0x30 >= 0` 和 `0x39 - x >= 0`**，然后对这两个条件进行判断真假，只有当这两个条件都成立时，原条件才成立。而为了判断一个数字是否大于等于 0，我们可以使用符号位来进行快速判断。当符号位为 0 的时候，说明该数字是大于等于 0 的。

所以可以得到函数为：

```c
int isAsciiDigit(int x) {
  // judge x - 0x30 >= 0 and 0x39 - x >= 0
  // use most significant bit to judge sign
  int most_significant_bit = 0x1 << 31;
  int x_substract_0x30 = x + ~(0x30) + 1;
  int x39_substract_x = 0x39 + ~(x) + 1;
  return !((most_significant_bit & x_substract_0x30) | (most_significant_bit & x39_substract_x));
}
```

#### 7. `conditional` 函数

**目标：实现与 `x ? y : z` 一样的功能**

可以看出，`x` 作为选择条件，根据其正负来选择另一个值，那么就要**设置掩码 `mask` 为 `0x0` 或者 `0xffffffff`**。首先还是根据 `x` 的符号来进行判断，然后对其进行左移和右移。注意：负数在右移的时候，会在高位补 1，而不是补 0。

所以可以得到函数为：

```c
int conditional(int x, int y, int z) {
  // use most significant bit to judge x >= 0 and switch one result
  // use left shift and right shift to change
  int result_mask = (!x << 31) >> 31;
  return (y & ~(result_mask)) + (z & result_mask);
}
```

#### 8. `isLessOrEqual` 函数

**目标：判断两个数的大小关系 `x <= y` 是否成立**

我们可以将这个**目标条件转换为 `y - x > = 0`**。在这里，因为 `x` 和 `y` 的取值范围较大，所以对两数进行计算时容易发生溢出现象。我们知道，只有当两个数的符号为异号相减的时候才会发生溢出。所以可到下述三个条件：

- 若 `x` 为非负数而 `y` 为负数，则 `x <= y` 不成立
- 若 `x` 为负数而 `y` 为非负数，则 `x <= y` 成立
- 若 `x` 和 `y` 正负关系相同，则判断 `y - x >= 0`

所以可以得到函数为：

```c
int isLessOrEqual(int x, int y) {
  // when x and y aren't the same sign, they may overflow when substracting
  // then use most significant bit to judge y - x >= 0
  int most_significant_bit = 0x1 << 31;
  int positive_x_negative_y = (!(x & most_significant_bit)) & (!!(y & most_significant_bit));
  int negative_x_positive_y = (!!(x & most_significant_bit)) & (!(y & most_significant_bit));
  int y_substract_x = !((y + ~(x) + 1) & most_significant_bit);
  return negative_x_positive_y | ((!positive_x_negative_y) & y_substract_x);
}
```

#### 9. `logicalNeg` 函数

**目标：判断一个数是否为 0**

0 这个数字具有一个特征就是， 0 的相反数就是它本身。则可以根据这一特征进一步得知：0 和 -0 的最高位（符号位）都是 0。那么对**自身符号位和相反数符号位**进行判断即可。

所以可以得到函数为：

```c
int logicalNeg(int x) {
  // judge 0, 0 and -0 have the same most significant bit 0
  return (((~(x) + 1) | x) >> 31) + 1;
}
```

#### 10. `howManyBits` 函数

**目标：判断一个函数最少需要多少位来表示**

因为正数和负数的表示逻辑是不一样的。对于正数来说，前面的 0 是可以忽略省去的；而对于负数来说，前面的 1 是可以忽略省去的。因此首先需要对其进行统一的表示。接着**依此构造 16, 8, 4, 2, 1 位的掩码用来指示需要多少位**进行表示。因为是从 0 开始计算的，因此最后还需要加上 1。

所以可以得到函数为：

```c
int howManyBits(int x) {
  // make 16, 8, 4, 2, 1 bits mask to represent
  int temp = x ^ (x << 1);
  int bit_16, bit_8, bit_4, bit_2, bit_1;
  bit_16 = !!(temp >> 16) << 4;
  temp = temp >> bit_16;
  bit_8 = !!(temp >> 8) << 3;
  temp = temp >> bit_8;
  bit_4 = !!(temp >> 4) << 2;
  temp = temp >> bit_4;
  bit_2 = !!(temp >> 2) << 1;
  temp = temp >> bit_2;
  bit_1 = !!(temp >> 1);
  return bit_16 + bit_8 + bit_4 + bit_2 + bit_1 + 1;
}
```

#### 11. `floatScale2` 函数

**目标：给出输入浮点数 `f`，返回 `2 * f` 的值**

根据 IEEE 754 浮点数标准详解，我们知道浮点数是由 32 位来进行表示的：其中最高位 31 位是符号位，23 - 30 位是指数位，0 - 22 位是有效数字位。首先我们需要获取 `f` 的指数位和有效数字位，**根据指数位的值来确定 `f` 是什么数值范围的**。指数位的值可以分为如下情况：

- 格式化值：指数位的值不全为 0 也不全为 1
- 特殊数值：指数位的值全为 1。当有效数字位非零时，则表示为 Not a Number；当有效数字为零时，则表示为无穷大 $\infty$
- 非格式化值：指数位的值全为 0。用来表示那些非常接近于 0.0 的数

所以可以得到函数为：

```c
unsigned floatScale2(unsigned uf) {
  // first judge exponent, then refer to IEEE
  unsigned exponent = (uf & 0x7f800000) >> 23;
  unsigned fraction = uf & 0x007fffff;
  if (exponent == 0xff) // Not a Number or Infinity
    return uf;
  if (exponent == 0x0)
    return (uf & 0x80000000) + (exponent << 23) + (fraction << 1);
  return (uf & 0x80000000) + ((exponent + 1) << 23) + fraction;
}
```

#### 12. `floatFloat2Int` 函数

**目标：将输入浮点数 `f` 类型转换为整型**

同样，根据 IEEE 754 浮点数标准详解，根据指数位的值来确定 `f` 是什么数值范围的。

1. 如果 `f` 的指数位是小于 bias 127 的话，说明 `f` 是个小数，那么类型转换后就是 0
2. 如果 `f` 的指数位是大于 bias 127 + 31 的话，说明其指数范围过大，在类型转换之后，会超出 int 类型的表示范围，所以在类型转换之后会发生溢出，那么类型转换后就是 `0x80000000u`
3. 在正常情况下，根据 `f` 指数位减去 bias 127 后剩余的位数来确定转换后的值，同时要需要判断 `f` 的符号位。

所以可以得到函数为：

```c
int floatFloat2Int(unsigned uf) {
  // first judge exponent, then refer to IEEE
  unsigned exponent = (uf & 0x7f800000) >> 23;
  int value = 1 << (exponent - 0x7f); // get int exponent bits;
  if (exponent < 0x7f)  // fraction
    return 0;
  if (exponent >= (0x7f + 0x1f))  // int value overflow 
    return 0x80000000u;
  return ((0x1 << 31) & uf) ? (-value) : value; // check sign bit
}
```

#### 13. `floatPower2` 函数

**目标：给出整数 `x`，返回浮点数 `2.0 ^ x ` 的浮点表示结果**

同样，根据 IEEE 754 浮点数标准详解，需要判断计算表达式 `2.0 ^ x` 的指数位范围：**因为是需要用浮点表示结果，所以指数位需要加上 bias 127。**在加上 bias 127 之后，若指数位范围还是小于等于 0 的话，说明计算结果太小，则返回 0；若指数位范围是大于等于 255 的话，说明这个计算结果太大，则返回正无穷 `0x7f800000`；其它情况则正常返回其浮点表示结果。

所以可以得到函数为：

```c
unsigned floatPower2(int x) {
  // first judge exponent, then refer to IEEE
  int exponent = x + 127;
  if (exponent <= 0)  // underflow
    return 0;
  if (exponent >= 255)  // Not a Number or Infinity
    return 0xff << 23;
  return exponent << 23;
}
```

#### Data Lab 结果展示

![](image/csapp-datalab.png)

### Shell Lab

首先获取实验框架，然后解压：

```bash
wget http://csapp.cs.cmu.edu/3e/shlab-handout.tar
tar -xvf shlab-handout.tar
rm shlab-handout.tar
```

#### 1. `sigchld_handler` 函数

`sigchld_handler` 函数用来处理 `SIGCHLD` 信号，`SIGCHLD` 信号产生有以下这些条件：

- 子进程终止时会向父进程发送 `SIGCHLD` 信号，告知父进程回收自己，但该信号的默认处理动作为忽略，因此父进程仍然不会去回收子进程，需要捕捉处理实现子进程的回收。
- 子进程收到 `SIGSTOP` 信号后被强制结束
- 子进程暂停运行，接受到 `SIGCONT` 后唤醒时

因此根据提示，可以在 `sigchld_handler` 中通过使用一个函数 `waitpid` 并且搭配到参数 `WUNTRACED` 和 `WNOHANG` 来获取信号对应的进程号 `pid` 和状态 `status`。`WNOHANG` 选项使得 `waitpid` 函数变为同步函数，如果没有僵死进程就能立刻返回；`WUNTRACED` 选项使得这一函数能够处理暂停的进程。捕捉这些信息后再判断信息的类别：

- 如果在捕捉信息的过程中发现子进程是暂停运行，则需要将子进程的状态改为暂停 `ST`
- 如果在捕捉信息的过程中发现子进程是结束运行，则需要将子进程从任务 `jobs` 列表中删除

```c
void sigchld_handler(int sig)
{
    int jid, status;
    pid_t pid;
    struct job_t* job;
    
    // In sigchld handler, use exactly one call to waitpid.
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        jid = pid2jid(pid);
        job = getjobpid(jobs, pid);
        // judge child process status
        if (WIFSTOPPED(status)) {   // suspend
            printf("Job [%d] (%d) stopped by signal %d\n", jid, pid, WSTOPSIG(status));
            job->state = ST;
        }
        else {      // stop
            deletejob(jobs, pid);
            if (WIFSIGNALED(status)) {
                printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, WTERMSIG(status));
            }
        }
    }
    return;
}
```

#### 2. `sigint_handler` 函数

`sigint_handler` 函数用来处理 `SIGINT` 信号。这个函数比较简单，只需要获取当前前台的任务 `pid`，然后调用 `kill(-pid, SIGINT)` 函数来向进程发送信号即可。

```c
void sigint_handler(int sig)
{
    pid_t pid = fgpid(jobs);
    if (pid) {
        kill(-pid, SIGINT);
    }
    return;
}
```

#### 3. `sigtstp_handler` 函数

`sigtstp_handler` 函数用来处理 `SIGTSTP` 信号。同理，只需要获取当前前台的任务 `pid`，然后调用 `kill(-pid, SIGTSTP)` 函数来向进程发送信号即可。

```c
void sigtstp_handler(int sig)
{
    pid_t pid = fgpid(jobs);
    if (pid) {
        kill(-pid, SIGTSTP);
    }
    return;
}
```

#### 4. `waitfg` 函数

根据实验提示

> In waitfg, use a busy loop around the sleep function.

因此可以这样实现：

```c
void waitfg(pid_t pid)
{
    struct job_t* job = getjobpid(jobs, pid);
    // In waitfg, use a busy loop around the sleep function
    while (job && job->state == FG) {
        sleep(1);
    }
    return;
}
```

#### 5. `do_bgfg` 函数

首先，`bg` 和 `fg` 指令的输入格式一般都是 `bg/fg pid` 或者 `bg/fg %jid`。因此在调用 `do_bgfg` 函数时，重点需要考察的就是第 1 个参数（这里不考虑有多个参数，因此程序健壮性比较差）

- 如果没有第一个参数，那么直接跳过
- 如果给出的第一个参数是进程号 `pid`，则根据 `pid` 获取任务 `job`
- 如果给出的第一个参数是 `jid`，则根据 `jid` 获取任务 `job`
- 如果给出的第一个参数格式不匹配，则直接跳过

接着根据对应的指令，更改进程的状态：如果是 `bg` 指令，则将进程状态改为 `BG`；如果是 `fg` 指令，则将进程状态改为 `FG`。然后需要用 `SIGCONT` 信号唤醒进程，这里注意在 `kill` 函数中需要使用 `-pid` 来替代 `pid`。最后，如果是 `fg` 指令的话，还需要调用 `waitfg` 来等待前台进程结束。

```c
void do_bgfg(char **argv)
{
    if (argv[1] == NULL) {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }
    struct job_t* job;
    int jid;
    pid_t pid; 
    // if argv[1] contains a job id
    if (sscanf(argv[1], "%%%d", &jid) > 0) {    // `%%%d` -> %jid
        job = getjobjid(jobs, jid);
        if (job == NULL) {
            printf("%%%d: No such job\n", jid);
            return;
        }
    }
    // if argv[1] contains a process id
    else if (sscanf(argv[1], "%d", &pid) > 0) {      // `%d` -> pid
        job = getjobpid(jobs, pid);
        if (job == NULL) {
            printf("(%d): No such process\n", pid);
            return;
        }
    }
    // argument format error: not a job id or process id
    else {
        printf("%s: argument must be a PID or %%jobid\n", argv[0]);
        return;
    }
    // change job state according to the input command 
    if (strcmp(argv[0], "bg") == 0) {
        job->state = BG;
    }
    else if (strcmp(argv[0], "fg") == 0) {
        job->state = FG;
    }
    // using "-pid" instead of "pid" in the argument to the kill function
    kill(-job->pid, SIGCONT);
    if (strcmp(argv[0], "bg") == 0) {
        printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
    }
    else if (strcmp(argv[0], "fg") == 0) {
        waitfg(job->pid);   // only one foreground process allowed
    }
    return;
}
```

#### 6. `bulitin_cmd` 函数

`bulitin_cmd` 函数的实现逻辑其实很简单，只需要判断相应指令即可。

- `quit` 指令：直接调用 `exit(0)` 退出
- `fg` 指令和 `bg` 指令：直接调用 `do_bgfg` 函数并且返回 1
- `jobs` 指令：直接调用 `listjobs` 函数并且返回 1
- 若非上述内置指令，则返回 0

```c
int builtin_cmd(char **argv)
{
    // quit command
    if (strcmp(argv[0], "quit") == 0) {
        exit(0);
    }
    // fg command
    if (strcmp(argv[0], "fg") == 0) {
        do_bgfg(argv);
        return 1;
    }
    // bg command
    if (strcmp(argv[0], "bg") == 0) {
        do_bgfg(argv);
        return 1;
    }
    // jobs command
    if (strcmp(argv[0], "jobs") == 0) {
        listjobs(jobs);
        return 1;
    }
    return 0; /* not a builtin command */
}
```

#### 7. `eval` 函数

`eval` 函数首先要对命令行 `cmdline` 进行解析 `parseline`，得到参数列表 `argv`，同时判断用户使用的是否为 `BG` 命令。如果命令行为空，则直接返回。当参数列表指示命令行不是内置指令时，则需要创建一个新的子进程来运行指令。

根据文档中的提示，在创建一个新的子进程之间，必须调用 `sigprocmask` 来阻塞 `SIGCHLD` 信号，这是因为父进程需要以这种方式阻止 `SIGCHLD` 信号，以避免出现同步竞争问题。所以使用：

```c
// initial mask
sigemptyset(&mask);
// set SIGCHLD mask
sigaddset(&mask, SIGCHLD);
// the parent must use sigprocmask to block SIGCHLD 
// signals before it forks the child
sigprocmask(SIG_BLOCK, &mask, &prev_mask);
```

然后调用 `fork()` 创建子进程。`fork()` 之后，在子进程中执行命令之前，需要调用 `setpgid(0, 0)`，它将子进程放入到一个新的进程组，其组 `id` 与子进程的 `pid` 相同。这样可以确保在前台进程中只有一个进程。并且由于子进程继承了父进程的阻塞向量，因此子进程必须确保在执行新进程之前解除阻塞 `SIGCHLD` 信号。因此在子进程中：

```c
if (pid == 0) {     // child process
    // according to workaround, this ensures that there will 
    // be only one process, your shell, 
    // in the foreground process group
    setpgid(0, 0);
    // Since children inherit the blocked vectors of their parents,
    // the child must be sure to then unblock SIGCHLD 
    // signals before it execs the new program.
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    if (execve(argv[0], argv, environ) < 0) {
        printf("%s: Command not found\n", argv[0]);
        exit(0);
    }
}
```

在父进程中，也需要在调用 `addjob` 将子进程加入到工作列表之后调用 `sigprocmask` 来解除阻塞 `SIGCHLD` 信号。因此在父进程中：

```c
else {  // parent process
    addjob(jobs, pid, (bg == 1 ? BG : FG), cmdline);
    // again using sigprocmask after it adds 
    // the child to the job list by calling addjob
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    if (!bg) {  // if job is FG job
        waitfg(pid);
    }
    else {      // if job is BG job
        printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
    }
} 
```

#### Shell Lab 结果展示

因为本次实验并没有给出一个统一的测试脚本，因此使用一个自己编写的脚本来快速识别程序的正确性。脚本的具体内容如下：

```shell
#! /bin/bash

touch tsh.out
make test01 > tsh.out
for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
do
    make test$i >> tsh.out
done

sed -i 's|(\b[0-9]*)|(1000)|g;' tsh.out
sed -i 's|(\b[0-9]*)|(1000)|g;' tshref.out

touch diff.out
echo "蓝俊玮 PB20111689" > diff.out
whoami >> diff.out
diff tsh.out tshref.out >> diff.out
```

首先创建一个新的文件 `tsh.out` 来记录 `tsh` 程序的输出结果。首先使用 `make test01` 并且使用重定向符 `>` 将输出覆盖到 `tsh.out` 文件中，紧接着使用一个循环，将后续测试的输出结果追加到 `tsh.out` 文件当中。因为程序每次运行的 pid 不一定会相同，因此将输出信息中的 `(pid)` 部分替换成 `(1000)`，以便于比较。这一点可以通过 `sed sed -i 's|(\b[0-9]*)|(1000)|g;' tsh.out` 完成。最后创建一个新的文件 `diff.out` 文件，并且运行 `diff tsh.out tshref.out > diff.out` 指令，将两个文件的不同信息记录到 `diff.out` 当中，以便于检查程序与参考程序的输出有何不同。

下面便是结果展示（在这里我预先删除了 `tshref.out` 当中的两条 `make` 信息）：

![](image/csapp-shell-diff1.png)

![](image/csapp-shell-diff2.png)

![](image/csapp-shell-diff3.png)

可以看出 `tsh.out` 和 `tshref.out` 之间只有 `/bin/ps` 的运行结果有较大差别，但这是不可避免的。并且可以看出程序 `tsh` 执行 `/bin/ps` 的输出是正常的（只是因为我的实验环境在 WSL 中，而多出了许多无关的内容），因此可以认为 `tsh` 的行为与 `tshref` 达到了一致，因此符合实验要求。


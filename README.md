



# HT_project

合泰杯-基于HT单片机的无人送货与售货智能机器人



## GitHub使用简介

#### 上传

在zgl分支下编程，如果代码测试成功某个功能或模块，就push到本分支上

```
git add .
git commit -m "注释内容"
git push
```



在zgl分支下编程，如果有了一个稳定的版本，就与主分支合并（要保证主分支始终是一个可行的版本）

1. 先讲代码放到暂存区：

```
git add .
git commit -m "注释内容"
```

2. 接着切换回主分支：

```
git checkout main
```

3. 在main分支的情况下，将zgl分支与main分支合并：

```
git merge zgl
```

4. 最后将合并后的内容推到github上面

```
git push
```



#### 拉取

1. 在main分支想要将github上的代码拉取到zgl分支，需要先切换到zgl分支：

```
git checkout zgl
```

2. 将仓库代码拉取到当前分支

```
git pull
```



---



或者如下操作：

- 将远程主机 origin 的 main分支拉取过来，与本地的 zgl分支合并。

```git
git pull origin main:zgl
```

- 如果远程分支是与当前分支合并，则冒号后面的部分可以省略。

```
git pull origin main
```


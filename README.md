



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

1. 先讲代码push到本分支

```
git add .
git commit -m "注释内容"
push
```

2. 接着切换回主分支：

```
git checkout main
```

3. 在main分支的情况下，将zgl分支已经push的代码拉下来与main分支合并：

```
git pull origin zgl
```

4. 使用VSCode或keil、文本编辑器去修改有冲突的代码，选择保留哪部分
4. 最后将合并后的内容推到github上面

```
git add .
git commit -m "注释内容"
push
```

如果不小心在main分支下编程，这时可以Push但是不能切换到别的分支
1. 因为更改后的东西与其他分支下的相同文件发送冲突（别的分支修改在切换Main分支则不会）
2. 使用```git stash```先让修改暂存，然后就可以切换了（git stash将工作区恢复到上次提交的内容，同时备份本地所做的修改）
3. 接着可以执行```git stash pop```将之前本地做的修改应用到当前工作区。


#### 拉取

1. 在main分支想要将github上的代码拉取到zgl分支，需要先切换到zgl分支：

```
git checkout zgl
```

2. 将远程指定分支 拉取到 本地当前分支上：

```
git pull origin <远程分支名> // 比如 git pull origin main
```



如果只想拉取本分支的代码

```
git pull
```



#### 更新远程分支信息
```
git fetch origin --prune
```

#### 查看所有分支
```
git branch -v
```

#### 解决拉取文件冲突
1. 拉取完成后，此时由于存在文件冲突，因此当前的分支状态属于MERGING
2. 使用文本编辑器或者vscode打开
2. 找到标识有HEAD的地方进行修改，上面为自己的，下面为拉取过来的（使用vscode可以方便修改）
3. 注意修改所有的地方（有时候proj文件里面由于配置也会产生冲突，也需要进行）
4. 再次```git add .```
5. ```git commit -m "xxx"```
6. ```git push```

#!/bin/bash

MAIN_BEANCH_PATH="master"
BRANCH_PROJECT="$(git rev-parse --abbrev-ref HEAD)"

## 执行打包脚本
set -e
hitPack(){
  cd ../..
  ./gradlew :modules:momo-ui:clean :modules:momo-ui:uploadArchives
}


## 提示push代码远端
isCommit(){
  if [ -n "$(git status --porcelain)" ]; then
    echo "还有未提交的，请现处理";
    exit 1
  else
      echo "已全部提交";
      checkoutMain
  fi
}

## 切换到主分枝并merge开发分支到主分枝并打包
checkoutMain(){
  git pull --rebase
  echo "当前分支 $BRANCH_PROJECT 已是最新代码"

  git checkout $MAIN_BEANCH_PATH
  git pull --rebase

  if [ -n "$(git merge BRANCH_PROJECT)" ]; then
    echo "存在冲突，请解决"
    exit 1
  else
    hitPack
  fi
}


## 解决冲突之后，重新运行当前脚本
mainBranchCommit(){
  if [ -n "$(git merge BRANCH_PROJECT)" ]; then
    git add .
    git commit
    git push origin $MAIN_BEANCH_PATH
    hitPack
  fi
}


if [ "$BRANCH_PROJECT" = "$MAIN_BEANCH_PATH" ]; then
  if [ -n "$(git status --porcelain)" ]; then
    mainBranchCommit
  else
    hitPack
  fi
  exit 1
else
  echo "当前分支： $BRANCH_PROJECT";
  isCommit
fi











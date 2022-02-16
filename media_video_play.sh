#!/bin/bash

#MAIN_BEANCH_PATH="master"
#BRANCH_PROJECT="$(git rev-parse --abbrev-ref HEAD)"

MODULES_NAME=""
DEV_BRANCH_NAME=""
MAIN_BRANCH_PATH=""

MODULES_FIRST_NAME=""
MODULES_LAST_NAME=""


set -e
info(){
    echo "./media_video_play.sh [-m] modules [-d] development branch [-b] main branch [-v] version"
    echo "m 指定modules, 对应:modules:xxx"
    echo "d 指定开发分支， 指定当前是在那个开发分支进行开发，用于开发分支打包"
    echo "b 主分枝， merge时开发分支与主分枝都必填，merge后产生冲突，解决冲突之后，可只填主分枝"
    echo "v 版本号，不需要输入，默认版本号规则为xxx.20200310104130"
}
if [ $# -lt 1 ]; then
    info
    exit 1
fi


# 获取输入参数
while getopts "m:d:b:" opt; do
  echo " ${opt}    ${OPTARG}"
  case ${opt} in
  m)
    MODULES_NAME=${OPTARG}
    ;;
  d)
    DEV_BRANCH_NAME=${OPTARG}
    ;;
  b)
    MAIN_BRANCH_PATH=${OPTARG}
    ;;
  ?)
    echo "error: 无效参数"
    info
    exit 1
  esac
done


if [ "${MODULES_NAME}" != "" ]; then

  OLD_IFS="$IFS"
  IFS=":"
  modulesPathArray=(${MODULES_NAME})
  IFS="$OLD_IFS"
  for (( i = 0; i < ${#modulesPathArray[*]}; i++ )); do
    if [ "$i" -eq 1 ]; then
      MODULES_FIRST_NAME=${modulesPathArray[i]}
    elif [ "$i" -eq 2 ]; then
      MODULES_LAST_NAME=${modulesPathArray[i]}
    fi
  done
fi


if [[ "${MODULES_FIRST_NAME}" == "modules"  &&  "${MODULES_LAST_NAME}" != "" ]]; then
  if [ ! -d "${MODULES_FIRST_NAME}" ]; then
      echo "找不到文件位置 /n"
      info
      exit 1
  else
    cd "${MODULES_FIRST_NAME}"
  fi

  if [ ! -d "${MODULES_LAST_NAME}" ]; then
    echo "找不到文件位置"
    info
    exit 1
  else
    cd "${MODULES_LAST_NAME}"
  fi

else
  info
  exit 1
fi

#=======================

BRANCH_PROJECT="$(git rev-parse --abbrev-ref HEAD)"


if [  ]; then

fi



read -p "是否操作当前分支 ${BRANCH_PROJECT} ？Y|N : " add_params
if [[ $add_params == "Y" || $add_params == "y" ]]; then

else
  info
  exit
fi



























### 执行打包脚本
#set -e
#hitPack(){
#  cd ../..
#  ./gradlew :modules:momo-ui:clean :modules:momo-ui:uploadArchives
#}
#
#
### 提示push代码远端
#isCommit(){
#  if [ -n "$(git status --porcelain)" ]; then
#    echo "还有未提交的，请现处理";
#    exit 1
#  else
#      echo "已全部提交";
#      checkoutMain
#  fi
#}
#
### 切换到主分枝并merge开发分支到主分枝并打包
#checkoutMain(){
#  git pull --rebase
#  echo "当前分支 $BRANCH_PROJECT 已是最新代码"
#
#  git checkout $MAIN_BEANCH_PATH
#  git pull --rebase
#
#  if [ -n "$(git merge BRANCH_PROJECT)" ]; then
#    echo "存在冲突，请解决"
#    exit 1
#  else
#    hitPack
#  fi
#}
#
#
### 解决冲突之后，重新运行当前脚本
#mainBranchCommit(){
#  if [ -n "$(git merge BRANCH_PROJECT)" ]; then
#    git add .
#    git commit
#    git push origin $MAIN_BEANCH_PATH
#    hitPack
#  fi
#}
#
#
#if [ "$BRANCH_PROJECT" = "$MAIN_BEANCH_PATH" ]; then
#  if [ -n "$(git status --porcelain)" ]; then
#    mainBranchCommit
#  else
#    hitPack
#  fi
#  exit 1
#else
#  echo "当前分支： $BRANCH_PROJECT";
#  isCommit
#fi











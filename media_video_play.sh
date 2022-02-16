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

## 解决冲突处理
mergeConflictSolve(){
  echo "当前代码有冲突，请现解决冲突，冲突文件为："
  git ls-files -u | cut -f 2 | sort -u

  read -p "解决冲突后按 Y，现在退出，N？ Y｜N" merge_params

  git add .
  git commit
  git pull --rebase
  if [ -n "$(git status --porcelain)" ]; then
    mergeConflictSolve
  fi
}

# push代码到当前分支
addCodePushOrigin(){
  git add .
  read -p "请输入提交名称: " commit_params
  if [ "${commit_params}" == "" ]; then
    addCodePushOrigin
  else
    git commit -m "${commit_params}"
  fi

  git pull --rebase

  if [ -n "$(git status --porcelain)" ]; then
    mergeConflictSolve
  fi

  git push origin "$(git rev-parse --abbrev-ref HEAD)"

}

# 对未提交代码进行处理
statusCodeOperation(){
  read -p "当前有未提交代码，1, 继续提交， 2，暂存，  3， 舍弃  ？1|2｜3 : " operation_params
  if [ ${operation_params} == "1" ]; then
    addCodePushOrigin
  elif [ ${operation_params} == "2" ]; then
    git stash
  else

  fi
}

# 判断是否有未提交代码
isStatus(){
  if [ -n "$(git status --porcelain)" ]; then
    statusCodeOperation
  else
      echo "已全部提交";
      checkoutMain
  fi
}







BRANCH_PROJECT="$(git rev-parse --abbrev-ref HEAD)"




mainBranch(){
  if [ "${MAIN_BRANCH_PATH}" == "${BRANCH_PROJECT}" ]; then
    isStatus




  else




  fi
  
  
  
  
}







# 主分枝判断   在   直接处理     不在  查看是否未提交    然后切换到主分枝    然后更新数据   pull

# 输入合并分支 




# 首先判断是否输入主分枝， 输入主分枝  判断当前分支是否在主分枝， 在主分枝   判断是否还有未提交代码   如果有就提交，或者舍弃， 或者暂存，  如果没有就询问是否 直接打包
# 在输入主分枝的情况下

#

# 首先判断是否输入开发分支名称， 如果输入   则判断当前是否当前分支， 在开发分支之后， 判断是否还有未提交代码   如果有就提交，或者舍弃， 或者暂存，  如果没有就询问是否 直接打包

#如果当前分支不在开发分支， 现判断是否有未提交代码， 如果有   要么提交，  要么舍弃  要不暂存    如果没有    则直接切换分支，   并询问是否打包


# 是否输入主分枝，



#if [  ]; then
#
#fi



read -p "是否操作当前分支 ${BRANCH_PROJECT} ？Y|N : " add_params
if [[ $add_params == "Y" || $add_params == "y" ]]; then

else
  #请


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











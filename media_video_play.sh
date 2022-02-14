#!/bin/bash

if [ -n "$(git status --porcelain)" ]; then
  echo "还有未提交的，请现处理";
else
    echo "已全部提交";
fi






#BRANCH_PROJECT=""
#
#git checkout master
#
#git pull --rebase
#
#set -e
#info(){
#    echo "./media_video_play.sh [-b] branchName [v] version"
#    echo "b 指定从那个分支merge到主分枝，"
#    echo "v 版本号，不需要输入，默认版本号规则为 xxxSDK.20220214181520"
#}
#
## 获取输入参数
#while getopts "b:v:" opt; do
#    echo " ${opt}    ${OPTARG}  "
#  case ${opt} in
#    v)
#      NEWVERSION=${OPTARG}
#      ;;
#    b)
#      BRANCH_PROJECT=${OPTARG}
#      ;;
#    ?)
#      echo "error: 无效参数"
#      info
#      exit 1
#  esac
#done
#
#if [ $BRANCH_PROJECT -ne "" ]; then
#  git merge $BRANCH_PROJECT



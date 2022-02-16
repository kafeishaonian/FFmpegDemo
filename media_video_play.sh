#!/bin/bash

MODULES_NAME=""
DEV_BRANCH_NAME=""
MAIN_BRANCH_PATH=""

MODULES_FIRST_NAME=""
MODULES_LAST_NAME=""


set -e
info(){
    echo "./media_video_play.sh [-m] modules [-d] development branch [-b] main branch"
    echo "m 指定modules, 对应:modules:xxx"
    echo "d 指定开发分支， 指定当前是在那个开发分支进行开发，用于开发分支打包"
    echo "b 主分枝， merge时开发分支与主分枝都必填，merge后产生冲突，解决冲突之后，可只填主分枝"
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
  if [[ $add_params == "Y" || $add_params == "y" ]]; then
    git add .
    git commit
    git pull --rebase
    if [ -n "$(git status --porcelain)" ]; then
      mergeConflictSolve
    fi
  else
    exit 1
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
  if [ "${operation_params}" == "1" ]; then
    addCodePushOrigin
  elif [ "${operation_params}" == "2" ]; then
    git stash
  else
    git checkout .
  fi
}

# 判断是否有未提交代码
isStatus(){
  if [ -n "$(git status --porcelain)" ]; then
    statusCodeOperation
  fi
}

#打包
hitPack(){
  cd ../..
  ./gradlew "${MODULES_NAME}":clean "${MODULES_NAME}":uploadArchives
}

#合并分支
mergeDevelopmentBranch(){
  git merge "${DEV_BRANCH_NAME}"
  if [ -n "$(git status --porcelain)" ]; then
    mergeConflictSolve
  fi
}


gitTagAndLog(){
  if [ "${MAIN_BRANCH_PATH}" == "${BRANCH_PROJECT}" ]; then
    #记录编译时间，版本号， commitid， 信息至文件，方便查询
    BUILD_INFO="对 ${MODULES_NAME} 发布版本，版本号详见tag"
    echo 'back git commit'
    set -x
    echo -e '\n' >> "${MODULES_FIRST_NAME}"/"${MODULES_LAST_NAME}"/"${MODULES_LAST_NAME}_log.txt"
    echo "$BUILD_INFO" >> "${MODULES_FIRST_NAME}"/"${MODULES_LAST_NAME}"/"${MODULES_LAST_NAME}_log.txt"
    echo "$current_date_time" >> "${MODULES_FIRST_NAME}"/"${MODULES_LAST_NAME}"/"${MODULES_LAST_NAME}_log.txt"
    git rev-parse --abbrev-ref HEAD >> "${MODULES_FIRST_NAME}"/"${MODULES_LAST_NAME}"/"${MODULES_LAST_NAME}_log.txt"
    git rev-parse HEAD >> "${MODULES_FIRST_NAME}"/"${MODULES_LAST_NAME}"/"${MODULES_LAST_NAME}_log.txt"
    git log --pretty=format:'%D %ci%n' -1 >> "${MODULES_FIRST_NAME}"/"${MODULES_LAST_NAME}"/"${MODULES_LAST_NAME}_log.txt"
  fi
}



BRANCH_PROJECT="$(git rev-parse --abbrev-ref HEAD)"

mainBranch(){
  # 主分支与当前分支一致
  if [ "${MAIN_BRANCH_PATH}" == "${BRANCH_PROJECT}" ]; then
    isStatus
    if [ "${DEV_BRANCH_NAME}" != "" ]; then
      git checkout "${DEV_BRANCH_NAME}"
      git pull --rebase
      git checkout "${MAIN_BRANCH_PATH}"
      mergeDevelopmentBranch
    fi
    hitPack
  elif [ "${DEV_BRANCH_NAME}" == "${BRANCH_PROJECT}" ]; then
  #当前分支与主分枝相同
    isStatus
    if [ "${MAIN_BRANCH_PATH}" != "" ]; then
      git checkout "${MAIN_BRANCH_PATH}"
      mergeDevelopmentBranch
    fi
    hitPack
  elif [[ "${MAIN_BRANCH_PATH}" == "" || "${DEV_BRANCH_NAME}" == "" ]]; then
  # 主分支与合并分支都为空时, 直接提交代码并打包
    isStatus
    hitPack
  fi
  gitTagAndLog
}

mainBranch










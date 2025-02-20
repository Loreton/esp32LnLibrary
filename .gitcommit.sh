#!/bin/bash
#
# updated by ...: Loreto Notarantonio
# Date .........: 20-02-2025 18.04.51
# Updates:
#
# #########################################################


####################à
# Colors:
# Black        0;30     Dark Gray     1;30
# Red          0;31     Light Red     1;31
# Green        0;32     Light Green   1;32
# Brown/Orange 0;33     Yellow        1;33
# Blue         0;34     Light Blue    1;34
# Purple       0;35     Light Purple  1;35
# Cyan         0;36     Light Cyan    1;36
# Light Gray   0;37     White         1;37
redH='\033[1;31m'
cyanH='\033[1;36m'
yellowH='\033[1;33m'
purpleH='\033[1;35m'
colorReset='\033[0m' # No Color


# ######### M A I N #########
# ######### M A I N #########
# ######### M A I N #########
# ######### M A I N #########
# ######### M A I N #########
# ######### M A I N #########
    args=$@
    ESEGUI="echo"
    word='--push';  [[ " $args " == *" $word "* ]] && args=${args//$word/} && g_PUSH=1
    word='--go';    [[ " $args " == *" $word "* ]] && args=${args//$word/} && ESEGUI=""
    # comment=$(echo $args) # ciò che resta
    comment="${args:-update}" # ciò che resta con default == update
    scriptFullPath="$(readlink -f ${BASH_SOURCE[0]})"       # OTTIMA

    q='"'
    ${ESEGUI} git add --all
    ${ESEGUI} git commit -a -m "${q}${comment}${q}"
    if [[ "$g_PUSH" == "1" ]]; then
        ${ESEGUI} git push
    fi
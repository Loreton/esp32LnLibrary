#
# updated by ...: Loreto Notarantonio
# Date .........: 06-06-2025 06.31.10
#


import sys; sys.dont_write_bytecode=True
import os
from enum import Enum
from SCons.Script import Import

Import("env")

# ==============================================
# - funzione utile per usarla nei display....
# - ref https://tldp.org/HOWTO/Bash-Prompt-HOWTO/x329.html
# ==============================================
def getColors():
    from types import SimpleNamespace
    colors=SimpleNamespace(
        red        = '\033[0;31m',
        redH       = '\033[1;31m',
        green      = '\033[0;32m',
        greenH     = '\033[1;32m',
        yellow     = '\033[0;33m',
        yellowH    = '\033[1;33m',
        blue       = '\033[0;34m',
        blueH      = '\033[1;34m',
        purple     = '\033[0;35m',
        purpleH    = '\033[1;35m',
        cyan       = '\033[0;36m',
        cyanH      = '\033[1;36m',
        gray       = '\033[0;37m',
        white      = '\033[1;37m',
        colorReset = '\033[0m',
    )
    return colors

#####################################################
# variabili (#define) del progetto
#####################################################
class ENVAR(Enum):
    ESP32_WROOM_32E_MODULE        = 10
    ESP32_WROOM_32E_2RELAY_MODULE = 20
    DEVEL                         = 30
    PRODUCTION                    = 40
    LN_RELEASE_TYPE               = 50 # will be DEVEL ! PRODUCTION
    LN_ESP32_BOARD_TYPE           = 60 # will be ESP32_WROOM_32E_MODULE | ESP32_WROOM_32E_2RELAY_MODULE

    @staticmethod
    def printAll():
        for item in ENVAR:
            print(f"{C.yellow}{item.name:30} : {C.yellowH}{item.value}{C.colorReset}")


#####################################################
# post_program actions ... ma non lo uso
# ...sono qui solo come sample nel caso...
#####################################################
def post_program_action(source, target, env):
    print("Program has been built!")
    program_path = target[0].get_abspath()
    print("Program path", program_path)
    # Use case: sign a firmware, do any manipulations with ELF, etc
    # env.Execute(f"sign --elf {program_path}")

env.AddPostAction("$PROGPATH", post_program_action)

#####################################################
# Upload actions ... ma non le uso
# ...sono qui solo come sample nel caso...
#####################################################
def before_upload(source, target, env):
    print(f"{C.yellow}before_upload{C.colorReset}")
    print(f"{C.yellowH}source: {source}{C.colorReset}")
    print(f"{C.yellowH}target: {target}{C.colorReset}")
    print(f"{C.yellowH}env:    {env}{C.colorReset}")
    # call Node.JS or other script
    # env.Execute("node --version")
    # do some actions

def after_upload(source, target, env):
    print(f"{C.yellow}after_upload{C.colorReset}")
    print(f"{C.yellowH}source: {source}{C.colorReset}")
    print(f"{C.yellowH}target: {target}{C.colorReset}")
    print(f"{C.yellowH}env:    {env}{C.colorReset}")
    # do some actions

env.AddPreAction("upload", before_upload)
env.AddPostAction("upload", after_upload)


#####################################################
# controlla se la variabile è definita in envar
# altrimenti assegna il valore default_value
#####################################################
def setPlatformioVar(var_name, value):
    fExists=False
    if "CPPDEFINES" in env:
        my_vars = env['CPPDEFINES']
        for name, _val in my_vars:
            if name == var_name:
                fExists=True


    if fExists:
        print(f"{TAB}{C.yellow}replacing {C.yellowH}{var_name:35}: {C.greenH}{value}")
        # env.Replace(CPPDEFINES=[(var_name, value)]) ### cancella tutto CPDEFINES
        env.ProcessUnFlags(f"-D{var_name}") ### remove it
        env.Append(CPPDEFINES=[(var_name, value)]) # or  env.Append(CPPDEFINES=(var_name, value)) ????
    else:
        print(f"{TAB}{C.yellow}setting   {C.yellowH}{var_name:35}: {C.greenH}{value}")
        env.Append(CPPDEFINES=[(var_name, value)])


#####################################################
# controlla se la variabile è definita in envar
# altrimenti assegna il valore passato
#####################################################
def checkEnvar(var_name, default_value=None):
    value = os.getenv(var_name, default_value)  # Default_value se non definito
    if value:
        setPlatformioVar(var_name, value)
    else:
        print(f"{TAB}{C.redH}variable {var_name:35} NOT defined")



#####################################################
# convert value to ENVAR name for display scope
#####################################################
USE_MAIN=True
TAB=' '*4
# if _*4_name__ == '__main__': NON FUNZIONA
if USE_MAIN:
    rel_level = os.getenv("REL_LEVEL", "DEVEL")  # Default_value se non definito
    C=getColors()
    print()
    print(f"{C.purpleH}-"*60, C.colorReset)
    print(f"{TAB}{C.greenH}Current CLI targets", COMMAND_LINE_TARGETS, C.colorReset)
    print(f"{TAB}{C.greenH}Current Build targets", BUILD_TARGETS, C.colorReset)
    print(f"{TAB}{C.greenH}ciao" , sys.argv, C.colorReset)


    ### - assegna le valiabili all'ambiente platformio
    for item in ENVAR:
        checkEnvar(item.name, item.value)

    if rel_level == 'prod':
        setPlatformioVar("LN_RELEASE_TYPE", ENVAR.PRODUCTION.value)
        setPlatformioVar("LN_ESP32_BOARD_TYPE", ENVAR.ESP32_WROOM_32E_2RELAY_MODULE.value)
    else:
        setPlatformioVar("LN_RELEASE_TYPE", ENVAR.DEVEL.value)
        setPlatformioVar("LN_ESP32_BOARD_TYPE", ENVAR.ESP32_WROOM_32E_MODULE.value)


    print()
    my_vars = env['CPPDEFINES']
    for name, value in my_vars:
        if name in ["LN_RELEASE_TYPE", "LN_ESP32_BOARD_TYPE"]:
            value = ENVAR(value).name
        print(f"{TAB}{C.yellow}{name:35}: {C.yellowH}{value}{C.colorReset}")


    print()
    # print("press ENTER to continue, [q|x] to exit")
    choice = input("press ENTER to continue, [q|x] to exit [d] dumpVars\n").strip()
    if choice in ["x", "q"]:
        sys.exit(1)
    elif choice in ["d"]:
        # Dump global construction environment (for debug purpose)
        # print(env.Dump())

        # Dump project construction environment (for debug purpose)
        # print(projenv.Dump())
        print( env['CPPDEFINES'])
        xx = env['CPPDEFINES']
        for name, value in xx:
            print(f"{TAB}{C.yellow}{name:35}: {C.yellowH}{value}{C.colorReset}")

        sys.exit(1)



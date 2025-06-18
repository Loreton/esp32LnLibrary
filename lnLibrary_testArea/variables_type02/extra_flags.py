#
# updated by ...: Loreto Notarantonio
# Date .........: 12-06-2025 07.56.59
#


import sys; sys.dont_write_bytecode=True
import os
from enum import Enum
from SCons.Script import Import


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


def callerID(stacklevel=1):
    """
    :return: name of caller
    """
    func_name = sys._getframe(stacklevel).f_code.co_name
    line_no = sys._getframe(stacklevel).f_lineno
    return f"{func_name}: {line_no}"

"""
:return: name of caller
"""
def writeConsole(text, stacklevel=1):
    func_name = sys._getframe(stacklevel).f_code.co_name
    line_no   = sys._getframe(stacklevel).f_lineno
    print(f"[{func_name}: {line_no}]: {text}")


#####################################################
# input: env var "REL_LEVEL:PROD|DEVEL"
#####################################################
def keyPrompt(msg, continue_char=["c"], quit_char=["x", "q"]):
    # "press [c] to continue, [q] to quit [d] dumpVars\n").strip()
    msg=f"{msg} press {continue_char} to continue, {quit_char} to quit [d] dumpVars\n"

    while True:
        choice = input(f"{msg}\n").strip()

        if choice in continue_char:
            break

        elif choice in ["d"]:
            # Dump global construction environment (for debug purpose)
            print(env.Dump())

            # Dump project construction environment (for debug purpose)
            # print(projenv.Dump())

            for name, value in scanCPPDEFINES():
                print(f"{TAB}{C.yellow}{name:35}: {C.yellowH}{value}{C.colorReset}")



        elif choice in quit_char:
            print(f"exiting...")
            sys.exit(1)




#####################################################
# post_program actions ... ma non lo uso
# ...sono qui solo come sample nel caso...
#####################################################
def post_program_action(source, target, env):
    print("Program has been built!")
    program_path = target[0].get_abspath()
    print("Program path", program_path)
    # keyPrompt(msg="[AFTER_COMPILATION]", continue_char=["c"], quit_char=["x", "q"])
    # Use case: sign a firmware, do any manipulations with ELF, etc
    # env.Execute(f"sign --elf {program_path}")


#####################################################
# Upload actions ... ma non le uso
# ...sono qui solo come sample nel caso...
#####################################################
def before_upload(source, target, env):
    print(f"{C.yellow}before_upload{C.colorReset}")
    print(f"{C.yellowH}source: {source}{C.colorReset}")
    print(f"{C.yellowH}target: {target}{C.colorReset}")
    print(f"{C.yellowH}env:    {env}{C.colorReset}")
    keyPrompt(msg='[BEFORE_UPLOAD]', continue_char=["c"], quit_char=["x", "q"])
    # call Node.JS or other script
    # env.Execute("node --version")
    # do some actions

def after_upload(source, target, env):
    print(f"{C.yellow}after_upload{C.colorReset}")
    print(f"{C.yellowH}source: {source}{C.colorReset}")
    print(f"{C.yellowH}target: {target}{C.colorReset}")
    print(f"{C.yellowH}env:    {env}{C.colorReset}")
    # do some actions




def scanCPPDEFINES():
    if "CPPDEFINES" in env:
        my_vars = env['CPPDEFINES'] ### [(var1, val), var2, (var3, val), var4, ... ]
        for item in my_vars:
            if isinstance(item , tuple):
                yield item
            else:
                yield (item, None)


def listCPPDEFINES():
    cppdefines_vars = []
    if "CPPDEFINES" in env:
        my_vars = env['CPPDEFINES'] ### [(var1, val), var2, (var3, val), var4, ... ]
        for item in my_vars:
            if isinstance(item , tuple):
                entry = item
            else:
                entry = (item, None)
            cppdefines_vars.append(entry)
    for entry in cppdefines_vars:
        writeConsole(entry)
    return cppdefines_vars



#####################################################
# controlla se la variabile è definita in envar
# altrimenti assegna il valore default_value
# NON FUNZIONA SU pressControl . non so perché
#####################################################
def set_BUILD_FLAGS(var_name, value):
    new_entry=f"{var_name}={value}"
    for entry in env["BUILD_FLAGS"]:
        if new_entry in entry:
            writeConsole(f"{entry} already present")
            return

    writeConsole(f"{C.yellow}adding {C.yellowH}{var_name:35}: {C.greenH}{value}")
    env.Append(BUILD_FLAGS = [f"-D{var_name}={value}" ])

#####################################################
# controlla se la variabile è definita in envar
# altrimenti assegna il valore default_value
# NON FUNZIONA SU pressControl . non so perché
#####################################################
def set_UPLOADERFLAGS(var_name, value):
    # print(f"{caller}{C.yellow}adding {C.yellowH}{var_name:35}: {C.greenH}{value}")
    writeConsole(f"{C.yellow}adding {C.yellowH}{var_name:35}: {C.greenH}{value}")
    env.Append(UPLOADERFLAGS = ["--auth", pw ])



#####################################################
# controlla se la variabile è definita in envar
# altrimenti assegna il valore default_value
# NON FUNZIONA SU pressControl . non so perché
#####################################################
def set_CPP_DEFINES(var_name, value):
    caller = f"{TAB}[{callerID()}]: "
    for name, cur_val in scanCPPDEFINES():
        if name == var_name:
            writeConsole(f"current: {name}: {cur_val}")
            if cur_val != value:
                import pdb; pdb.set_trace() # by Loreto
                env.ProcessUnFlags(f"-D{var_name}=''") ### remove it
            break

    writeConsole(f"{C.yellow}setting   {C.yellowH}{var_name:35}: {C.greenH}{value}")
    env.Append(CPPDEFINES=[(var_name, value)])



#####################################################
# prendiamo tutte le variabili che iniziano con 'ln_esp32''
# e le impostiamo nelle build_flags
#####################################################
def checkEnvar():
    for name, value in os.environ.items():
        if name.startswith("ln_esp32_"):
            # print(f"{TAB}{C.yellow}{name}: {C.yellowH}{value}")
            set_CPP_DEFINES(name, value) ### non funziona in pressControl ????









def main(Env):
    global C, envxxx
    envxxx = Env
    rel_level = os.getenv("REL_LEVEL", "devel")  # Default_value se non definito
    C=getColors()
    print()
    print(f"{C.purpleH}-"*60, C.colorReset)
    print(f"{TAB}{C.greenH}Current CLI targets", COMMAND_LINE_TARGETS, C.colorReset)
    print(f"{TAB}{C.greenH}Current Build targets", BUILD_TARGETS, C.colorReset)
    print(f"{TAB}{C.greenH}ciao" , sys.argv, C.colorReset)


    ### - assegna le valiabili all'ambiente platformio
    checkEnvar()
    listCPPDEFINES()
    for item in env["BUILD_FLAGS"]:
        print(f"{TAB}{C.greenH}{item}", C.colorReset)

        # value = os.getenv(item.name)
        # print(f"var: {item.name}: {value}")

    # if rel_level.lower() == 'prod':
    #     set_CPP_DEFINES("ln_RELEASE_TYPE", ENVAR.ln_PRODUCTION.value)
    #     set_CPP_DEFINES("ln_ESP32_BOARD_TYPE", ENVAR.ln_ESP32_WROOM_32E_MODULE_2RELAY.value)
    # else:
    #     set_CPP_DEFINES("ln_RELEASE_TYPE", ENVAR.ln_DEVEL.value)
    #     set_CPP_DEFINES("ln_ESP32_BOARD_TYPE", ENVAR.ln_ESP32_WROOM_32E_MODULE.value)


    # print()
    # for name, value in scanCPPDEFINES():
    #     if name in ["ln_RELEASE_TYPE", "ln_ESP32_BOARD_TYPE"]:
    #         # value = ENVAR(value).name
    #         pass
    #     print(f"{TAB}{C.yellow}{name:35}: {C.yellowH}{value}{C.colorReset}")


    print()

#####################################################
# input: env var "REL_LEVEL:PROD|DEVEL"
#####################################################
USE_MAIN=True

if USE_MAIN: # if _*4_name__ == '__main__': NON FUNZIONA
    TAB=' '*4
    Import("env")

    main(env)

    # env = DefaultEnvironment()
    # main(env)

    print(f"{TAB}{C.greenH}Current CLI targets", COMMAND_LINE_TARGETS, C.colorReset)
    keyPrompt(msg="[BEFORE COMPILE]", continue_char=["c"], quit_char=["x", "q"])


    # env.AddPostAction("$PROGPATH", post_program_action)
    # env.AddPreAction("upload", before_upload)
    # env.AddPostAction("upload", after_upload)
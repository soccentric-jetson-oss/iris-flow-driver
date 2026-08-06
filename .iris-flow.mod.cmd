savedcmd_iris-flow.mod := printf '%s\n'   src/iris_flow_main.o | awk '!x[$$0]++ { print("./"$$0) }' > iris-flow.mod

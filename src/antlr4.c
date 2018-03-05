
#include <corto/platform.h>

int cortomain(int argc, char *argv[]) {
    char **args = malloc((argc + 5) * sizeof(char*));

    /* Add default arguments to run antlr4 tool */
    args[0] = "java";
    args[1] = "-Xmx500M";
    args[2] = "-cp";
    args[3] = corto_envparse("$BAKE_TARGET/java/antlr-4.7.1-complete.jar:$CLASSPATH");
    args[4] = "org.antlr.v4.Tool";

    memcpy(&args[5], &argv[1], (argc - 1) * sizeof(char*));
    args[argc + 5] = NULL;

    corto_proc pid = corto_proc_run("java", args);
    if (!pid) {
        corto_throw("failed to invoke antlr tool");
        goto error;
    }

    int8_t ret = 0;
    int sig = corto_proc_wait(pid, &ret);
    if (sig) {
        corto_throw("antlr4 tool exited with signal %d", sig);
        goto error;
    } else if (ret) {
        corto_throw("antlr4 tool exited with code %d", ret);
        goto error;
    }

    return 0;
error:
    return -1;
}

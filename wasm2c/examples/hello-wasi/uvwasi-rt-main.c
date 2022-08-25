/* Link this with wasm2c output and uvwasi runtime to build a standalone app */
#include <stdio.h>
#include <stdlib.h>
#include "uvwasi.h"
#include "hello.wasm.h"

extern uvwasi_t uvwasi;

Z_hello_instance_t global_instance;

/*
#define MODULE_NAME hello

#define module_init() Z_## MODULE_NAME ##_init_module()
#define module_instantiate(instance_p, wasi_p)  Z_ ##MODULE_NAME## _instantiate_module(instance_p, wasi_p)
#define module_free(instance_p) Z_##MODULE_NAME##_free(instance_p)
*/

int main(int argc, const char** argv)
{
    #define ENV_COUNT       7
    #define PREOPENS_COUNT  2

    char* env[ENV_COUNT];
    env[0] = "TERM=xterm-256color";
    env[1] = "COLORTERM=truecolor";
    env[2] = "LANG=en_US.UTF-8";
    env[3] = "PWD=/";
    env[4] = "HOME=/";
    env[5] = "PATH=/";
    env[6] = NULL;

    uvwasi_preopen_t preopens[PREOPENS_COUNT];

    //No sandboxing is enforced, binary has access to everything user does
    preopens[0].mapped_path = "/";
    preopens[0].real_path = "/";
    preopens[1].mapped_path = "./";
    preopens[1].real_path = ".";

    uvwasi_options_t init_options;

    init_options.argc = argc;
    init_options.argv = argv;
    init_options.envp = (const char **) env;
    init_options.preopenc = PREOPENS_COUNT;
    init_options.preopens = preopens;

    wasm_rt_init();
    uvwasi_errno_t ret = uvwasi_init(&uvwasi, &init_options);

    if (ret != UVWASI_ESUCCESS) {
        printf("uvwasi_init failed with error %d\n", ret);
        exit(1);
    }

    Z_hello_init_module();
    Z_hello_instantiate(&global_instance, NULL);
    Z_helloZ__start(&global_instance);
    Z_hello_free(&global_instance);
    uvwasi_destroy(&uvwasi);

    wasm_rt_free();

    return 0;
}

/* Link this with wasm2c output and uvwasi runtime to build a standalone app */
#include <stdio.h>
#include <stdlib.h>
#include "uvwasi.h"
#include "hello.wasm.h"

uvwasi_t uvwasi;

Z_hello_instance_t global_instance;

/*
#define MODULE_NAME hello

#define module_init() Z_## MODULE_NAME ##_init_module()
#define module_instantiate(instance_p, wasi_p)  Z_ ##MODULE_NAME## _instantiate_module(instance_p, wasi_p)
#define module_free(instance_p) Z_##MODULE_NAME##_free(instance_p)
*/

int main(int argc, const char** argv)
{

    uvwasi_options_t init_options;

    //pass in standard descriptors
    init_options.in = 0;
    init_options.out = 1;
    init_options.err = 2;
    init_options.fd_table_size = 3;

    //pass in args and environement
    extern const char ** environ;
    init_options.argc = argc;
    init_options.argv = argv;
    init_options.envp = (const char **) environ;

    //no sandboxing enforced, binary has access to everything user does
    init_options.preopenc = 2;
    init_options.preopens = calloc(2, sizeof(uvwasi_preopen_t));

    init_options.preopens[0].mapped_path = "/";
    init_options.preopens[0].real_path = "/";
    init_options.preopens[1].mapped_path = "./";
    init_options.preopens[1].real_path = ".";

    init_options.allocator = NULL;

    wasm_rt_init();
    uvwasi_errno_t ret = uvwasi_init(&uvwasi, &init_options);

    if (ret != UVWASI_ESUCCESS) {
        printf("uvwasi_init failed with error %d\n", ret);
        exit(1);
    }

    Z_hello_init_module();
    Z_hello_instantiate(&global_instance,(struct Z_wasi_snapshot_preview1_instance_t *) &uvwasi);
    Z_helloZ__start(&global_instance);
    Z_hello_free(&global_instance);
    uvwasi_destroy(&uvwasi);

    wasm_rt_free();

    return 0;
}

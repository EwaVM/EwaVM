#pragma once

#include <stdint.h>

namespace EwaVM
{

    /* Low Level API , about loading and compiling wasm module.*/

    /* return error message if any, or NULL if succeded. */
    struct ewa_symbol_resolve_request
    {
        char *import_module;
        char *import_field;
        uint32_t kind;
        void *result;
    };
    struct ewa_symbol_resolver
    {
        void (*resolve)(struct ewa_symbol_resolver *_this, struct ewa_symbol_resolve_request *req);
    };

    typedef void *ewa_module_compiler;

    typedef void *ewa_module_state;

    /* webassembly function definition (for 1.0 version) */

    typedef void *ewa_wasm_function;

    typedef void (*ewa_host_function_c)(void *stack_frame);

/* symbol type */
#define SYMBOL_KIND_FUNCTION 0
#define SYMBOL_KIND_TABLE 1
#define SYMBOL_KIND_MEMORY 2
#define SYMBOL_KIND_GLOBAL 3

    /* module load config */

    /* These config are related to ABI of the generated code, so keep same config to load module at same time in one process. */
    struct ewa_global_compile_config
    {
        /* ewa_STACK_FLAGS_xxx flags, indicate how operand stored in stack. */
        char stack_flags;
        /* ewa_MISC_FLAGS_xxx flags */
        char misc_flags;
    };

/* If set, elements on stack will align to it's size, and function frame base will align to 8.
Some arch require this flag to avoid align error. */
#define STACK_FLAGS_AUTO_ALIGN 1

/* If set, 32bit index will be always extended to 64bit on 64bit host on loading/storing.
Some processor, like risc-v64, extend sign bit , require this flag to use index larger than 2GB.
In most case, this flag is not necessary and may slow the generated code.
This flag has no effect for "native_memory" access. */
#define MISC_FLAGS_EXTEND_INDEX 1

/* If set. Local variables, if used before initialized.  will be set to 0 when entering a function.
ewa use a simple glance instead of SSA analyzation, So this flag may slow the generated code.
But according to the WebAssembly spec, this flag is SET by default  */
#define MISC_FLAGS_LOCALS_ZERO_INIT 2

    extern int ewa_get_version();

    struct ewa_wasm_table
    {
        uint8_t elem_type; /* type of entries (only FUNC in MVP) */
        uint32_t initial;  /* initial table size */
        uint32_t maximum;  /* maximum table size */
        uint32_t size;     /* current table size */
        void **entries;
    };
    struct ewa_wasm_memory
    {
        uint32_t initial; /* initial size (64K pages) */
        uint32_t maximum; /* maximum size (64K pages) */
        uint32_t pages;   /* current size (64K pages) */

        /*  memory area, if 0(NULL), indicate this memory is mapped to native host memory directly(native_memory), and the ewa will generated faster code to access this memory.
            In this case, memory can only be allocated by call ewa_builtin.memory_alloc, and index type should match the machine word size.
            wasm module can use this memory by import memory ewa_builtin.native_memory, see "ewa builtin symbols list" for detail.
        */
        uint8_t *bytes;

        /*  fixed memory, if set, memory base will never change, and memory.grow to this memory always return -1.
            set this flags make generated code faster, by avoid memory base reloading.
            by default, ewa set fixed to true if wasm memory maximum size is set.(allocate to maximum),
            and false if no maximum provided */
        uint8_t fixed;
    };

    /*  ewa_load_module load a module from data, if succeeded, return compiled module, if failed, return NULL and set err_msg, if err_msg is not NULL.
        It invoke "ewa_new_module_compiler","ewa_compile","ewa_get_module_state","ewa_free_module_compiler" internally. */
    extern ewa_module_state *ewa_load_module(char *data, int len, char **err_msg);

    extern ewa_module_compiler ewa_new_module_compiler();

    /*  free compile infomation, have no effect to ewa_module_state and generated code.
        return error message if any, or NULL if succeded. */
    extern char *ewa_free_module_compiler(ewa_module_compiler mod);

    extern char *ewa_set_symbol_resolver(ewa_module_compiler m, struct ewa_symbol_resolver *resolver);

    /*  compile module and generate code. if cfg is NULL, use default compile config.
        return error message if any, or NULL if succeded. */
    extern char *ewa_compile(ewa_module_compiler m, char *data, int len);

    /*  return wasm start function, or NULL if no start function specified.
        you should call the start function to init module, according to WebAssembly spec.
        Though it's may not necessary for ewa. */
    extern ewa_wasm_function ewa_get_start_function(ewa_module_state m);

    /*  return error message if any, or NULL if succeded. */
    extern char *ewa_set_global_compile_config(struct ewa_global_compile_config *config);
    extern char *ewa_get_global_compile_config(struct ewa_global_compile_config *config);

    extern ewa_module_state ewa_get_module_state(ewa_module_compiler m);

    /*  free module state and generated code.
        return error message if any, or NULL if succeded.
        only need free if ewa_compile succeeded. */
    extern char *ewa_free_module_state(ewa_module_state rc);

    /*  The symbol resolver of ewa_module_state will be set to the same as compiler use by default,
        and can be changed by this function */
    extern char *ewa_set_state_symbol_resolver(ewa_module_state rc, struct ewa_symbol_resolver *resolver);

    /* get wasm function exported by wasm module runtime. */
    extern ewa_wasm_function ewa_get_export_function(ewa_module_state rc, char *name);

    /* get wasm memory exported by wasm module runtime. */
    extern struct ewa_wasm_memory *ewa_get_export_memory(ewa_module_state rc, char *name);

    /* get wasm table exported by wasm module runtime. */
    extern struct ewa_wasm_table *ewa_get_export_table(ewa_module_state rc, char *name);

    /* get wasm globals exported by wasm module runtime. */
    extern void *ewa_get_export_global(ewa_module_state rc, char *name);

    /* ewa_module_state inspect result */
    struct ewa_inspect_result1
    {
        /* memory 0 size , in byte */
        int memory_size;
        void *memory;
        /* table 0 entries count, in void* */
        int table_entries_count;
        void **table_entries;
        /* global buffer size, in byte */
        int globals_buffer_size;
        void *globals_buffer;
        struct ewa_symbol_resolver *symbol_resolver;
    };
    /*  return error message if any, or NULL if succeded. */
    extern char *ewa_inspect_module_state(ewa_module_state c, struct ewa_inspect_result1 *result);

    extern void ewa_module_state_set_user_data(ewa_module_state c, void *ud);
    extern void *ewa_module_state_get_user_data(ewa_module_state c);

    /*  return error message if any, or NULL if succeded. */
    extern char *ewa_free_module_compiler(ewa_module_compiler mod);

    /* ewa invoke and runtime stack helper */

    /* allocate a stack buffer, with align 16. can only be free by ewa_free_stack. */
    extern void *ewa_allocate_stack(int size);
    extern void ewa_free_stack(void *stack);

    /* wrap host function to wasm function, must be free by ewa_free_wrapped_function. */
    extern ewa_wasm_function ewa_wrap_host_function_c(ewa_host_function_c host_func);

    extern void ewa_free_wrapped_function(ewa_wasm_function wrapped);

    extern void ewa_call_wasm_function(ewa_wasm_function fn, void *stack_pointer);

    /* For version 1.0, arguments and results are all stored on stack. */
    /* stack_flags has effect on these function. */

    /* put value to *sp, move *sp to next entries. */
    extern void ewa_rstack_put_i32(void **sp, int val);
    extern void ewa_rstack_put_i64(void **sp, long long val);
    extern void ewa_rstack_put_f32(void **sp, float val);
    extern void ewa_rstack_put_f64(void **sp, double val);
    extern void ewa_rstack_put_ref(void **sp, void *val);
    /* get value on *sp, move *sp to next entries. */
    extern int ewa_rstack_get_i32(void **sp);
    extern long long ewa_rstack_get_i64(void **sp);
    extern float ewa_rstack_get_f32(void **sp);
    extern double ewa_rstack_get_f64(void **sp);
    extern void *ewa_rstack_get_ref(void **sp);

    struct ewa_named_symbol
    {
        const char *name;
        union
        {
            ewa_wasm_function fn;
            struct ewa_wasm_memory *mem;
            struct ewa_wasm_table *tb;
        } val;
        uint8_t kind;
    };
    /* get the ewa builtin symbol array. Builtin symbol can be use by import "ewa_builtin" module in wasm. */
    extern struct ewa_named_symbol *ewa_get_builtin_symbols(int *arr_size);

    /*
    ewa builtin symbols list

    ======== Format ============
    name
    type
    optional description
    ============================

    version
    func []->[i32]
    get ewa version.

    memory_alloc
    func [i32 size]->[i64]
    allocate memory with 'size', in byte, return i64 index on native_memory, for 32bit native_memory, the most significant 32bit word are 0.
    see also ewa_wasm_memory.bytes

    memory_free
    func [i64]->[]

    native_index_size
    func []->[i32]
    get the index size to access native memory, in byte (sizeof(void *)).

    load_module
    func [ref self_context,ref name,ref wasm_bytes,i32 length]->[ref err_msg]
    load module into same namespace, the caller module must been attached to a namespace.

    unload_module
    func [ref self_context,ref name]->[ref err_msg]

    import
    func [ref self_context,ref module_name,ref field_name,i32 kind] -> [ref]
    Only support SYMBOL_KIND_FUNCTION now

    get_self_runtime_context
    func []->[ref self_context]
    This is a STUB function can ONLY be called DIRECTLY by wasm.
    ewa will compile these 'call' instruction into native code, instead of a function call.

    ref_from_index
    func [const i32 memindex,i32|i64 index]->[ref]
    STUB function, get "pointer" reference that represent the address specified by the index of memory memindex.
    the memindex must be produced by i32.const xxx.

    ref_copy_bytes
    func [ref dst,ref src,i32 n]->[]
    copy bytes from 'src' to 'dst' for 'n' bytes, use "pointer" reference.

    ref_string_length
    func [ref str]->[i32 n]
    get the length of the null-terminal string, specified by "pointer" reference.

    ref_from_i64
    func [i64]->[ref]
    cast i64 to "pointer" reference.

    i64_from_ref
    func [ref]->[i64]
    cast "pointer" reference to i64.

    host_definition
    func [i32]->[ref]
    get string in host definition array (at index). the end of the array is NULL(ref.null).
    "host definition" is the string like the compiler definition "__linux__" , "_WIN32", "__arm__","_M_ARM=7","__ANDROID_API__=21", etc.

    stdio function

    fopen
    func [ref path,ref mode]->[ref file,i32 err]

    fread
    func [ref buf,i32 size,i32 count,ref file]->[i32 nread]
    ewa_wasm_function fread;

    fwrite
    func [ref buf,i32 size,i32 count,ref file]->[i32 nwrite]

    fclose
    func [ref file]->[i32 err]

    stdio
    func []->[ref stdin,ref stdout,ref stderr]

    native_memory
    memory
    special ewa_wasm_memory that map to the native host memory directly, memory->bytes is 0;
    */

    /* High level API, Namespace and Module */

    typedef void *ewa_namespace;

    struct ewa_host_module
    {
        void (*resolve)(struct ewa_host_module *_this, struct ewa_symbol_resolve_request *req);
        /*Will be set after added to namespace , suffixed 2 to avoid keyword conflict with c++*/
        ewa_namespace namespace2;
        /*If not null, call after added to namespace */
        void (*on_attached)(struct ewa_host_module *_this);
        /*If not null, call after remove from namespace, or namespace is deleted */
        void (*on_detached)(struct ewa_host_module *_this);
    };

#define MODULE_TYPE_HOST_MODULE 1
#define MODULE_TYPE_WASM_MODULE 2
/* Indicate module has been free. */
#define MODULE_TYPE_NULL 0;

    struct ewa_named_module
    {
        char *name;
        int type; /* MODULE_TYPE_xxxx */
        union
        {
            struct ewa_host_module *host;
            ewa_module_state wasm;
        } val;
    };

    ewa_namespace ewa_namespace_new();

    char *ewa_namespace_delete(ewa_namespace ns);

    char *ewa_namespace_remove_module(ewa_namespace ns, char *name);

    /* module structure will be copied into internal buffer, No need for caller to hold the memory */
    char *ewa_namespace_define_module(ewa_namespace ns, struct ewa_named_module *mod);

    ewa_module_state *ewa_namespace_define_wasm_module(ewa_namespace ns, char *name, char *wasm_bytes, int length, char **err_msg);

    struct ewa_named_module *ewa_namespace_find_module(ewa_namespace ns, char *name);

    struct ewa_symbol_resolver *ewa_namespace_resolver(ewa_namespace ns);

};
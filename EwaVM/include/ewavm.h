#pragma once

#include <stdint.h>

namespace EwaVM
{

    /* Low Level API , about loading and compiling wasm module.*/

    /* return error message if any, or NULL if succeded. */
    struct EwaSymbolResolveRequest
    {
        char *import_module;
        char *import_field;
        uint32_t kind;
        void *result;
    };
    struct EwaSymbolResolver
    {
        void (*resolve)(struct EwaSymbolResolver *_this, struct EwaSymbolResolveRequest *req);
    };

    typedef void *EwaModuleCompiler;

    typedef void *EwaModuleState;

    /* webassembly function definition (for 1.0 version) */

    typedef void *WasmManagedFunction;

    typedef void (*ewa_host_function_c)(void *stack_frame);

/* symbol type */
#define SYMBOL_KIND_FUNCTION 0
#define SYMBOL_KIND_TABLE 1
#define SYMBOL_KIND_MEMORY 2
#define SYMBOL_KIND_GLOBAL 3

    /* module load config */

    /* These config are related to ABI of the generated code, so keep same config to load module at same time in one process. */
    struct EwaGlobalCompileConfig
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

    struct WasmManagedTable
    {
        uint8_t elem_type; /* type of entries (only FUNC in MVP) */
        uint32_t initial;  /* initial table size */
        uint32_t maximum;  /* maximum table size */
        uint32_t size;     /* current table size */
        void **entries;
    };
    struct WasmManagedMemory
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

    /* EwaModuleState inspect result */
    // struct ewa_inspect_result1
    struct EwaInspectResult
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
        struct EwaSymbolResolver *symbol_resolver;
    };

    struct NamedSymbol
    {
        const char *name;
        union
        {
            WasmManagedFunction fn;
            struct WasmManagedMemory *mem;
            struct WasmManagedTable *tb;
        } val;
        uint8_t kind;
    };
    /* get the ewa builtin symbol array. Builtin symbol can be use by import "ewa_builtin" module in wasm. */
    extern struct NamedSymbol *ewa_get_builtin_symbols(int *arr_size);

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
    see also WasmManagedMemory.bytes

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
    WasmManagedFunction fread;

    fwrite
    func [ref buf,i32 size,i32 count,ref file]->[i32 nwrite]

    fclose
    func [ref file]->[i32 err]

    stdio
    func []->[ref stdin,ref stdout,ref stderr]

    native_memory
    memory
    special WasmManagedMemory that map to the native host memory directly, memory->bytes is 0;
    */

    /* High level API, Namespace and Module */

    typedef void *WasmNamespace;

    struct WasmManagedModule
    {
        void (*resolve)(struct WasmManagedModule *_this, struct EwaSymbolResolveRequest *req);
        /*Will be set after added to namespace , suffixed 2 to avoid keyword conflict with c++*/
        WasmNamespace namespace2;
        /*If not null, call after added to namespace */
        void (*on_attached)(struct WasmManagedModule *_this);
        /*If not null, call after remove from namespace, or namespace is deleted */
        void (*on_detached)(struct WasmManagedModule *_this);
    };

#define MODULE_TYPE_HOST_MODULE 1
#define MODULE_TYPE_WASM_MODULE 2
/* Indicate module has been free. */
#define MODULE_TYPE_NULL 0;

    struct WasmNamedModule
    {
        char *name;
        int type; /* MODULE_TYPE_xxxx */
        union
        {
            struct WasmManagedModule *host;
            EwaModuleState wasm;
        } val;
    };


    extern int GetVersion();

    /*  LoadModule load a module from data, if succeeded, return compiled module, if failed, return NULL and set err_msg, if err_msg is not NULL.
        It invoke "NewModuleCompiler","Compile","GetModuleState","FreeModuleCompiler" internally. */
    extern EwaModuleState *LoadModule(char *data, int len, char **err_msg);

    extern EwaModuleCompiler NewModuleCompiler();

    /*  free compile infomation, have no effect to EwaModuleState and generated code.
        return error message if any, or NULL if succeded. */
    extern char *FreeModuleCompiler(EwaModuleCompiler mod);

    extern char *SetSymbolResovler(EwaModuleCompiler m, struct EwaSymbolResolver *resolver);

    /*  compile module and generate code. if cfg is NULL, use default compile config.
        return error message if any, or NULL if succeded. */
    extern char *Compile(EwaModuleCompiler m, char *data, int len);

    /*  return wasm start function, or NULL if no start function specified.
        you should call the start function to init module, according to WebAssembly spec.
        Though it's may not necessary for ewa. */
    extern WasmManagedFunction GetStartFunction(EwaModuleState m);

    /*  return error message if any, or NULL if succeded. */
    extern char *SetGlobalCompileConfig(struct EwaGlobalCompileConfig *config);
    extern char *GetGlobalCompileConfig(struct EwaGlobalCompileConfig *config);

    extern EwaModuleState GetModuleState(EwaModuleCompiler m);

    /*  free module state and generated code.
        return error message if any, or NULL if succeded.
        only need free if Compile succeeded. */
    extern char *FreeModuleState(EwaModuleState rc);

    /*  The symbol resolver of EwaModuleState will be set to the same as compiler use by default,
        and can be changed by this function */
    extern char *SetStateSymbolResolver(EwaModuleState rc, struct EwaSymbolResolver *resolver);

    /* get wasm function exported by wasm module runtime. */
    extern WasmManagedFunction GetExportFunction(EwaModuleState rc, char *name);

    /* get wasm memory exported by wasm module runtime. */
    extern struct WasmManagedMemory *GetExportMemory(EwaModuleState rc, char *name);

    /* get wasm table exported by wasm module runtime. */
    extern struct WasmManagedTable *GetExportTable(EwaModuleState rc, char *name);

    /* get wasm globals exported by wasm module runtime. */
    extern void *GetExportGlobal(EwaModuleState rc, char *name);

    /*  return error message if any, or NULL if succeded. */
    extern char *InspectModuleState(EwaModuleState c, struct EwaInspectResult *result);

    extern void SetModuleStateUserData(EwaModuleState c, void *ud);
    extern void *GetModuleStateUserData(EwaModuleState c);

    /*  return error message if any, or NULL if succeded. */
    extern char *FreeModuleCompiler(EwaModuleCompiler mod);

    /* ewa invoke and runtime stack helper */

    /* allocate a stack buffer, with align 16. can only be free by FreeManagedStack. */
    extern void *AllocateManagedStack(int size);
    extern void FreeManagedStack(void *stack);

    /* wrap host function to wasm function, must be free by FreeWrappedFunction. */
    extern WasmManagedFunction WrapHostStdcFunction(ewa_host_function_c host_func);

    extern void FreeWrappedFunction(WasmManagedFunction wrapped);

    extern void CallFunction(WasmManagedFunction fn, void *stack_pointer);

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


    WasmNamespace NewNamespace();

    char *FreeNamespace(WasmNamespace ns);

    char *RemoveNamespaceModule(WasmNamespace ns, char *name);

    /* module structure will be copied into internal buffer, No need for caller to hold the memory */
    char *DefineModule(WasmNamespace ns, struct WasmNamedModule *mod);

    EwaModuleState *DefineWasmModule(WasmNamespace ns, char *name, char *wasm_bytes, int length, char **err_msg);

    struct WasmNamedModule *WFindNamespaceModule(WasmNamespace ns, char *name);

    struct EwaSymbolResolver *GetNamespaceSymbolResolver(WasmNamespace ns);

};
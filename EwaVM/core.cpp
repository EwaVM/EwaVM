#include <core.h>
#include <def.h>

#define EXPORT __attribute__((visibility("default")))

namespace EwaVM
{
    EXPORT int ewa_get_version()
    {
        return 1;
    }

    EXPORT ewa_module_compiler ewa_new_module_compiler()
    {
        ModuleCompiler *m = (ModuleCompiler *)wa_calloc(sizeof(ModuleCompiler));
        return m;
    }

    EXPORT char *ewa_free_module_compiler(ewa_module_compiler mod)
    {
        ModuleCompiler *m = (ModuleCompiler *)mod;
        free_module(m);
        return NULL;
    }

    EXPORT char *ewa_free_module_state(ewa_module_state rc2)
    {
        RuntimeContext *rc = (RuntimeContext *)rc2;
        free_runtimectx(rc);
        return NULL;
    }

    EXPORT char *ewa_compile(ewa_module_compiler m, char *data, int len)
    {
        return load_module((ModuleCompiler *)m, (uint8_t *)data, len);
    }

    EXPORT ewa_wasm_function ewa_get_export_function(ewa_module_state rc, char *name)
    {
        RuntimeContext *m = (RuntimeContext *)rc;
        uint32_t kind = SYMBOL_KIND_FUNCTION;
        Export *exp = get_export((RuntimeContext *)rc, name, &kind);
        if (exp != NULL)
        {
            return exp->value;
        }
        else
        {
            return NULL;
        }
    }

    EXPORT struct ewa_wasm_memory *ewa_get_export_memory(ewa_module_state rc, char *name)
    {
        RuntimeContext *m = (RuntimeContext *)rc;
        uint32_t kind = SYMBOL_KIND_MEMORY;
        Export *exp = (Export *)get_export((RuntimeContext *)rc, name, &kind);
        if (exp != NULL)
        {
            return (struct ewa_wasm_memory *)exp->value;
        }
        else
        {
            return NULL;
        }
    }

    EXPORT void *ewa_get_export_global(ewa_module_state rc, char *name)
    {
        RuntimeContext *m = (RuntimeContext *)rc;
        uint32_t kind = SYMBOL_KIND_GLOBAL;
        Export *exp = get_export((RuntimeContext *)rc, name, &kind);
        if (exp != NULL)
        {
            return exp->value;
        }
        else
        {
            return NULL;
        }
    }

    EXPORT struct ewa_wasm_table *ewa_get_export_table(ewa_module_state rc, char *name)
    {
        RuntimeContext *m = (RuntimeContext *)rc;
        uint32_t kind = SYMBOL_KIND_TABLE;
        Export *exp = get_export((RuntimeContext *)rc, name, &kind);
        if (exp != NULL)
        {
            return (struct ewa_wasm_table *)exp->value;
        }
        else
        {
            return NULL;
        }
    }

    EXPORT char *ewa_set_symbol_resolver(ewa_module_compiler m2, struct ewa_symbol_resolver *resolver)
    {
        ModuleCompiler *m = (ModuleCompiler *)m2;
        m->import_resolver = resolver;
        return NULL;
    }

    EXPORT char *ewa_set_global_compile_config(struct ewa_global_compile_config *cfg)
    {
        memcpy(&ewa_gcfg, cfg, sizeof(struct ewa_global_compile_config));
    };
    EXPORT char *ewa_get_global_compile_config(struct ewa_global_compile_config *cfg)
    {
        memcpy(cfg, &ewa_gcfg, sizeof(struct ewa_global_compile_config));
    }

    EXPORT ewa_module_state ewa_get_module_state(ewa_module_compiler mod)
    {
        ModuleCompiler *m = (ModuleCompiler *)mod;
        return m->context;
    }

    EXPORT char *ewa_inspect_module_state(ewa_module_state c, struct ewa_inspect_result1 *result)
    {
        RuntimeContext *rc = (RuntimeContext *)c;
        if (rc->tables->len > 0)
        {
            Table *tab = *dynarr_get(rc->tables, Table *, 0);
            result->table_entries_count = tab->size;
            result->table_entries = tab->entries;
        }
        if (rc->memories->len > 0)
        {
            Memory *mem = *dynarr_get(rc->memories, Memory *, 0);
            result->memory_size = mem->pages * PAGE_SIZE;
            result->memory = mem->bytes;
        }
        result->globals_buffer_size = rc->own_globals->len;
        result->globals_buffer = &rc->own_globals->data;
        result->symbol_resolver = rc->resolver;
        return NULL;
    }

    EXPORT char *ewa_set_state_symbol_resolver(ewa_module_state c, struct ewa_symbol_resolver *resolver)
    {
        RuntimeContext *rc = (RuntimeContext *)c;
        rc->resolver = resolver;
    }

    EXPORT void ewa_module_state_set_user_data(ewa_module_state c, void *ud)
    {
        RuntimeContext *rc = (RuntimeContext *)c;
        rc->userdata = ud;
    }

    EXPORT void *ewa_module_state_get_user_data(ewa_module_state c)
    {
        RuntimeContext *rc = (RuntimeContext *)c;
        return rc->userdata;
    }

    EXPORT void *ewa_allocate_stack(int size)
    {
        uint8_t *mem = (uint8_t *)wa_malloc(size + 16);
        uint8_t *stack_buffer = (uint8_t *)((((size_t)mem) + 16) & (~(size_t)(0xf)));
        stack_buffer[-1] = stack_buffer - mem;
        return stack_buffer;
    }

    EXPORT void ewa_free_stack(void *stack)
    {
        uint8_t *stack_buffer = (uint8_t *)stack;
        int off = stack_buffer[-1];
        stack_buffer -= off;
        wa_free(stack_buffer);
    }

    EXPORT void ewa_rstack_put_i32(void **sp, int val)
    {
        *(int32_t *)(*sp) = val;
        // *sp+=4
        (*sp) = ((char *)*sp) + 4;
    }
    EXPORT void ewa_rstack_put_i64(void **sp, long long val)
    {
        if (ewa_gcfg.stack_flags & STACK_FLAGS_AUTO_ALIGN)
        {
            size_t spv = (size_t)*sp;
            int t = (size_t)spv & 7;
            if (t > 0)
            {
                *sp = (void *)(spv + 8 - t);
            }
        }
        *(int64_t *)(*sp) = val;
        // *sp+=8;
        (*sp) = ((char *)*sp) + 8;
    }
    EXPORT void ewa_rstack_put_f32(void **sp, float val)
    {
        *(float *)(*sp) = val;
        // (*sp) += 4;
        (*sp) = ((char *)*sp) + 4;
    }
    EXPORT void ewa_rstack_put_f64(void **sp, double val)
    {
        if (ewa_gcfg.stack_flags & STACK_FLAGS_AUTO_ALIGN)
        {
            size_t spv = (size_t)*sp;
            int t = (size_t)spv & 7;
            if (t > 0)
            {
                *sp = (void *)(spv + 8 - t);
            }
        }
        *(double *)(*sp) = val;
        // *sp+=8;
        (*sp) = ((char *)*sp) + 8;
    }
    EXPORT void ewa_rstack_put_ref(void **sp, void *val)
    {
        if ((ewa_gcfg.stack_flags & STACK_FLAGS_AUTO_ALIGN) && (sizeof(void *) == 8))
        {
            size_t spv = (size_t)*sp;
            int t = (size_t)spv & 7;
            if (t > 0)
            {
                *sp = (void *)(spv + 8 - t);
            }
        }
        *(void **)(*sp) = val;
        // *sp+=sizeof(void *);
        (*sp) = ((char *)*sp) + sizeof(void *);
    }

    EXPORT int ewa_rstack_get_i32(void **sp)
    {
        int32_t *sp2 = (int32_t *)*sp;
        // *sp+=4;
        (*sp) = ((char *)*sp) + 4;
        return *sp2;
    }
    EXPORT long long ewa_rstack_get_i64(void **sp)
    {
        if (ewa_gcfg.stack_flags & STACK_FLAGS_AUTO_ALIGN)
        {
            size_t spv = (size_t)*sp;
            int t = (size_t)spv & 7;
            if (t > 0)
            {
                *sp = (void *)(spv + 8 - t);
            }
        }
        int64_t *sp2 = (int64_t *)*sp;
        // *sp+=8;
        (*sp) = ((char *)*sp) + 8;
        return *sp2;
    }
    EXPORT float ewa_rstack_get_f32(void **sp)
    {
        float *sp2 = (float *)*sp;
        // *sp+=4;
        (*sp) = ((char *)*sp) + 4;
        return *sp2;
    }
    EXPORT double ewa_rstack_get_f64(void **sp)
    {
        if (ewa_gcfg.stack_flags & STACK_FLAGS_AUTO_ALIGN)
        {
            size_t spv = (size_t)*sp;
            int t = (size_t)spv & 7;
            if (t > 0)
            {
                *sp = (void *)(spv + 8 - t);
            }
        }
        double *sp2 = (double *)*sp;
        // *sp+=8;
        (*sp) = ((char *)*sp) + 8;
        return *sp2;
    }
    EXPORT void *ewa_rstack_get_ref(void **sp)
    {
        if ((ewa_gcfg.stack_flags & STACK_FLAGS_AUTO_ALIGN) && (sizeof(void *) == 8))
        {
            size_t spv = (size_t)*sp;
            int t = (size_t)spv & 7;
            if (t > 0)
            {
                *sp = (void *)(spv + 8 - t);
            }
        }
        void **sp2 = (void **)*sp;
        // *sp+=sizeof(void *);
        (*sp) = ((char *)*sp) + sizeof(void *);
        return *sp2;
    }

    EXPORT ewa_wasm_function ewa_wrap_host_function_c(ewa_host_function_c host_func)
    {
        // currently, we don't need wrap host function.
        return host_func;
    }

    EXPORT void ewa_free_wrapped_function(ewa_wasm_function wrapped)
    {
    }

    EXPORT void ewa_call_wasm_function(ewa_wasm_function fn, void *stack_pointer)
    {
        WasmFunctionEntry fn2 = (WasmFunctionEntry)fn;
        (*fn2)(stack_pointer);
    }

    EXPORT ewa_module_state *ewa_load_module(char *data, int len, char **err_msg)
    {
        char *err = NULL;
        ewa_module_state state = NULL;
        ewa_module_compiler m = ewa_new_module_compiler();
        err = ewa_compile(m, data, len);
        if (err == NULL)
        {
            state = ewa_get_module_state(m);
        }
        else
        {
            if (err_msg != NULL)
                *err_msg = err;
        }
        ewa_free_module_compiler(m);
        return (ewa_module_state *)state;
    }

    EXPORT ewa_wasm_function ewa_get_start_function(ewa_module_state m)
    {
        RuntimeContext *rc = (RuntimeContext *)m;
        if (rc->start_function == 0xffffffff)
        {
            return NULL;
        }
        else
        {
            return (ewa_wasm_function)rc->funcentries[rc->start_function];
        }
    }

    static struct dynarr *builtin_symbols = NULL; // type ewa_named_symbol
    static struct ewa_wasm_memory native_memory = {
        .initial = 0x7fffffff,
        .maximum = 0x7fffffff,
        .pages = 0x7fffffff,
        .bytes = NULL,
        .fixed = 1};

#define _ADD_BUILTIN_FN(fname)                                           \
    sym = dynarr_push_type(&builtin_symbols, struct ewa_named_symbol); \
    sym->name = #fname;                                                  \
    sym->kind = SYMBOL_KIND_FUNCTION;                                     \
    sym->val.fn = ewa_wrap_host_function_c((ewa_host_function_c)insn_##fname);

    EXPORT struct ewa_named_symbol *ewa_get_builtin_symbols(int *arr_size)
    {
        if (builtin_symbols == NULL)
        {
            struct ewa_named_symbol *sym;
            dynarr_init(&builtin_symbols, sizeof(struct ewa_named_symbol));
            _ADD_BUILTIN_FN(version)
            _ADD_BUILTIN_FN(memory_alloc)
            _ADD_BUILTIN_FN(memory_free)
            _ADD_BUILTIN_FN(load_module)
            _ADD_BUILTIN_FN(unload_module)
            _ADD_BUILTIN_FN(import)
            _ADD_BUILTIN_FN(get_self_runtime_context)
            ewa_InlineFuncList.get_self_runtime_context = sym->val.fn;
            _ADD_BUILTIN_FN(native_index_size)
            _ADD_BUILTIN_FN(ref_from_index)
            ewa_InlineFuncList.ref_from_index = sym->val.fn;
            _ADD_BUILTIN_FN(ref_copy_bytes)
            _ADD_BUILTIN_FN(ref_string_length)
            _ADD_BUILTIN_FN(ref_from_i64)
            ewa_InlineFuncList.ref_from_i64 = sym->val.fn;
            _ADD_BUILTIN_FN(i64_from_ref)
            ewa_InlineFuncList.i64_from_ref = sym->val.fn;

            _ADD_BUILTIN_FN(host_definition)

            _ADD_BUILTIN_FN(fread)
            _ADD_BUILTIN_FN(fwrite)
            _ADD_BUILTIN_FN(fopen)
            _ADD_BUILTIN_FN(fclose)
            _ADD_BUILTIN_FN(stdio)

            sym = dynarr_push_type(&builtin_symbols, struct ewa_named_symbol);
            sym->name = "native_memory";
            sym->kind = SYMBOL_KIND_MEMORY;
            sym->val.mem = &native_memory;
        }
        *arr_size = builtin_symbols->len;
        return (struct ewa_named_symbol *)&builtin_symbols->data;
    }

};
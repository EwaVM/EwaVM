#include <def.h>

namespace EwaVM
{

    struct ewa_global_compile_config ewa_gcfg = {
        .stack_flags = 0,
        .misc_flags = MISC_FLAGS_LOCALS_ZERO_INIT};

    Namespace *namespace_GetNamespaceFormResolver(struct ewa_symbol_resolver *_this)
    {
        return (Namespace *)_this;
    }

    struct ewa_symbol_resolver *ewa_namespace_resolver(ewa_namespace ns)
    {
        return &((Namespace *)ns)->resolver;
    }

    void namespace_SymbolResolve(struct ewa_symbol_resolver *_this, struct ewa_symbol_resolve_request *req)
    {
        Namespace *n = namespace_GetNamespaceFormResolver(_this);
        struct ewa_named_module *m = ewa_namespace_find_module(n, req->import_module);
        req->result = NULL;
        if (m == NULL)
        {
            return;
        }
        switch (m->type)
        {
        case MODULE_TYPE_HOST_MODULE:
            m->val.host->resolve(m->val.host, req);
            break;
        case MODULE_TYPE_WASM_MODULE:
            switch (req->kind)
            {
            case SYMBOL_KIND_FUNCTION:
                req->result = ewa_get_export_function(m->val.wasm, req->import_field);
                break;
            case SYMBOL_KIND_GLOBAL:
                req->result = ewa_get_export_global(m->val.wasm, req->import_field);
                break;
            case SYMBOL_KIND_TABLE:
                req->result = ewa_get_export_table(m->val.wasm, req->import_field);
                break;
            case SYMBOL_KIND_MEMORY:
                req->result = ewa_get_export_memory(m->val.wasm, req->import_field);
                break;
            }
            break;
        }
        return;
    }

    ewa_namespace ewa_namespace_new()
    {
        Namespace *ns = (Namespace *)wa_calloc(sizeof(Namespace));
        dynarr_init(&ns->mods, sizeof(struct ewa_named_module));
        ns->resolver.resolve = &namespace_SymbolResolve;
        return ns;
    }

    char *ewa_namespace_delete(ewa_namespace ns)
    {
        Namespace *n = (Namespace *)ns;
        int i = 0;
        for (i = 0; i < n->mods->len; i++)
        {
            struct ewa_named_module *m = dynarr_get(n->mods, struct ewa_named_module, i);
            switch (m->type)
            {
            case MODULE_TYPE_HOST_MODULE:
                if (m->val.host->on_detached != NULL)
                {
                    m->val.host->on_detached(m->val.host);
                }
                break;
            case MODULE_TYPE_WASM_MODULE:
                ReturnIfErr(ewa_free_module_state(m->val.wasm));
                break;
            }
        }
        wa_free(n);
        return NULL;
    }

    struct ewa_named_module *ewa_namespace_find_module(ewa_namespace ns, char *name)
    {
        Namespace *n = (Namespace *)ns;
        int i = 0;
        for (i = 0; i < n->mods->len; i++)
        {
            struct ewa_named_module *m = dynarr_get(n->mods, struct ewa_named_module, i);
            if (strcmp(m->name, name) == 0)
            {
                return m;
            }
        }
        return NULL;
    }

    char *ewa_namespace_remove_module(ewa_namespace ns, char *name)
    {
        struct ewa_named_module *m = ewa_namespace_find_module(ns, name);
        if (m != NULL)
        {
            switch (m->type)
            {
            case MODULE_TYPE_HOST_MODULE:
                if (m->val.host->on_detached != NULL)
                {
                    m->val.host->on_detached(m->val.host);
                }
                break;
            case MODULE_TYPE_WASM_MODULE:
                ReturnIfErr(ewa_free_module_state(m->val.wasm));
                break;
            }
            m->type = MODULE_TYPE_NULL;
        }
        return NULL;
    }

    char *ewa_namespace_define_module(ewa_namespace ns, struct ewa_named_module *mod)
    {
        Namespace *n = (Namespace *)ns;
        struct ewa_named_module *m;
        m = ewa_namespace_find_module(ns, mod->name);
        if (m != NULL)
        {
            ewa_namespace_remove_module(ns, m->name);
        }
        else
        {
            m = dynarr_push_type(&n->mods, struct ewa_named_module);
        }
        memmove(m, mod, sizeof(struct ewa_named_module));
        switch (m->type)
        {
        case MODULE_TYPE_HOST_MODULE:
            m->val.host->namespace2 = ns;
            if (m->val.host->on_attached != NULL)
            {
                m->val.host->on_attached(m->val.host);
            }
            break;
        case MODULE_TYPE_WASM_MODULE:
            ewa_set_state_symbol_resolver(m->val.wasm, &n->resolver);
            ((RuntimeContext *)m->val.wasm)->is_in_namespace = 1;
            break;
        }
    }

    ewa_module_state *ewa_namespace_define_wasm_module(ewa_namespace ns, char *name, char *wasm_bytes, int length, char **err_msg)
    {
        Namespace *n = (Namespace *)ns;
        struct ewa_named_module mod;
        char *err = NULL;
        ewa_module_state state = NULL;
        ewa_module_compiler m = ewa_new_module_compiler();
        ewa_set_symbol_resolver(m, &n->resolver);
        err = ewa_compile(m, wasm_bytes, length);
        if (err == NULL)
        {
            state = ewa_get_module_state(m);
        }
        else
        {
            if (err_msg != NULL)
                *err_msg = err;
            return NULL;
        }
        ewa_free_module_compiler(m);
        mod.name = name;
        mod.type = MODULE_TYPE_WASM_MODULE;
        mod.val.wasm = state;
        ewa_namespace_define_module(ns, &mod);
        return (ewa_module_state *)state;
    }

};
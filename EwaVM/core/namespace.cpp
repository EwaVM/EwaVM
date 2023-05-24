#include <internal/def.h>

namespace EwaVM
{

    struct EwaGlobalCompileConfig gConfig = {
        .stack_flags = 0,
        .misc_flags = MISC_FLAGS_LOCALS_ZERO_INIT};

    Namespace *namespace_GetNamespaceFormResolver(struct EwaSymbolResolver *_this)
    {
        return (Namespace *)_this;
    }

    struct EwaSymbolResolver *GetNamespaceSymbolResolver(WasmNamespace ns)
    {
        return &((Namespace *)ns)->resolver;
    }

    void namespace_SymbolResolve(struct EwaSymbolResolver *_this, struct EwaSymbolResolveRequest *req)
    {
        Namespace *n = namespace_GetNamespaceFormResolver(_this);
        struct WasmNamedModule *m = WFindNamespaceModule(n, req->import_module);
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
                req->result = GetExportFunction(m->val.wasm, req->import_field);
                break;
            case SYMBOL_KIND_GLOBAL:
                req->result = GetExportGlobal(m->val.wasm, req->import_field);
                break;
            case SYMBOL_KIND_TABLE:
                req->result = GetExportTable(m->val.wasm, req->import_field);
                break;
            case SYMBOL_KIND_MEMORY:
                req->result = GetExportMemory(m->val.wasm, req->import_field);
                break;
            }
            break;
        }
        return;
    }

    WasmNamespace NewNamespace()
    {
        Namespace *ns = (Namespace *)wa_calloc(sizeof(Namespace));
        dynarr_init(&ns->mods, sizeof(struct WasmNamedModule));
        ns->resolver.resolve = &namespace_SymbolResolve;
        return ns;
    }

    char *FreeNamespace(WasmNamespace ns)
    {
        Namespace *n = (Namespace *)ns;
        int i = 0;
        for (i = 0; i < n->mods->len; i++)
        {
            struct WasmNamedModule *m = dynarr_get(n->mods, struct WasmNamedModule, i);
            switch (m->type)
            {
            case MODULE_TYPE_HOST_MODULE:
                if (m->val.host->on_detached != NULL)
                {
                    m->val.host->on_detached(m->val.host);
                }
                break;
            case MODULE_TYPE_WASM_MODULE:
                ReturnIfErr(FreeModuleState(m->val.wasm));
                break;
            }
        }
        wa_free(n);
        return NULL;
    }

    struct WasmNamedModule *WFindNamespaceModule(WasmNamespace ns, char *name)
    {
        Namespace *n = (Namespace *)ns;
        int i = 0;
        for (i = 0; i < n->mods->len; i++)
        {
            struct WasmNamedModule *m = dynarr_get(n->mods, struct WasmNamedModule, i);
            if (strcmp(m->name, name) == 0)
            {
                return m;
            }
        }
        return NULL;
    }

    char *RemoveNamespaceModule(WasmNamespace ns, char *name)
    {
        struct WasmNamedModule *m = WFindNamespaceModule(ns, name);
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
                ReturnIfErr(FreeModuleState(m->val.wasm));
                break;
            }
            m->type = MODULE_TYPE_NULL;
        }
        return NULL;
    }

    char *DefineModule(WasmNamespace ns, struct WasmNamedModule *mod)
    {
        Namespace *n = (Namespace *)ns;
        struct WasmNamedModule *m;
        m = WFindNamespaceModule(ns, mod->name);
        if (m != NULL)
        {
            RemoveNamespaceModule(ns, m->name);
        }
        else
        {
            m = dynarr_push_type(&n->mods, struct WasmNamedModule);
        }
        memmove(m, mod, sizeof(struct WasmNamedModule));
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
            SetStateSymbolResolver(m->val.wasm, &n->resolver);
            ((RuntimeContext *)m->val.wasm)->is_in_namespace = 1;
            break;
        }
    }

    EwaModuleState *DefineWasmModule(WasmNamespace ns, char *name, char *wasm_bytes, int length, char **err_msg)
    {
        Namespace *n = (Namespace *)ns;
        struct WasmNamedModule mod;
        char *err = NULL;
        EwaModuleState state = NULL;
        EwaModuleCompiler m = NewModuleCompiler();
        SetSymbolResovler(m, &n->resolver);
        err = Compile(m, wasm_bytes, length);
        if (err == NULL)
        {
            state = GetModuleState(m);
        }
        else
        {
            if (err_msg != NULL)
                *err_msg = err;
            return NULL;
        }
        FreeModuleCompiler(m);
        mod.name = name;
        mod.type = MODULE_TYPE_WASM_MODULE;
        mod.val.wasm = state;
        DefineModule(ns, &mod);
        return (EwaModuleState *)state;
    }

};
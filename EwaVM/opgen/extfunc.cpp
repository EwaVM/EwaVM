#include <internal/def.h>

namespace EwaVM
{
  namespace OpGen
  {

    void insn_i64eqz(void *fp)
    {
      uint32_t *result = (uint32_t *)fp;
      uint64_t *arg1 = (uint64_t *)fp;
      *result = *arg1 == 0;
    }

    void insn_i64clz(void *fp)
    {
      uint64_t x = *(uint64_t *)fp;
      if (x != 0)
      {
        int c = 63;
        if (x >> 32)
          x >>= 32, c -= 32;
        if (x >> 16)
          x >>= 16, c -= 16;
        if (x >> 8)
          x >>= 8, c -= 8;
        if (x >> 4)
          x >>= 4, c -= 4;
        if (x >> 2)
          x >>= 2, c -= 2;
        *(uint32_t *)fp = c - (x >> 1);
      }
      else
      {
        *(uint32_t *)fp = 64;
      }
    }
    void insn_i64ctz(void *fp)
    {
      uint64_t x = *(uint64_t *)fp;
      if (x != 0)
      {
        int c = 63;
        if (x << 32)
          x <<= 32, c -= 32;
        if (x << 16)
          x <<= 16, c -= 16;
        if (x << 8)
          x <<= 8, c -= 8;
        if (x << 4)
          x <<= 4, c -= 4;
        if (x << 2)
          x <<= 2, c -= 2;
        if (x << 1)
          c -= 1;
        *(uint32_t *)fp = c;
      }
      else
      {
        *(uint32_t *)fp = 64;
      }
    }
    void insn_i64popcount(void *fp)
    {
      uint64_t x = *(uint64_t *)fp;
      x -= (x & 0xaaaaaaaaaaaaaaaaull) >> 1;
      x = (x & 0x3333333333333333ull) + ((x & 0xccccccccccccccccull) >> 2);
      x = ((x * 0x11) & 0xf0f0f0f0f0f0f0f0ull) >> 4;
      *(uint32_t *)fp = (uint32_t)((x * 0x0101010101010101ull) >> 56);
    }

    void insn_i32clz(void *fp)
    {
      uint32_t x = *(uint32_t *)fp;
      if (x != 0)
      {
        int c = 31;
        if (x >> 16)
          x >>= 16, c -= 16;
        if (x >> 8)
          x >>= 8, c -= 8;
        if (x >> 4)
          x >>= 4, c -= 4;
        if (x >> 2)
          x >>= 2, c -= 2;
        *(uint32_t *)fp = c - (x >> 1);
      }
      else
      {
        *(uint32_t *)fp = 32;
      }
    }
    void insn_i32ctz(void *fp)
    {
      uint32_t x = *(uint32_t *)fp;
      if (x != 0)
      {
        int c = 31;
        if (x << 16)
          x <<= 16, c -= 16;
        if (x << 8)
          x <<= 8, c -= 8;
        if (x << 4)
          x <<= 4, c -= 4;
        if (x << 2)
          x <<= 2, c -= 2;
        if (x << 1)
          c -= 1;
        *(uint32_t *)fp = c;
      }
      else
      {
        *(uint32_t *)fp = 32;
      }
    }
    void insn_i32popcount(void *fp)
    {
      uint32_t x = *(uint32_t *)fp;
      x = x - ((x >> 1) & 0x55555555);
      x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
      *(uint32_t *)fp = ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
    }

    void insn_i64eq(void *fp)
    {
      uint64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (uint64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 == *arg2;
    }
    void insn_i64ne(void *fp)
    {
      uint64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (uint64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 != *arg2;
    }
    void insn_i64lts(void *fp)
    {
      int64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (int64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 < *arg2;
    }
    void insn_i64ltu(void *fp)
    {
      uint64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (uint64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 < *arg2;
    }
    void insn_i64gts(void *fp)
    {
      int64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (int64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 > *arg2;
    }
    void insn_i64gtu(void *fp)
    {
      uint64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (uint64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 > *arg2;
    }
    void insn_i64les(void *fp)
    {
      int64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (int64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 <= *arg2;
    }
    void insn_i64leu(void *fp)
    {
      uint64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (uint64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 <= *arg2;
    }
    void insn_i64ges(void *fp)
    {
      int64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (int64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 >= *arg2;
    }
    void insn_i64geu(void *fp)
    {
      uint64_t *arg1, *arg2;
      uint32_t *result = (uint32_t *)fp;
      arg1 = (uint64_t *)fp;
      arg2 = arg1 + 1;
      *result = *arg1 >= *arg2;
    }

#include <math.h>

    void insn_f32ceil(float *fp)
    {
      *fp = ceilf(*fp);
    }
    void insn_f32floor(float *fp) { *fp = floorf(*fp); }
    void insn_f32trunc(float *fp) { *fp = truncf(*fp); }
    void insn_f32nearest(float *fp) { *fp = rintf(*fp); }
    void insn_f32sqrt(float *fp) { *fp = sqrtf(*fp); }

    void insn_f64ceil(double *fp) { *fp = ceil(*fp); }
    void insn_f64floor(double *fp) { *fp = floor(*fp); }
    void insn_f64trunc(double *fp) { *fp = trunc(*fp); }
    void insn_f64nearest(double *fp) { *fp = rint(*fp); }
    void insn_f64sqrt(double *fp) { *fp = sqrt(*fp); }

    void insn_i32rotl(uint32_t *fp)
    {
      uint32_t x = *fp;
      uint32_t y = *(fp + 1);
      y = y & 31;
      *fp = (x << y) | (x >> (32 - y));
    }
    void insn_i32rotr(uint32_t *fp)
    {
      uint32_t x = *fp;
      uint32_t y = *(fp + 1);
      y = y & 31;
      *fp = (x >> y) | (x << (32 - y));
    }

    void insn_i64rotl(uint64_t *fp)
    {
      uint64_t x = *fp;
      uint64_t y = *(fp + 1);
      y = y & 63;
      *fp = (x << y) | (x >> (64 - y));
    }
    void insn_i64rotr(uint64_t *fp)
    {
      uint64_t x = *fp;
      uint64_t y = *(fp + 1);
      y = y & 63;
      *fp = (x >> y) | (x << (64 - y));
    }
    void insn_i64mul(uint64_t *fp) { *fp = (*fp) * (*(fp + 1)); }
    void insn_i64divs(int64_t *fp) { *fp = (*fp) / (*(fp + 1)); }
    void insn_i64divu(uint64_t *fp) { *fp = (*fp) / (*(fp + 1)); }
    void insn_i64rems(int64_t *fp) { *fp = (*fp) % (*(fp + 1)); }
    void insn_i64remu(uint64_t *fp) { *fp = (*fp) % (*(fp + 1)); }
    void insn_i64shl(uint64_t *fp) { *fp = (*fp) << (*(fp + 1)); }
    void insn_i64shrs(int64_t *fp) { *fp = (*fp) >> (*(fp + 1)); }
    void insn_i64shru(uint64_t *fp) { *fp = (*fp) >> (*(fp + 1)); }

    void insn_f32max(float *fp)
    {
      *fp = (*fp) > *(fp + 1) ? (*fp) : *(fp + 1);
    }
    void insn_f32min(float *fp)
    {
      *fp = (*fp) < *(fp + 1) ? (*fp) : *(fp + 1);
    }
    void insn_f32copysign(float *fp)
    {
      *fp = signbit(*fp) ? -fabs(*(fp + 1)) : fabs(*(fp + 1));
    }

    void insn_f64max(double *fp)
    {
      *fp = (*fp) > *(fp + 1) ? (*fp) : *(fp + 1);
    }
    void insn_f64min(double *fp)
    {
      *fp = (*fp) < *(fp + 1) ? (*fp) : *(fp + 1);
    }
    void insn_f64copysign(double *fp)
    {
      *fp = signbit(*fp) ? -fabs(*(fp + 1)) : fabs(*(fp + 1));
    }

    void insn_i32truncf32u(uint32_t *fp)
    {
      *fp = (uint32_t)(*(float *)fp);
    }

    void insn_i32truncf64u(uint32_t *fp)
    {
      *fp = (uint32_t)(*(double *)fp);
    }

    void insn_i64truncf32s(int64_t *fp)
    {
      *fp = (int64_t)(*(float *)fp);
    }
    void insn_i64truncf32u(uint64_t *fp)
    {
      *fp = (uint64_t)(*(float *)fp);
    }

    void insn_i64truncf64s(int64_t *fp)
    {
      *fp = (int64_t)(*(double *)fp);
    }
    void insn_i64truncf64u(uint64_t *fp)
    {
      *fp = (uint64_t)(*(double *)fp);
    }

    void insn_f32converti32u(float *fp)
    {
      *fp = (float)(*(uint32_t *)fp);
    }

    void insn_f32converti64s(float *fp)
    {
      *fp = (float)(*(int64_t *)fp);
    }
    void insn_f32converti64u(float *fp)
    {
      *fp = (float)(*(uint64_t *)fp);
    }

    void insn_f64converti32u(double *fp)
    {
      *fp = (double)(*(uint32_t *)fp);
    }

    void insn_f64converti64s(double *fp)
    {
      *fp = (double)(*(int64_t *)fp);
    }
    void insn_f64converti64u(double *fp)
    {
      *fp = (double)(*(uint64_t *)fp);
    }
    // keep argument aligned, to avoid effect from STACK_FLAGS_AUTO_ALIGN.
    // (i32 delta, i32 memory_index,ref runtime_context)
    void insn_memorygrow(void *fp)
    {
      void *sp = fp;
      uint32_t delta = ewa_rstack_get_i32(&sp);
      uint32_t index = ewa_rstack_get_i32(&sp); // memory index
      RuntimeContext *m = (RuntimeContext *)ewa_rstack_get_ref(&sp);
      Memory *mem = *dynarr_get(m->memories, Memory *, index);
      uint32_t prev_pages = mem->pages;
      if (delta == 0)
      {
        return; // No change
      }
      else if (delta + prev_pages > mem->maximum)
      {
        prev_pages = 0xffffffff;
        return;
      }
      mem->pages += delta;
      if (mem->fixed != 0)
      {
        mem->bytes = (uint8_t *)wa_realloc(mem->bytes, mem->pages * PAGE_SIZE);
      }
      sp = fp;
      ewa_rstack_put_i32(&sp, prev_pages);
      return;
    }
    // get ewa version
    void insn_version(uint32_t *fp)
    {
      *fp = GetVersion();
    }
    // allocate memory
    void insn_memory_alloc(void *fp)
    {
      void *sp = fp;
      uint32_t size = ewa_rstack_get_i32(&sp);
      sp = fp;
      ewa_rstack_put_i64(&sp, (uint64_t)(size_t)malloc(size));
    }
    void insn_memory_free(void *fp)
    {
      void *sp = fp;
      void *p = (void *)(size_t)ewa_rstack_get_i64(&sp);
      free(p);
    }

    //(ref destination,i32 value,i32 count,ref refvalue,i32 fillref)
    void insn_memoryfill(void *fp)
    {
      void *sp = fp;
      void *d = ewa_rstack_get_ref(&sp);
      uint32_t val = ewa_rstack_get_i32(&sp);
      uint32_t n = ewa_rstack_get_i32(&sp);
      void *val2 = ewa_rstack_get_ref(&sp);
      uint32_t fillref = ewa_rstack_get_i32(&sp);
#if DEBUG_BUILD
      wa_debug("insn_memoryfill:%p,%x,%x,%p,%x\n", d, val, n, val2, fillref);
#endif
      if (fillref)
      {
        void **ent = (void **)d;
        for (int i1 = 0; i1 < n; i1++)
        {
          ent[i1] = val2;
        }
      }
      else
      {
        memset(d, val, n);
      }
    }

    void insn_native_index_size(void *fp)
    {
      ewa_rstack_put_i32(&fp, sizeof(void *));
    }

    void insn_ref_from_i64(void *fp)
    {
      void *sp = fp;
      void *ref = (void *)(size_t)ewa_rstack_get_i64(&sp);
      sp = fp;
      ewa_rstack_put_ref(&sp, ref);
    }
    void insn_i64_from_ref(void *fp)
    {
      void *sp = fp;
      uint64_t i64 = (uint64_t)(size_t)ewa_rstack_get_ref(&sp);
      sp = fp;
      ewa_rstack_put_i64(&sp, i64);
    }

    void insn_get_self_runtime_context(void *fp)
    {
      wa_assert(0, "This is stub function. can only call directly.");
    }
    void insn_ref_from_index(void *fp)
    {
      wa_assert(0, "This is stub function. can only call directly.");
    }
    void insn_ref_copy_bytes(void *fp)
    {
      void *sp = fp;
      void *d = ewa_rstack_get_ref(&sp);
      void *s = ewa_rstack_get_ref(&sp);
      uint32_t n = ewa_rstack_get_i32(&sp);
      memmove(d, s, n);
    }

    void insn_ref_string_length(void *fp)
    {
      void *sp = fp;
      char *s = (char *)ewa_rstack_get_ref(&sp);
      uint32_t n = strlen(s);
      sp = fp;
      ewa_rstack_put_i32(&sp, n);
    }

    void insn_load_module(void *fp)
    {
      void *sp = fp;
      RuntimeContext *m = (RuntimeContext *)ewa_rstack_get_ref(&sp);
      char *name = (char *)ewa_rstack_get_ref(&sp);
      char *wasm_bytes = (char *)ewa_rstack_get_ref(&sp);
      uint32_t length = ewa_rstack_get_i32(&sp);
      sp = fp;
      Namespace *ns = (Namespace *)namespace_GetNamespaceFormResolver(m->resolver);
      char *err_msg = NULL;
      EwaModuleState stat = DefineWasmModule((WasmNamespace)ns, name, wasm_bytes, length, &err_msg);
      WasmManagedFunction stfn = GetStartFunction(stat);
      if (stfn != NULL)
      {
        CallFunction(stfn, sp);
      }
      ewa_rstack_put_ref(&sp, err_msg);
    }

    void insn_unload_module(void *fp)
    {
      void *sp = fp;
      RuntimeContext *m = (RuntimeContext *)ewa_rstack_get_ref(&sp);
      char *name = (char *)ewa_rstack_get_ref(&sp);
      Namespace *ns = (Namespace *)namespace_GetNamespaceFormResolver(m->resolver);
      char *err_msg = NULL;
      err_msg = RemoveNamespaceModule((WasmNamespace)ns, name);
      sp = fp;
      ewa_rstack_put_ref(&sp, err_msg);
    }

    void insn_import(void *fp)
    {
      void *sp = fp;
      RuntimeContext *m = (RuntimeContext *)ewa_rstack_get_ref(&sp);
      struct EwaSymbolResolveRequest req;
      req.import_module = (char *)ewa_rstack_get_ref(&sp);
      req.import_field = (char *)ewa_rstack_get_ref(&sp);
      req.kind = ewa_rstack_get_i32(&sp);
      req.result = NULL;
      Namespace *ns = (Namespace *)namespace_GetNamespaceFormResolver(m->resolver);
      ns->resolver.resolve(&ns->resolver, &req);
      sp = fp;
      ewa_rstack_put_ref(&sp, req.result);
    }

    uint8_t types_void[] = {0};

    uint8_t types_i32[] = {WVT_I32, 0};
    uint8_t types_i64[] = {WVT_I64, 0};
    uint8_t types_f32[] = {WVT_F32, 0};
    uint8_t types_f64[] = {WVT_F64, 0};

    uint8_t types_i32_i32[] = {WVT_I32, WVT_I32, 0};
    uint8_t types_i64_i64[] = {WVT_I64, WVT_I64, 0};
    uint8_t types_f32_f32[] = {WVT_F32, WVT_F32, 0};
    uint8_t types_f64_f64[] = {WVT_F64, WVT_F64, 0};

    uint8_t types_i32_i32_ref[] = {WVT_I32, WVT_I32, WVT_REF, 0};

    uint8_t types_ref_ref_i32[] = {WVT_REF, WVT_REF, WVT_I32, 0};

    uint8_t types_ref_i32_i32_ref_i32[] = {WVT_REF, WVT_I32, WVT_I32, WVT_REF, WVT_I32, 0};

    Type func_type_ret_void = {.params = types_void, .results = types_void};
    Type func_type_ret_i32 = {.params = types_void, .results = types_i32};
    Type func_type_ret_i64 = {.params = types_void, .results = types_i64};
    Type func_type_ret_f32 = {.params = types_void, .results = types_i32};
    Type func_type_ret_f64 = {.params = types_void, .results = types_f64};
    Type func_type_i64_ret_i32 = {.params = types_i64, .results = types_i32};
    Type func_type_i64_ret_i64 = {.params = types_i64, .results = types_i64};
    Type func_type_i32_ret_i32 = {.params = types_i32, .results = types_i32};
    Type func_type_i64_i64_ret_i32 = {.params = types_i64_i64,
                                      .results = types_i32};
    Type func_type_f32_ret_f32 = {.params = types_f32, .results = types_f32};
    Type func_type_f64_ret_f64 = {.params = types_f64, .results = types_f64};
    Type func_type_i32_i32_ret_i32 = {.params = types_i32_i32,
                                      .results = types_i32};
    Type func_type_i64_i64_ret_i64 = {.params = types_i64_i64,
                                      .results = types_i64};
    Type func_type_f32_f32_ret_f32 = {.params = types_f32_f32,
                                      .results = types_f32};
    Type func_type_f64_f64_ret_f64 = {.params = types_f64_f64,
                                      .results = types_f64};
    Type func_type_f32_ret_i32 = {.params = types_f32, .results = types_i32};
    Type func_type_f64_ret_i32 = {.params = types_f64, .results = types_i32};
    Type func_type_f32_ret_i64 = {.params = types_f32, .results = types_i64};
    Type func_type_f64_ret_i64 = {.params = types_f64, .results = types_i64};
    Type func_type_i32_ret_f32 = {.params = types_i32, .results = types_f32};
    Type func_type_i64_ret_f32 = {.params = types_i64, .results = types_f32};
    Type func_type_i32_ret_f64 = {.params = types_i32, .results = types_f64};
    Type func_type_i64_ret_f64 = {.params = types_i64, .results = types_f64};

    Type func_type_i32_i32_ref_ret_i32 = {.params = types_i32_i32_ref, .results = types_i32};
    Type func_type_ref_ref_i32_void = {.params = types_ref_ref_i32, .results = types_void};
    Type func_type_memoryfill = {.params = types_ref_i32_i32_ref_i32, .results = types_void};

    void waexpr_run_const(ModuleCompiler *m, void *result)
    {
      int opcode = m->bytes[m->pc];
      m->pc++;
      switch (opcode)
      {
      case 0x41: // i32.const
        *(uint32_t *)result = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32);
        break;
      case 0x42: // i64.const
        *(uint64_t *)result = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 64);
        break;
      case 0x43: // f32.const
        *(float *)result = *(float *)(m->bytes + m->pc);
        m->pc += 4;
        break;
      case 0x44: // f64.const
        *(double *)result = *(double *)(m->bytes + m->pc);
        m->pc += 8;
        break;
      case 0x23: // global.get
      {
        uint32_t arg = read_LEB(m->bytes, (uint32_t *)&m->pc, 32);
        StackValue *sv = dynarr_get(m->globals, StackValue, arg);
        if (stackvalue_GetSizeAndAlign(sv, NULL) == 8)
        {
          *(uint64_t *)result = *(uint64_t *)(sv->val.opw);
        }
        else
        {
          *(uint32_t *)result = *(uint32_t *)(sv->val.opw);
        }
      }
      break;
      case 0xd0: // ref.null
        *(void **)result = NULL;
        break;
      case 0xd2: // ref.func
      {
        uint32_t arg = read_LEB(m->bytes, (uint32_t *)&m->pc, 32);
        *(WasmFunction **)result = dynarr_get(m->functions, WasmFunction, arg);
      }
      default:
        SLJIT_UNREACHABLE();
        break;
      }
      SLJIT_ASSERT(m->bytes[m->pc] == 0xb);
      m->pc++;
    }

    void debug_printtypes(char *t)
    {
      for (char *t2 = t; *t2 != 0; *t2++)
      {
        switch (*t2)
        {
        case WVT_I32:
          wa_debug("i32,");
          break;
        case WVT_I64:
          wa_debug("i64,");
          break;
        case WVT_F32:
          wa_debug("f32,");
          break;
        case WVT_F64:
          wa_debug("f64,");
          break;
        case WVT_FUNC:
          wa_debug("func,");
          break;
        case WVT_REF:
          wa_debug("ref,");
          break;
        default:
          wa_debug("unknown,");
          break;
        }
      }
    }

    void debug_printfunctype(Type *type)
    {
      wa_debug("type->form:%x\n", type->form);
      wa_debug("type->params at %p:", type->params);
      debug_printtypes((char *)type->params);
      wa_debug("\n");
      wa_debug("type->result at %p:", type->results);
      debug_printtypes((char *)type->results);
      wa_debug("\n");
    }

#define __tostrInternal(s) #s
#define tostr(s) __tostrInternal(s)
    char *host_definition[] = {
#ifdef __linux__
        "__linux__",
#endif
#ifdef _WIN32
        "_WIN32",
#endif
#ifdef _WIN64
        "_WIN64",
#endif
#ifdef __x86__
        "__x86__",
#endif
#ifdef __x86_64__
        "__x86_64__",
#endif
#ifdef __ARM_ARCH
        "__ARM_ARCH=" tostr(__ARM_ARCH),
#endif
#ifdef __riscv_xlen
        "__riscv_xlen=" tostr(__riscv_xlen),
#endif
#ifdef __arm__
        "__arm__",
#endif
#ifdef __aarch64__
        "__aarch64__",
#endif
#ifdef __ANDROID__
        "__ANDROID__",
#endif
#ifdef __ANDROID_API__
        "__ANDROID_API__=" tostr(__ANDROID_API__),
#endif
        NULL};

#undef tostr

    void insn_host_definition(void *fp)
    {
      void *sp = fp;
      uint32_t idx = ewa_rstack_get_i32(&sp);
      sp = fp;
      ewa_rstack_put_ref(&sp, host_definition[idx]);
    }

#include <stdio.h>
#include <errno.h>
#define _wargref(vname) void *vname = ewa_rstack_get_ref(&sp);
#define _wargi32(vname) uint32_t vname = ewa_rstack_get_i32(&sp);
#define _wargi64(vname) uint64_t vname = ewa_rstack_get_i32(&sp);
    void insn_fopen(void *fp)
    {
      void *sp = fp;
      _wargref(filename)
          _wargref(mode)
              sp = fp;
      FILE *f = fopen((const char *)filename, (const char *)mode);
      ewa_rstack_put_ref(&sp, f);
      ewa_rstack_put_i32(&sp, errno);
    }

    void insn_fread(void *fp)
    {
      void *sp = fp;
      _wargref(buf)
          _wargi32(size)
              _wargi32(count)
                  _wargref(file)
                      sp = fp;
      uint32_t nread = (uint32_t)fread(buf, size, count, (FILE *)file);
      ewa_rstack_put_i32(&sp, nread);
      ewa_rstack_put_i32(&sp, errno);
    }

    void insn_fwrite(void *fp)
    {
      void *sp = fp;
      _wargref(buf)
          _wargi32(size)
              _wargi32(count)
                  _wargref(file)
                      sp = fp;
      uint32_t nwrite = (uint32_t)fwrite(buf, size, count, (FILE *)file);
      ewa_rstack_put_i32(&sp, nwrite);
      ewa_rstack_put_i32(&sp, errno);
    }

    void insn_fclose(void *fp)
    {
      void *sp = fp;
      _wargref(file)
          sp = fp;
      fclose((FILE *)file);
      ewa_rstack_put_i32(&sp, errno);
    }

    void insn_stdio(void *fp)
    {
      void *sp = fp;
      ewa_rstack_put_ref(&sp, stdin);
      ewa_rstack_put_ref(&sp, stdout);
      ewa_rstack_put_ref(&sp, stderr);
    }

  }

}